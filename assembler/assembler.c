#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "pre_assembler.h"


bool process_file(char *filename);

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

    @param filename The name of the file to be processed.
    @return TRUE if the file processing succeeds, FALSE otherwise.
*/
bool process_file(char *filename) {
    /* Initialize variables and data structures */
    /* NOTE probably more variables will be created here, too early to know :) */
    long ic = 0, dc = 0;
    /* TODO: Initialize data image to an array of data words of size 1024 */
    /* TODO: Initialize code image to an array of machine words of size 1024 */
    /* TODO: Initialize a temporary line of size MAX_LINE_LENGTH + 2.
             This temp line holds the line that was read each time. */
    /* TODO: Initialize symbol table */
    /* TODO: Initialize Abstract Syntax Tree (AST) */
    bool success = TRUE, has_extern = FALSE, has_entry = FALSE;

    /* Preprocessing step */
    if (!preprocess_file(filename)) {
        printf("Error: Preprocessing step failed. Macros could not be spread correctly.\n");
        return FALSE;
    }

    /* Open the file created by the preprocessor */
    /* TODO: Open the file created by the preprocessor using fopen.
             Handle the error if the pointer is null. */

    /* First pass */
    /* TODO: Perform the first pass - process each line in the .am file and update the success variable */

    /* Second pass */
    /* TODO: Perform the second pass and update the success variable */

    /* Check if we have exceeded the memory size */
    if (ic + dc > MEMORY_SIZE - 100) {
        /* First 100 memory cells reserved for the system */
        /* TODO: Print an error indicating that the memory size is too small for the file */
        success = FALSE;
    }

    /* Print output files if success */
    if (success) {
        /* TODO: Create the object file */
        if (has_entry)
            exit(0);
        /* TODO: Create the entry file */
        if (has_extern)
            exit(0);
        /* TODO: Create the extern file */
    }

    /* Free all allocated memory */
    /* TODO: Free all allocated memory */

    return TRUE;
}
