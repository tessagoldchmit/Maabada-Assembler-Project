#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"
#include "symbols.h"
#include "extern_symbols.h"

void write_object_file(char *filename, code_image *my_code_image, int *ic, data_image *my_data_image, int *dc);

void write_entries_file(char *filename, symbol_table *table);

void write_externals_file(char *filename, extern_table *table);

#endif /* OUTPUT_H */