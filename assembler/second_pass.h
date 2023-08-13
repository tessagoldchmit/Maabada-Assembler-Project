#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "data_structures.h"
#include "symbols.h"

/* TODO enscpulate tessa */
#define R 2
#define E 1

/**
    Performs the second pass of the assembly process to update the instruction counter and decode instructions.

    @param filename_with_am_suffix The name of the .am file to be processed.
    @param ic A pointer to the instruction counter value.
    @param my_code_image The code image containing the assembled instructions.
    @param symbol_table The symbol table containing symbol information.
    @param extern_table The external symbol table containing external symbol information.
    @return TRUE if the second pass processing succeeds, FALSE otherwise.
*/
bool second_pass_process(char *filename_with_am_suffix, int *ic, code_image *my_code_image, symbol_table *symbol_table,
                         extern_table *extern_table);


#endif /* SECOND_PASS_H */