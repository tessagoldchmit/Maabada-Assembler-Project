#include "pre_assembler.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "utils.h"
#include "logs.h"

/**
 * Initializes the macro array.
 *
 * @param arr: Pointer to the macro array.
 * @param initial_size: Initial size of the macro array.
 */
void init_macro_array(macro_array *, int);

/**
 * Inserts a macro into the macro array.
 *
 * @param arr: Pointer to the macro array.
 * @param name: Name of the macro to insert.
 * @param content: Content of the macro to insert.
 */
void insert_macro_array(macro_array *arr, char *name, char *content);


/**
 * Frees all memory used by the macro array.
 *
 * @param arr: Pointer to the macro array.
 */
void free_macro_array(macro_array *);

/**
 * Determines the current "macro status" based on the given input.
 *
 * @param str: Input string to analyze.
 * @param macro_flag: Flag indicating if currently processing a macro.
 * @param cmd_length: Length of the word_union.
 * @return: Macro status: START_OF_MACRO, BODY_OF_MACRO, END_OF_MACRO, or NOT_A_MACRO.
 */
int current_macro_status(char *, bool, int);

bool preprocess_file(char *base_filename) {
    /* Variables */
    bool is_macro_flag=FALSE, replaced_flag;
    macro_array macro_array;

    char line[MAX_LINE_LENGTH];
    char macro_name[MAX_MACRO_LENGTH], macro_content[MAX_MACRO_INST];

    FILE *as_file;
    FILE *am_file;

    /* Create filename with .as suffix */
    char *filename_with_as_suffix = concatenate_strings(base_filename, ".as");

    /* Create filename with .am suffix */
    char *filename_with_am_suffix = concatenate_strings(base_filename, ".am");


    /* Open .as file  */
    as_file = fopen(filename_with_as_suffix, "r");
    if (as_file == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_READING_AS_FILE);
        return FALSE;
    }

    /* Open .am file */
    am_file = fopen(filename_with_am_suffix, "w");
    if (am_file == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_WRITING_TO_AM_FILE);
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
        char *main_str_ptr = skip_spaces(line); /* Points to first ast_word */
        if (empty_string(line) || main_str_ptr[0] == ';') {
            continue;
        }

        /* Remove leading spaces and get the word_union length */
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

void init_macro_array(macro_array *arr, int initial_size) {
    arr->array = malloc(initial_size * sizeof(macro));
    if (arr->array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(arr->array);
        exit(1);
    }
    arr->used = 0;
    arr->size = initial_size;
}

void insert_macro_array(macro_array *arr, char *name, char *content) {
    if (arr->used == arr->size) {
        arr->size *= 2;
        void *temp = realloc(arr->array, arr->size * sizeof(macro));
        if (!temp) {
            PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_REALLOCATE_MEM);
            exit(EXIT_FAILURE);
        }
        arr->array = temp;
    }
    arr->array[arr->used].name = malloc(strlen(name) + 1);
    arr->array[arr->used].content = malloc(strlen(content) + 1);
    if (!arr->array[arr->used].name || !arr->array[arr->used].content) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(arr->array[arr->used].name);
        free(arr->array[arr->used].content);
        exit(EXIT_FAILURE);
    }
    strcpy(arr->array[arr->used].name, name);
    strcpy(arr->array[arr->used].content, content);
    arr->used++;
}

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

int current_macro_status(char *str, bool macro_flag, int cmd_length) {
    if (strncmp(str, MACRO_START, strlen(MACRO_START)) == 0 && str[4] && str[4] == ' ') { /* First ast_word is 'macro'*/
        return START_OF_MACRO;
    } else if (macro_flag && strncmp(str, MACRO_END, cmd_length) != 0) {
        return BODY_OF_MACRO;
    } else if (macro_flag && strncmp(str, MACRO_END, cmd_length) == 0) {
        return END_OF_MACRO;
    }
    return NOT_A_MACRO;
}
