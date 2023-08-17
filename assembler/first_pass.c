#include <string.h>
#include <stdio.h>

#include "first_pass.h"
#include "globals.h"
#include "ast.h"
#include "logs.h"

/**
    Decodes data information and adds it to the data image.

    @param line The original line containing the data information.
    @param ast_line_info The abstract syntax tree information for the line.
    @param dc A pointer to the data counter for memory length update.
    @param my_data_image The data image where data nodes will be added.
*/
void decode_data(char *line, ast ast_line_info, int *dc, data_image *my_data_image) {
    data_node *new_node;
    int word_length = 0;
    int i;
    i=0;

    /* handle directive */
    if (ast_line_info.ast_word.directive_word.directive_type == STRING_TYPE) {
        while (ast_line_info.ast_word.directive_word.directive_option.string[i] != '\000') {
            i++;
            word_length++;
        }
        word_length++; /* for null terminating */
    } else if (ast_line_info.ast_word.directive_word.directive_type == DATA_TYPE) {
        word_length = ast_line_info.ast_word.directive_word.directive_option.machine_code.machine_code_count;
    }

    *dc += word_length; /* update memory length */

    new_node = create_data_node(line, word_length, ast_line_info);
    add_data_node(my_data_image, new_node);
}

/**
    Analyzes operands and creates code nodes based on the given instruction.

    @param line The original line containing the instruction.
    @param ast_line_info The abstract syntax tree information for the line.
    @param ic A pointer to the instruction counter for memory length update.
    @param my_code_image The code image where code nodes will be added.
    @return The length of the generated word(s).
*/
int analyze_operands(char *line, ast *ast_line_info, code_image *my_code_image) {
    code_node *new_node;
    int new_operand_code;
    int new_operand_target = 0;
    int new_operand_source = 0;
    int L = 0;

    if (check_group(ast_line_info->ast_word.instruction_word.instruction_name) == GROUP_A) {
        new_operand_code = ast_line_info->ast_word.instruction_word.instruction_name;
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.target_type == NUMBER_OPERAND_TYPE) {
            if (ast_line_info->ast_word.instruction_word.instruction_name == MOV_TYPE ||
                ast_line_info->ast_word.instruction_word.instruction_name == ADD_TYPE ||
                ast_line_info->ast_word.instruction_word.instruction_name == SUB_TYPE ||
                ast_line_info->ast_word.instruction_word.instruction_name == LEA_TYPE) {
                HANDLE_AST_ERROR_NON_POINTER(ast_line_info, ERROR_ILLEGAL_TARGET_OPERAND_TYPE_GROUP_A);
                return -1;
            } else {
                new_operand_target = IMMEDIATE_ADDRESS;
            }
        }
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.target_type == SYMBOL_OPERAND_TYPE)
            new_operand_target = DIRECT_ADDRESS;
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.target_type == REGISTER_OPERAND_TYPE)
            new_operand_target = REGISTER_ADDRESS;

        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.source_type == NUMBER_OPERAND_TYPE) {
            if (ast_line_info->ast_word.instruction_word.instruction_name == LEA_TYPE) {
                HANDLE_AST_ERROR_NON_POINTER(ast_line_info, ERROR_ILLEGAL_SOURCE_OPERAND_TYPE_GROUP_A);
                return -1;
            } else {
                new_operand_source = IMMEDIATE_ADDRESS;
            }
        }
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.source_type == SYMBOL_OPERAND_TYPE)
            new_operand_source = DIRECT_ADDRESS;
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_a.source_type == REGISTER_OPERAND_TYPE) {
            if (ast_line_info->ast_word.instruction_word.instruction_name == LEA_TYPE) {
                HANDLE_AST_ERROR_NON_POINTER(ast_line_info, ERROR_ILLEGAL_SOURCE_OPERAND_TYPE_GROUP_A);
                return -1;
            } else {
                new_operand_source = REGISTER_ADDRESS;
            }
        }
        if (new_operand_target == REGISTER_ADDRESS && new_operand_source == REGISTER_ADDRESS) {
            new_node = create_code_node_registers(line, DEFAULT_WORD_LENGTH, *ast_line_info, new_operand_code,
                                                  new_operand_target,
                                                  new_operand_source); /* because of two registers */
            L = DEFAULT_WORD_LENGTH;
        } else {
            new_node = create_code_node(line, GROUP_A_WORD_LENGTH, *ast_line_info, new_operand_code, new_operand_target,
                                        new_operand_source); /* group a handles 3 words */
            L = GROUP_A_WORD_LENGTH;
        }
    }
    if (check_group(ast_line_info->ast_word.instruction_word.instruction_name) == GROUP_B) {
        new_operand_code = ast_line_info->ast_word.instruction_word.instruction_name;
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_b.target_type == NUMBER_OPERAND_TYPE) {
            if (ast_line_info->ast_word.instruction_word.instruction_name == PRN_TYPE) {
                new_operand_target = IMMEDIATE_ADDRESS;
            } else {
                HANDLE_AST_ERROR_NON_POINTER(ast_line_info, ERROR_ILLEGAL_TARGET_OPERAND_TYPE_GROUP_B);
                return -1;
            }
        }
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_b.target_type == SYMBOL_OPERAND_TYPE)
            new_operand_target = DIRECT_ADDRESS;
        if (ast_line_info->ast_word.instruction_word.instruction_union.group_b.target_type == REGISTER_OPERAND_TYPE)
            new_operand_target = REGISTER_ADDRESS;

        new_node = create_code_node(line, DEFAULT_WORD_LENGTH, *ast_line_info, new_operand_code, new_operand_target,
                                    NO_OPERAND);
        L = DEFAULT_WORD_LENGTH;
    }
    if (check_group(ast_line_info->ast_word.instruction_word.instruction_name) == GROUP_C) {
        new_operand_code = ast_line_info->ast_word.instruction_word.instruction_name;
        new_node = create_code_node(line, GROUP_C_WORD_LENGTH, *ast_line_info, new_operand_code, NO_OPERAND, NO_OPERAND);
        L = GROUP_C_WORD_LENGTH;
    }
    add_code_node(my_code_image, new_node);
    return L;
}

/**
    Updates the decimal addresses of symbols in the symbol table associated with data.

    @param my_symbol_table The symbol table to update.
    @param ic A pointer to the instruction counter value.
*/
void update_data_dc(symbol_table *my_symbol_table, int *ic) {
    symbol_node *symbol_node_pointer;
    if (my_symbol_table->first == NULL) {
        /* Symbol table is empty, nothing to update */
        return;
    }

    symbol_node_pointer = my_symbol_table->first;
    do {
        if (symbol_node_pointer->symbol_type == DATA)
            symbol_node_pointer->decimal_address += *ic;

        symbol_node_pointer = symbol_node_pointer->next_symbol;
    } while (symbol_node_pointer != NULL);
}

bool first_pass_process(char *filename_with_am_suffix, int *ic, int *dc, data_image *my_data_image,
                        code_image *my_code_image, symbol_table *symbol_table) {
    FILE *am_file;
    char line[MAX_LINE_LENGTH + 2];
    bool symbol_flag = FALSE;
    bool error_flag = FALSE;
    int L;
    int line_number = 0;
    size_t line_length;
    int c;
    /* Open .am file */
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_READING_AM_FILE);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH + 2, am_file)) {
        line_number++;

        line_length = strlen(line);
        /* Remove trailing newline character if present */
        if (line_length > 0 && line[line_length - 1] == '\n') {
            line[line_length - 1] = '\0';
            line_length--;
        }

        if (line_length > MAX_LINE_LENGTH) {
            printf("Error in line %d: Line is longer than %d characters.\n", line_number, MAX_LINE_LENGTH);
            error_flag = 1;

            /* Skip the remaining characters in the overlong line */
            while ((c = fgetc(am_file)) != '\n' && c != EOF) {
            }
        } else {
            ast ast_line_info = get_ast_line_info(line, line_number);
            if (ast_line_info.ast_word_type != ERROR) {
                if (ast_line_info.ast_symbol[0] != '\0') {
                    symbol_flag = TRUE;
                }
                if (ast_line_info.ast_word_type == DIRECTIVE) {
                    if (ast_line_info.ast_word.directive_word.directive_type == DATA_TYPE ||
                        ast_line_info.ast_word.directive_word.directive_type == STRING_TYPE) {
                        if (symbol_flag) {
                            if(add_symbol(symbol_table, ast_line_info.ast_symbol, dc, DATA)==FALSE){
                                HANDLE_AST_ERROR_NON_POINTER(&ast_line_info, ERROR_SYMBOL_ALREADY_EXISTS);
                                error_flag = TRUE;
                            }
                            else{
                                decode_data(line, ast_line_info, dc, my_data_image);
                            }
                        }
                        else{
                            decode_data(line, ast_line_info, dc, my_data_image);
                        }
                    } else if (ast_line_info.ast_word.directive_word.directive_type == EXTERN_TYPE ||
                               ast_line_info.ast_word.directive_word.directive_type == ENTRY_TYPE) {
                        if (ast_line_info.ast_word.directive_word.directive_type == EXTERN_TYPE) {
                            if(add_symbol(symbol_table, ast_line_info.ast_word.directive_word.directive_option.symbol, dc,
                                       EXTERNAL)==FALSE){
                                HANDLE_AST_ERROR_NON_POINTER(&ast_line_info, ERROR_SYMBOL_ALREADY_EXISTS);
                                error_flag = TRUE;
                            }
                        }
                    }
                } else {
                    if (symbol_flag) {
                        if (add_symbol(symbol_table, ast_line_info.ast_symbol, ic, CODE)==FALSE) {
                            HANDLE_AST_ERROR_NON_POINTER(&ast_line_info, ERROR_SYMBOL_ALREADY_EXISTS);
                            error_flag = TRUE;
                        }
                        else{
                            L = analyze_operands(line, &ast_line_info, my_code_image);
                            if(L==-1)
                                error_flag=TRUE;
                            *ic += L;
                        }
                    }
                    else{
                        L = analyze_operands(line, &ast_line_info, my_code_image);
                        if(L==-1)
                            error_flag=TRUE;
                        *ic += L;
                    }
                }
                symbol_flag = FALSE;
            } else {
                error_flag = TRUE;
            }

        }
    }
    update_data_dc(symbol_table, ic);
    if (error_flag) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_IN_FIRST_PASS);
        fclose(am_file);
        return FALSE;
    } else {
        PRINT_MESSAGE(INFO_MSG_TYPE, INFO_FIRST_PASS);
        fclose(am_file);
        return TRUE;
    }
}
