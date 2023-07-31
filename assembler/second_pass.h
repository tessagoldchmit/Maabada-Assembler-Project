#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "data_structures.h"
#include "symbols.h"

bool second_pass_process(char *filename_with_am_suffix, int *ic, int *dc, data_image *my_data_image,
                              code_image *my_code_image, symbol_table *symbol_table);

void second_pass_encode_symbols();


#endif /* SECOND_PASS_H */