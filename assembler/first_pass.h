#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "data_structures.h"
#include "symbols.h"

/**
    Processes the first pass of assembling, analyzing and updating symbol information.

    @param filename_with_am_suffix The name of the .am file to process.
    @param ic A pointer to the instruction counter value.
    @param dc A pointer to the data counter value.
    @param my_data_image The data image to update with data information.
    @param my_code_image The code image to update with code information.
    @param symbol_table The symbol table to update with symbol information.
    @return TRUE if the first pass process succeeds, FALSE otherwise.
*/
bool first_pass_process(char *base_filename, int *ic, int *dc, data_image *my_data_image, code_image *code_image,
                        symbol_table *symbol_table);

#endif /* FIRST_PASS_H */