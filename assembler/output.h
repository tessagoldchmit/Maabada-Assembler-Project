#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"
#include "symbols.h"

/**
 * Writes the object file (.ob) containing the code and data images.
 * @param filename The name of the source file (without extension).
 * @param my_code_image Pointer to the code image structure.
 * @param ic Pointer to the Instruction Counter value.
 * @param my_data_image Pointer to the data image structure.
 * @param dc Pointer to the Data Counter value.
 */
void write_object_file(char *filename, code_image *my_code_image, int *ic, data_image *my_data_image, int *dc);

/**
 * Writes the entries file (.ent) containing the symbols marked as entry points.
 * @param filename The name of the source file (without extension).
 * @param table Pointer to the symbol table structure.
 */
void write_entries_file(char *filename, symbol_table *table);

/**
 * Writes the externals file (.ext) containing the symbols marked as externals.
 * @param filename The name of the source file (without extension).
 * @param table Pointer to the extern table structure.
 */
void write_externals_file(char *filename, extern_table *table);

#endif /* OUTPUT_H */