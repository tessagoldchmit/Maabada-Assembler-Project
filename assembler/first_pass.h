#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "globals.h"
#include "data_structures.h"

bool is_symbol(char* line);

bool is_data_or_string(char* line);

void decode_data(directive_type directive_type, char *char_ptr, long *dc, char *symbol_name, data_image *my_data_image);

bool is_extern_entry(char *line, char *char_ptr);

bool is_extern(char *line, char *char_ptr);

void add_extern_symbols();

int analyze_operands();

void update_data_dc(symbol_table *symbol_table, long ic);

bool first_pass_process(char *base_filename, long ic, long dc, data_image *my_data_image, code_image *code_image, symbol_table symbol_table);

#endif /* FIRST_PASS_H */