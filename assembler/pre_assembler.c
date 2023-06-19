#include "pre_assembler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Preprocesses a C file by removing comments, empty lines, and spreading macros.
 * At the end of the preprocessing, any dynamically allocated memory is freed.
 *
 * @param base_filename: The name of the C file (without the suffix) to be preprocessed.
 * @return: True if the preprocessing is successful, False otherwise.
 */
bool preprocess_file(char *base_filename) {
    /* Variables */
    bool is_macro_flag=FALSE, replaced_flag=FALSE;
    macro_array macro_array;

    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_MACRO_LENGTH], macro_content[MAX_MACRO_INST];

    char *filename_with_as_suffix;
    char *filename_with_am_suffix;

    FILE *as_file;
    FILE *am_file;

    /* Create filename with .as suffix */
    filename_with_as_suffix = malloc(strlen(base_filename) + 4); /* +4 for ".as\0" */
    strcpy(filename_with_as_suffix, base_filename);
    strcat(filename_with_as_suffix, ".as");

    /* Create filename with .am suffix */
    filename_with_am_suffix = malloc(strlen(base_filename) + 4); /* +4 for ".am\0" */
    strcpy(filename_with_am_suffix, base_filename);
    strcat(filename_with_am_suffix, ".am");

    // Open .as file
    as_file = fopen(filename_with_as_suffix, "r");
    if (as_file == NULL) {
        fprintf(stderr, "Error: Failed to open .as file '%s' for reading.\n", filename_with_as_suffix);
        return FALSE;
    }

    // Open .am file
    am_file = fopen(filename_with_am_suffix, "w");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for writing.\n", filename_with_am_suffix);
        fclose(as_file);
        return FALSE;
    }

    /* Initialize variables */
    macro_content[0] = 0;

    /* Initialize macro array */
    init_macro_array(&macro_array, 1);

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, as_file)) {

        /* Skip empty lines and comments (lines starting with ';') */
        /* TODO tessa - handle cases where comments don't start at index 0 */
        if (empty_string(line) || line[0] == ';') {
            continue;
        }

        /* Remove leading spaces and get the command length */
        char *main_str_ptr = skip_spaces(line); /* Points to first word */
        int command_length = word_length(main_str_ptr);
        int macro_status;

        /* Check the status of the current line with respect to macros */
        macro_status = current_macro_status(main_str_ptr, is_macro_flag, command_length);

        if (macro_status == START_OF_MACRO) {  /* Start of a new macro definition */
            is_macro_flag = TRUE;
            main_str_ptr = skip_spaces(main_str_ptr + command_length);
            command_length = word_length(main_str_ptr);
            strncpy(macro_name, main_str_ptr, command_length); /* Saves macro name */

        } else if (macro_status == BODY_OF_MACRO) {  /* Inside a macro definition, append to macro content */
            strncat(macro_content, line, count_line_length(line));

        } else if (macro_status == END_OF_MACRO) {  /* End of a macro definition, insert into macro array */
            is_macro_flag = FALSE;
            insert_macro_array(&macro_array, macro_name, macro_content);
            macro_content[0] = 0;

        } else {
            replaced_flag = FALSE;

            /* Check if the line contains a macro reference and replace it */
            int idx;
            for (idx = 0; idx < macro_array.used; idx++) {
                if (strncmp(main_str_ptr, macro_array.array[idx].name, command_length) == 0 &&
                    empty_string(main_str_ptr + command_length)) { /* If it's a name of a macro */
                    replaced_flag = TRUE;
                    fputs(macro_array.array[idx].content, am_file);
                    break;
                }
            }
            /* If it's not a macro reference, copy the line to the destination file */
            if (!replaced_flag)
                fputs(line, am_file);
        }
    }

    /* Free memory and close files */
    free_macro_array(&macro_array);
    fclose(as_file);
    fclose(am_file);
    return TRUE;
}

/**
 * Initializes the macro array.
 *
 * @param arr: Pointer to the macro array.
 * @param initial_size: Initial size of the macro array.
 */
void init_macro_array(macro_array *arr, int initial_size) {
    arr->array = malloc(initial_size * sizeof(macro));
    arr->used = 0;
    arr->size = initial_size;
}

/**
 * Inserts a macro into the macro array.
 *
 * @param arr: Pointer to the macro array.
 * @param name: Name of the macro to insert.
 * @param content: Content of the macro to insert.
 */
void insert_macro_array(macro_array *arr, char *name, char *content) {
    if (arr->used == arr->size) {
        arr->size *= 2;
        void* temp = realloc(arr->array, arr->size * sizeof(macro));
        if (!temp) {
            fprintf(stderr, "Error: Failed to reallocate memory.\n");
            exit(1);
        }
        arr->array = temp;
    }
    arr->array[arr->used].name = malloc(strlen(name) + 1);
    arr->array[arr->used].content = malloc(strlen(content) + 1);
    if (!arr->array[arr->used].name || !arr->array[arr->used].content) {
        fprintf(stderr, "Error: Failed to allocate memory for macro name or content.\n");
        exit(1);
    }
    strcpy(arr->array[arr->used].name, name);
    strcpy(arr->array[arr->used].content, content);
    arr->used++;
}


/**
 * Frees all memory used by the macro array.
 *
 * @param arr: Pointer to the macro array.
 */
void free_macro_array(macro_array *arr) {
    int idx;
    for (idx = 0; idx < arr->used; idx++) {
        free(arr->array[idx].name);
        free(arr->array[idx].content);
    }
    free(arr->array);
    arr->array = NULL;
    arr->used = arr->size = 0;
}

/**
 * Determines the current "macro status" based on the given input.
 *
 * @param str: Input string to analyze.
 * @param macro_flag: Flag indicating if currently processing a macro.
 * @param cmd_length: Length of the command.
 * @return: Macro status: START_OF_MACRO, BODY_OF_MACRO, END_OF_MACRO, or NOT_A_MACRO.
 */
int current_macro_status(char *str, bool macro_flag, int cmd_length) {
    if (strncmp(str, "mcr", strlen("mcr")) == 0 && str[3] && str[3] == ' ') { /* First word is 'macro'*/
        return START_OF_MACRO;
    } else if (macro_flag && strncmp(str, "endmcr", cmd_length) != 0) {
        return BODY_OF_MACRO;
    } else if (macro_flag && strncmp(str, "endmcr", cmd_length) == 0) {
        return END_OF_MACRO;
    }
    return NOT_A_MACRO;
}
