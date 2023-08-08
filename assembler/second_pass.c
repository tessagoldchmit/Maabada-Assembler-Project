#include <stdio.h>
#include <stdlib.h>
#include "second_pass.h"
#include "temp.h"
#include "data_structures.h"
#include "string.h"
#include "symbols.h"

#define R 2
#define E 1

int get_correct_a_r_e_for_source(ast ast_line_info, symbol_table *symbol_table) {
    int symbol_type = -1;
    char *symbol_name = ast_line_info.ast_word.instruction_word.instruction_union.group_a.source_value.symbol;
    symbol_node *current_symbol_node = symbol_table->first;
    while (current_symbol_node != NULL) {
        if (strcmp(current_symbol_node->symbol_name, symbol_name) == 0) {
            symbol_type = current_symbol_node->symbol_type;
            break;
        }
        current_symbol_node = current_symbol_node->next_symbol;
    }
    if (symbol_type == EXTERNAL)
        return E;
    return R;
}

int get_correct_a_r_e_for_target(ast ast_line_info, symbol_table *symbol_table) {
    int symbol_type = -1;
    char *symbol_name = ast_line_info.ast_word.instruction_word.instruction_union.group_a.target_value.symbol;
    symbol_node *current_symbol_node = symbol_table->first;
    while (current_symbol_node != NULL) {
        if (strcmp(current_symbol_node->symbol_name, symbol_name) == 0) {
            symbol_type = current_symbol_node->symbol_type;
            break;
        }
        current_symbol_node = current_symbol_node->next_symbol;
    }
    if (symbol_type == EXTERNAL)
        return E;
    return R;
}


void decode_code_group_a(code_node *current_code_node, symbol_table *symbol_table) {
    int source_type = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_type;
    int target_type = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_type;

    /* Handle case of 2 registers */
    if (source_type == REGISTER_OPERAND_TYPE && target_type == REGISTER_OPERAND_TYPE) {
        int target_register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.register_num);
        int source_register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num);

        current_code_node->word[1] = insert_bits(current_code_node->word[1], target_register_num, 2, 6);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], source_register_num, 7, 11);
        return;
    }

    /* Handle source */
    if (source_type == REGISTER_OPERAND_TYPE) {
        int source_register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], source_register_num, 7, 11);

    } else if (source_type == SYMBOL_OPERAND_TYPE) {
        char *source_symbol = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.symbol;
        int source_symbol_address = get_symbol_address(symbol_table, source_symbol);
        if (source_symbol_address != 1) {
            int source_are = get_correct_a_r_e_for_source(current_code_node->ast, symbol_table);
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_are, 0, 1);
            source_symbol_address = source_symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_symbol_address, 2, 11);
        } else {
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_symbol_address, 0, 11);
        }

    } else if (source_type == NUMBER_OPERAND_TYPE) {
        int source_number = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.number;
        current_code_node->word[1] = insert_bits(current_code_node->word[1], source_number, 2, 11);
    }

    /* Handle target */
    if (target_type == REGISTER_OPERAND_TYPE) {
        int target_register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.register_num);
        current_code_node->word[2] = insert_bits(current_code_node->word[2], target_register_num, 2, 6);

    } else if (target_type == SYMBOL_OPERAND_TYPE) {
        char *target_symbol = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.symbol;
        int target_symbol_address = get_symbol_address(symbol_table, target_symbol);
        if (target_symbol_address != 1) {
            int target_are = get_correct_a_r_e_for_target(current_code_node->ast, symbol_table);
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_are, 0, 1);
            target_symbol_address = target_symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_symbol_address, 2, 11);
        } else {
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_symbol_address, 0, 11);
        }


    } else if (target_type == NUMBER_OPERAND_TYPE) {
        int target_number = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.number;
        current_code_node->word[2] = insert_bits(current_code_node->word[2], target_number, 2, 11);
    }
}


void decode_code_group_b(code_node *current_code_node, symbol_table *symbol_table) {
    int target_type = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_type;

    /* handle target */
    if (target_type == REGISTER_OPERAND_TYPE) {
        int register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.register_num);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], register_num, 2, 6);

    } else if (target_type == SYMBOL_OPERAND_TYPE) {
        char *symbol = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.symbol;
        int symbol_address = get_symbol_address(symbol_table, symbol);
        if (symbol_address != 1) {
            int target_are = get_correct_a_r_e_for_target(current_code_node->ast, symbol_table);
            current_code_node->word[1] = insert_bits(current_code_node->word[1], target_are, 0, 1);
            symbol_address = symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[1] = insert_bits(current_code_node->word[1], symbol_address, 2, 11);
        } else {
            current_code_node->word[1] = insert_bits(current_code_node->word[1], symbol_address, 0, 11);
        }

    } else if (target_type == NUMBER_OPERAND_TYPE) {
        int number = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.number;
        current_code_node->word[1] = insert_bits(current_code_node->word[1], number, 2, 11);
    }
}


void decode_code(symbol_table *symbol_table, code_node *current_code_node) {
    if (check_group(current_code_node->ast.ast_word.instruction_word.instruction_name) == GROUP_A) {
        decode_code_group_a(current_code_node, symbol_table);
    } else if (check_group(current_code_node->ast.ast_word.instruction_word.instruction_name) == GROUP_B) {
        decode_code_group_b(current_code_node, symbol_table);
    }

    /* Todo delete me :) */
    int i;
    for (i = 0; i < current_code_node->L; i++) {
        print_binary_12bits(current_code_node->word[i]);
        printf("\n");
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

    /* TODO macro for repetitives */
    int line_number=1;
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
        ast ast_line_info = get_ast_line_info(line, line_number);
        print_ast(&ast_line_info);

        if (ast_line_info.ast_word_type == DIRECTIVE) {
            if (ast_line_info.ast_word.directive_word.directive_type == ENTRY_TYPE) {
                mark_symbol_as_entry(symbol_table, ast_line_info.ast_word.directive_word.directive_option.symbol);
            }

            /* Todo delete me :) */
            if (ast_line_info.ast_word.directive_word.directive_type == DATA_TYPE ||
                ast_line_info.ast_word.directive_word.directive_type == STRING_TYPE) {
                data_node *current_data_node = find_data_node_by_line(my_data_image, line);
                int i;
                for (i = 0; i < current_data_node->L; i++) {
                    print_binary_12bits(current_data_node->word[i]);
                    printf("\n");
                }
            }

        } else {
            code_node *current_code_node = find_code_node_by_line(my_code_image, line);
            decode_code(symbol_table, current_code_node);
        }
        line_number++;
    }
    return TRUE;
}
