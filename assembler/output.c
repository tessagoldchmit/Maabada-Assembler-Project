#include "output.h"
#include "utils.h"

/**
 * Converts a 6-bit binary number to the corresponding Base64 character.
 * @param binary_number The 6-bit binary number to be converted.
 * @return The corresponding Base64 character.
 */
char binary_to_base64(unsigned int binary_number) {
    unsigned int six_bit_group = binary_number & 0b111111;

    if (six_bit_group <= 25) {
        return 'A' + six_bit_group; /* 'A' to 'Z' */
    } else if (six_bit_group <= 51) {
        return 'a' + (six_bit_group - 26); /* 'a' to 'z' */
    } else if (six_bit_group <= 61) {
        return '0' + (six_bit_group - 52); /* '0' to '9'  */
    } else if (six_bit_group == 62) {
        return '+'; /* Base64 character '+' */
    } else if (six_bit_group == 63) {
        return '/'; /* Base64 character '/' */
    } else {
        return '='; /* Padding character '=' */
    }
}

/**
 * Writes the object file (.ob) containing the code and data images.
 * @param filename The name of the source file (without extension).
 * @param my_code_image Pointer to the code image structure.
 * @param ic Pointer to the Instruction Counter value.
 * @param my_data_image Pointer to the data image structure.
 * @param dc Pointer to the Data Counter value.
 */
void write_object_file(char *filename, code_image *my_code_image, int *ic, data_image *my_data_image, int *dc) {
    FILE *file;
    char *filename_with_obj_suffix = concatenate_strings(filename, ".ob");
    unsigned int binary_numbers_arr[*ic + *dc];
    int i = 0;

    /* Open .ob file */
    file = fopen(filename_with_obj_suffix, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open .ob file '%s' for writing.\n", filename_with_obj_suffix);
        return;
    }
    fprintf(file, "%d %d\n", *ic, *dc);

    /* Add code image's binary numbers to the array */
    code_node *current_code_node = my_code_image->first;
    while (current_code_node != NULL) {
        int j;
        for (j = 0; j < current_code_node->L; j++) {
            binary_numbers_arr[i] = current_code_node->word[j];
            i++;
        }
        current_code_node = current_code_node->next;
    }

    /* Add data image's binary numbers to the array */
    data_node *current_data_node = my_data_image->first;
    while (current_data_node != NULL) {
        int j;
        for (j = 0; j < current_data_node->L; j++) {
            binary_numbers_arr[i] = current_data_node->word[j];
            i++;
        }
        current_data_node = current_data_node->next_node;
    }

    /* Write the binary numbers to the file as base64 characters */
    for (i = 0; i < *ic + *dc; i++) {
        unsigned int binary_number = binary_numbers_arr[i];

        unsigned int left_bits = (unsigned int)binary_number >> 6;
        unsigned int right_bits = binary_number & 0b111111;

        fputc(binary_to_base64(left_bits), file);
        fputc(binary_to_base64(right_bits), file);
        fputc('\n', file);
    }

    /* Free the allocated memory */
    fclose(file);
}

/**
 * Writes the entries file (.ent) containing the symbols marked as entry points.
 * @param filename The name of the source file (without extension).
 * @param table Pointer to the symbol table structure.
 */
void write_entries_file(char *filename, symbol_table *table) {
    char entries_filename[MAX_FILE_NAME + 5]; /* +5 for ".ent\0" */
    snprintf(entries_filename, sizeof(entries_filename), "%s.ent", filename);

    FILE *entries_file = fopen(entries_filename, "w");
    if (entries_file == NULL) {
        fprintf(stderr, "Error opening entries file for writing.\n");
        return;
    }

    symbol_node *current = table->first;
    while (current != NULL) {
        if (current->symbol_type == ENTRY) {
            int current_symbol_address = get_symbol_address(table, current->symbol_name) + START_OF_MEMORY_ADDRESS;
            fprintf(entries_file, "%s\t%d\n", current->symbol_name, current_symbol_address);
        }
        current = current->next_symbol;
    }

    fclose(entries_file);
}

/**
 * Writes the externals file (.ext) containing the symbols marked as externals.
 * @param filename The name of the source file (without extension).
 * @param table Pointer to the extern table structure.
 */
void write_externals_file(char *filename, extern_table *table) {
    char externals_filename[MAX_FILE_NAME + 5]; /* +5 for ".ext\0" */
    snprintf(externals_filename, sizeof(externals_filename), "%s.ext", filename);

    FILE *externals_file = fopen(externals_filename, "w");
    if (externals_file == NULL) {
        fprintf(stderr, "Error opening entries file for writing.\n");
        return;
    }

    extern_node *current = table->first;
    while (current != NULL) {
        fprintf(externals_file, "%s\t%d\n", current->symbol_name, current->address + START_OF_MEMORY_ADDRESS);
        current = current->next;
    }

    fclose(externals_file);
}