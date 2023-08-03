#include <stdio.h>
#include <stdlib.h>
#include "second_pass.h"
#include "temp.h"
#include "data_structures.h"
#include "string.h"

#define A 0
#define R 2
#define E 1

int get_symbol_address(symbol_table *table, symbol symbol_name) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            return current->decimal_address;
        }
        current = current->next_symbol;
    }
}

void decode_code_group_a(code_node *current_code_node, symbol_table* symbol_table) {
    if (current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_type == REGISTER_OPERAND_TYPE &&
        current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_type == REGISTER_OPERAND_TYPE){
        current_code_node->word[1] = insert_bits(current_code_node->word[1], A, 0, 1);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], atoi(&current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.register_num),2, 6);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], atoi(&current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num),7, 11);
    }


    current_code_node->word[1] = insert_bits(current_code_node->word[1],
                                             atoi(&current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num),
                                             7, 11);
    current_code_node->word[1] = insert_bits(current_code_node->word[1], A, 0, 1);
    current_code_node->word[2] = insert_bits(current_code_node->word[2], get_symbol_address(symbol_table,
                                                                                            current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.symbol),
                                             2, 11);
    current_code_node->word[2] = insert_bits(current_code_node->word[2], R, 0, 1);
    print_binary_12bits(current_code_node->word[0]);
    printf("\n");
    print_binary_12bits(current_code_node->word[1]);
    printf("\n");
    print_binary_12bits(current_code_node->word[2]);
    printf("\n");
}

void decode_code(symbol_table *symbol_table, ast ast_line_info, code_image *my_code_image, int *ic) {
    code_node *current_code_node = my_code_image->first;
    while (current_code_node) {
        if (check_group(current_code_node->ast.ast_word.instruction_word.instruction_name) == GROUP_A) {
            decode_code_group_a(current_code_node, symbol_table);
        }
        printf("-------\n");
        current_code_node = current_code_node->next;
    }
}

/*
    Processes a line during the second pass of the assembler.

    @param ast The ast tree representing the structure of the line.
    @param table The symbol table used for tracking symbols.
    @param line Information about the processed line.
    @return TRUE if the line is processed successfully, FALSE otherwise.
*/
bool second_pass_process(char *filename_with_am_suffix, int *ic, int *dc, data_image *my_data_image,
                         code_image *my_code_image, symbol_table *symbol_table) {
    printf("\nin second pass\n");
    *ic = 0;

    /* TODO macro for repetitives */
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    /* Open .am file */
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", filename_with_am_suffix);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        printf("\n------------------------------------------------------------------------------\n");
        printf(line);
        ast ast_line_info = get_ast_line_info(line);
        print_ast(&ast_line_info);

        if (ast_line_info.ast_word_type == DIRECTIVE) {
            if (ast_line_info.ast_word.directive_word.directive_type == ENTRY_TYPE) {
                mark_symbol_as_entry(symbol_table, ast_line_info.ast_word.directive_word.directive_option.symbol);
            }
        } else {
            decode_code(symbol_table, ast_line_info, my_code_image, ic);
        }
    }
    return TRUE;
}
