#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pre_assembler.h"
#include "first_pass.h"
#include "utils.h"
#include "output.h"
#include "second_pass.h"
#include "symbols.h"
#include "logs.h"


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
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_NO_FILES_PROVIDED);
        exit(EXIT_FAILURE);
    }

    /* Process each file if the file name does not exceed the maximum length */
    PRINT_MESSAGE(INFO_MSG_TYPE, INFO_START_OF_ASSEMBLER);
    for (i = 1; i < argc; i++) {
        if (strlen(argv[i]) > MAX_FILE_NAME) {
            PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FILE_NAME_TOO_LONG);
            continue;
        }
        printf("\n");
        PRINT_MESSAGE(INFO_MSG_TYPE, argv[i]);
        process_file(argv[i]);
    }
    printf("\n");
    PRINT_MESSAGE(INFO_MSG_TYPE, INFO_ALL_FILES_HAVE_BEEN_PROCESSED);
    return 0;
}

/**
    The function takes a file name (without the suffix) as input and converts it into machine code.

    @param base_filename The name of the file to be processed.
    @return TRUE if the file processing succeeds, FALSE otherwise.
*/
bool process_file(char *base_filename) {
    /* Initialize variables and data structures */
    int *ic;
    int *dc;
    data_image *my_data_image;
    code_image *my_code_image;
    symbol_table *symbol_table;
    extern_table *extern_table;
    bool success = TRUE, has_extern, has_entry;
    char *filename_with_am_suffix;

    ic = malloc(sizeof(int));
    if (ic == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(ic);
        exit(1);
    }
    dc = malloc(sizeof(int));
    if (dc == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(dc);
        exit(1);
    }
    *ic=0;
    *dc=0;

    my_data_image = initialize_data_image();
    my_code_image = initialize_code_image();
    symbol_table = initialize_symbol_table();
    extern_table = initialize_extern_table();

    /* Preprocessing step */
    if (!preprocess_file(base_filename)) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_IN_PREPROCESSING);
        /* Free allocated memory */
        free(dc);
        free(ic);
        free_all_data_structures(my_code_image, my_data_image);
        free_symbols(symbol_table, extern_table);
        return FALSE;
    }

    /* First pass */
    filename_with_am_suffix = concatenate_strings(base_filename, ".am");
    if (!first_pass_process(filename_with_am_suffix, ic, dc, my_data_image, my_code_image, symbol_table)) {
        /* Free allocated memory */
        free(dc);
        free(ic);
        free(filename_with_am_suffix);
        free_all_data_structures(my_code_image, my_data_image);
        free_symbols(symbol_table, extern_table);
        return FALSE;
    }

    /* Second pass */
    if (!second_pass_process(filename_with_am_suffix, ic, my_code_image, symbol_table, extern_table)) {
        /* Free allocated memory */
        free(dc);
        free(ic);
        free(filename_with_am_suffix);
        free_all_data_structures(my_code_image, my_data_image);
        free_symbols(symbol_table, extern_table);
        return FALSE;
    }

    /* Check if we have exceeded the memory size */
    if ((*ic + *dc) > (MEMORY_SIZE - START_OF_MEMORY_ADDRESS)) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_MEMORY_SIZE_EXCEEDED);
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
        PRINT_MESSAGE(INFO_MSG_TYPE, INFO_FILES_GENERATED_SUCCESSFULLY);
    }

    /* Free allocated memory */
    free(ic);
    free(dc);
    free(filename_with_am_suffix);
    free_all_data_structures(my_code_image, my_data_image);
    free_symbols(symbol_table, extern_table);

    return TRUE;
}
