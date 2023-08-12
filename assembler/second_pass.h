#ifndef SECOND_PASS_H
#define SECOND_PASS_H

#include "globals.h"
#include "data_structures.h"
#include "symbols.h"
#include "extern_symbols.h"

#define R 2
#define E 1

bool second_pass_process(char *filename_with_am_suffix, int *ic, code_image *my_code_image, symbol_table *symbol_table,
                         extern_table *extern_table);


#endif /* SECOND_PASS_H */