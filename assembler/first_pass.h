#ifndef FIRST_PASS_H
#define FIRST_PASS_H

#include "data_structures.h"
#include "symbols.h"

bool is_symbol(char *line);

void decode_data(ast ast_line_info, long *dc, data_image *my_data_image);

int analyze_operands(char *char_ptr, code_image *my_code_image);

void update_data_dc(symbol_table *symbol_table, long ic);

bool first_pass_process(char *base_filename, long ic, long dc, data_image *my_data_image, code_image *code_image,
                        symbol_table *symbol_table);

#endif /* FIRST_PASS_H */