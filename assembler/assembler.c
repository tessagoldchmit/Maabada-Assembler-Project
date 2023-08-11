#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pre_assembler.h"
#include "first_pass.h"
#include "utils.h"
#include "output.h"
#include "second_pass.h"
#include "symbols.h"
#include "extern_symbols.h"


bool process_file(char *base_filename);

/**
 * Entry point of the 12-bit assembler program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return 0 on successful execution, or a non-zero value on failure.
 */
int main(int argc, char *argv[]) {
    int i;

    /* Check that at least one file name was passed to the program */
    if (argc == 1) {
        fprintf(stderr, "Error: No file names provided.\n");
        exit(EXIT_FAILURE);
    }

    /* Process each file if the file name does not exceed the maximum length */
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > MAX_FILE_NAME) {
            fprintf(stderr, "Error: File name exceeds maximum length.\n");
            continue;
        }
        process_file(argv[i]);
    }

    printf("All files have been processed!\n");
    return 0;
}

/**
    The function takes a file name (without the suffix) as input and converts it into machine code.

    @param base_filename The name of the file to be processed.
    @return TRUE if the file processing succeeds, FALSE otherwise.
*/
bool process_file(char *base_filename) {
    /* Initialize variables and data structures */
    int *ic = malloc(sizeof(int));
    int *dc = malloc(sizeof(int));
    *ic=0;
    *dc=0;
    data_image *my_data_image;
    my_data_image = initialize_data_image();

    code_image *my_code_image;
    my_code_image = initialize_code_image();

    symbol_table *symbol_table;
    symbol_table = initialize_symbol_table();

    extern_table *extern_table;
    extern_table = initialize_extern_table();

    bool success = TRUE, has_extern, has_entry;

    /* Preprocessing step */
    if (!preprocess_file(base_filename)) {
        printf("Error: Preprocessing step failed. Macros could not be spread correctly.\n");
        return FALSE;
    }

    /* First pass */
    char *filename_with_am_suffix = concatenate_strings(base_filename, ".am");
    if (!first_pass_process(filename_with_am_suffix, ic, dc, my_data_image, my_code_image, symbol_table)) {
        printf("Error: First pass failed.\n");
        return FALSE;
    }

    /* Second pass */
    if (!second_pass_process(filename_with_am_suffix, ic, dc, my_data_image, my_code_image, symbol_table, extern_table)) {
        printf("Error: Second pass failed.\n");
        return FALSE;
    }

    /* Check if we have exceeded the memory size */
    if ((*ic + *dc) > (MEMORY_SIZE - START_OF_MEMORY_ADDRESS)) {
        printf("Error: Memory size exceeded\n");
        success = FALSE;
    }

    /* Print output files if success */
    if (success) {
        /* Check if entry and extern exist */
        has_entry = has_entry_symbol(symbol_table);
        has_extern = has_extern_symbol(symbol_table);
        write_object_file(base_filename, my_code_image, ic, my_data_image, dc);
        if (has_entry)
            write_entries_file(base_filename, symbol_table);
        if (has_extern)
            write_externals_file(base_filename, extern_table);
    }

    /* Free allocated memory */
    free(ic);
    free(dc);

    free(my_data_image);
    free(my_code_image);
    free(symbol_table);
    free(extern_table);


    return TRUE;
}
