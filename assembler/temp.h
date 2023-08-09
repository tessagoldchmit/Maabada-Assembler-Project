#ifndef MMN14_TEMP_H
#define MMN14_TEMP_H

#include "symbols.h"
#include "ast.h"

void print_binary_12bits(int num);
void print_symbol_table(symbol_table *table);
void print_ast(ast *ast_line_info);
void print_ast_directive(const directive *directive_node);
void print_ast_instruction(const instruction *instruction_node);

#endif /* MMN14_TEMP_H */
