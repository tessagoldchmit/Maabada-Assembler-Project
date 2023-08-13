#include <stdio.h>
#include <stdlib.h>

#include "second_pass.h"
#include "data_structures.h"
#include "string.h"
#include "symbols.h"
#include "logs.h"

/**
    Gets the correct A/R/E value for the source operand in Group A instruction.

    @param ast_line_info The abstract syntax tree information for the line.
    @param symbol_table The symbol table to search for the symbol type.
    @return The A/R/E value (A for Absolute, R for Relocatable, E for External).
*/
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

/**
    Gets the correct A/R/E value for the target operand in Group A instruction.

    @param ast_line_info The abstract syntax tree information for the line.
    @param symbol_table The symbol table to search for the symbol type.
    @return The A/R/E value (A for Absolute, R for Relocatable, E for External).
*/
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

/**
    Decodes the operands for Group A instruction in the code node.

    @param current_code_node The code node to decode.
    @param symbol_table The symbol table to search for symbol information.
    @param extern_table The external symbol table to add entries if needed.
    @param ic A pointer to the instruction counter value.
    @return TRUE if the decoding succeeds, FALSE otherwise.
*/
bool decode_code_group_a(code_node *current_code_node, symbol_table *symbol_table, extern_table *extern_table, int *ic) {
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
        return TRUE;
    }

    /* Handle source */
    if (source_type == REGISTER_OPERAND_TYPE) {
        int source_register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], source_register_num, 7, 11);

    } else if (source_type == SYMBOL_OPERAND_TYPE) {
        char *source_symbol = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.symbol;
        int source_symbol_address = get_symbol_address(symbol_table, source_symbol);
        if (source_symbol_address == NON_EXIST_SYMBOL_ADDRESS) {
            printf("Symbol does not exist\n");
            return FALSE;
        } else if (source_symbol_address != 1) {
            int source_are = get_correct_a_r_e_for_source(current_code_node->ast, symbol_table);
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_are, 0, 1);
            source_symbol_address = source_symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_symbol_address, 2, 11);
        } else {
            current_code_node->word[1] = insert_bits(current_code_node->word[1], source_symbol_address, 0, 11);
        }
        if (is_symbol_extern(symbol_table,
                             current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.symbol)) {
            add_extern_node(extern_table,
                            current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.source_value.symbol,
                            *ic + (current_code_node->L - 1));
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
        if (target_symbol_address == NON_EXIST_SYMBOL_ADDRESS) {
            printf("Symbol does not exist\n");
            return FALSE;
        } else if (target_symbol_address != 1) {
            int target_are = get_correct_a_r_e_for_target(current_code_node->ast, symbol_table);
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_are, 0, 1);
            target_symbol_address = target_symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_symbol_address, 2, 11);
        } else {
            current_code_node->word[2] = insert_bits(current_code_node->word[2], target_symbol_address, 0, 11);
        }
        if (is_symbol_extern(symbol_table,
                             current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.symbol)) {
            add_extern_node(extern_table,
                            current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.symbol,
                            *ic + (current_code_node->L - 1));
        }
    } else if (target_type == NUMBER_OPERAND_TYPE) {
        int target_number = current_code_node->ast.ast_word.instruction_word.instruction_union.group_a.target_value.number;
        current_code_node->word[2] = insert_bits(current_code_node->word[2], target_number, 2, 11);
    }
    return TRUE;
}

/**
    Decodes the operands for Group B instruction in the code node.

    @param current_code_node The code node to decode.
    @param symbol_table The symbol table to search for symbol information.
    @param extern_table The external symbol table to add entries if needed.
    @param ic A pointer to the instruction counter value.
    @return TRUE if the decoding succeeds, FALSE otherwise.
*/
bool decode_code_group_b(code_node *current_code_node, symbol_table *symbol_table, extern_table *extern_table, int *ic) {
    int target_type = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_type;

    /* handle target */
    if (target_type == REGISTER_OPERAND_TYPE) {
        int register_num = atoi(
                &current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.register_num);
        current_code_node->word[1] = insert_bits(current_code_node->word[1], register_num, 2, 6);

    } else if (target_type == SYMBOL_OPERAND_TYPE) {
        char *symbol = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.symbol;
        int symbol_address = get_symbol_address(symbol_table, symbol);
        if (symbol_address == NON_EXIST_SYMBOL_ADDRESS) {
            printf("Symbol does not exist\n");
            return FALSE;
        } else if (symbol_address != -1) { /* valid symbol case */
            int target_are = get_correct_a_r_e_for_target(current_code_node->ast, symbol_table);
            current_code_node->word[1] = insert_bits(current_code_node->word[1], target_are, 0, 1);
            symbol_address = symbol_address + START_OF_MEMORY_ADDRESS;
            current_code_node->word[1] = insert_bits(current_code_node->word[1], symbol_address, 2, 11);
        } else { /* extern symbol case */
            current_code_node->word[1] = insert_bits(current_code_node->word[1], 1, 0, 11);
        }
        if (is_symbol_extern(symbol_table,
                             current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.symbol)) {
            add_extern_node(extern_table,
                            current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.symbol,
                            *ic + (current_code_node->L - 1));
        }
    } else if (target_type == NUMBER_OPERAND_TYPE) {
        int number = current_code_node->ast.ast_word.instruction_word.instruction_union.group_b.target_value.number;
        current_code_node->word[1] = insert_bits(current_code_node->word[1], number, 2, 11);
    }
    return TRUE;
}

/**
    Decodes the operands for a given code node based on its instruction group.

    @param symbol_table The symbol table to search for symbol information.
    @param current_code_node The code node to decode.
    @param extern_table The external symbol table to add entries if needed.
    @param ic A pointer to the instruction counter value.
    @return TRUE if the decoding succeeds, FALSE otherwise.
*/
bool decode_code(symbol_table *symbol_table, code_node *current_code_node, extern_table *extern_table, int *ic) {
    if (check_group(current_code_node->ast.ast_word.instruction_word.instruction_name) == GROUP_A) {
        if (decode_code_group_a(current_code_node, symbol_table, extern_table, ic) == FALSE) {
            return FALSE;
        }
    } else if (check_group(current_code_node->ast.ast_word.instruction_word.instruction_name) == GROUP_B) {
        if (decode_code_group_b(current_code_node, symbol_table, extern_table, ic) == FALSE) {
            return FALSE;
        }
    }

    *ic += current_code_node->L;
    return TRUE;
}

bool second_pass_process(char *filename_with_am_suffix, int *ic, code_image *my_code_image, symbol_table *symbol_table,
                         extern_table *extern_table) {
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    bool error_flag = FALSE;
    int line_number = 1;
    *ic = 0;
    /* Open .am file */
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", filename_with_am_suffix);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        ast ast_line_info = get_ast_line_info(line, line_number);

        if (ast_line_info.ast_word_type == DIRECTIVE) {
            if (ast_line_info.ast_word.directive_word.directive_type == ENTRY_TYPE) {
                if (check_entry_symbol_duplication(symbol_table,
                                                   ast_line_info.ast_word.directive_word.directive_option.symbol) ==
                    FALSE) {
                    HANDLE_AST_ERROR_NON_POINTER(&ast_line_info, ERROR_SYMBOL_ALREADY_EXISTS);
                    error_flag = TRUE;
                } else
                    mark_symbol_as_entry(symbol_table, ast_line_info.ast_word.directive_word.directive_option.symbol);
            }


        } else {
            code_node *current_code_node = find_code_node_by_line(my_code_image, line);
            if (decode_code(symbol_table, current_code_node, extern_table, ic) == FALSE) {
                HANDLE_AST_ERROR_NON_POINTER(&ast_line_info, ERROR_SYMBOL_DOES_NOT_EXIST);
                error_flag = TRUE;
            }
        }
        line_number++;
    }
    if (error_flag) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_IN_SECOND_PASS);
        fclose(am_file);
        return FALSE;
    } else {
        PRINT_MESSAGE(INFO_MSG_TYPE, INFO_SECOND_PASS);
        fclose(am_file);
        return TRUE;
    }
}


