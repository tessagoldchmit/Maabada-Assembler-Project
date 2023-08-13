#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"
#include "symbols.h"
#include "logs.h"

/**
 * Extracts the directive from the input line and updates the AST accordingly.
 * @param line_ptr Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 * @return Updated pointer to the input line.
 */
char *get_directive(char *line_ptr, ast *ast) {
    /* Skip '.' */
    (line_ptr)++;
    char *directive;
    directive = line_ptr;

    if (strncmp(directive, "string", 6) == 0) {
        ast->ast_word_type = DIRECTIVE;
        ast->ast_word.directive_word.directive_type = STRING_TYPE;
        line_ptr += 6;
    } else if (strncmp(directive, "data", 4) == 0) {
        ast->ast_word_type = DIRECTIVE;
        ast->ast_word.directive_word.directive_type = DATA_TYPE;
        line_ptr += 4;
    } else if (strncmp(directive, "entry", 5) == 0) {
        ast->ast_word_type = DIRECTIVE;
        ast->ast_word.directive_word.directive_type = ENTRY_TYPE;
        line_ptr += 5;
    } else if (strncmp(directive, "extern", 6) == 0) {
        ast->ast_word_type = DIRECTIVE;
        ast->ast_word.directive_word.directive_type = EXTERN_TYPE;
        line_ptr += 6;
    } else {
        HANDLE_AST_ERROR(&ast, ERROR_INVALID_INSTRUCTION);
    }
    return line_ptr;
}

/**
 * Extracts a string from the input line and updates the AST accordingly.
 * @param ptr Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
static void get_string(char *ptr, ast *ast) {
    char *end_quote;
    ptr = skip_spaces(ptr);
    if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_STRING_MISSING);
        return;
    }
    if (ptr[0] != '"') {
        HANDLE_AST_ERROR(&ast, ERROR_STRING_START_QUOTES);
        return;
    }
    ptr += 1; /* Skip " */

    /* Check for closing quotes */
    end_quote = strchr(ptr, '"');
    if (end_quote == NULL) {
        HANDLE_AST_ERROR(&ast, ERROR_STRING_END_QUOTES);
        return;
    } else {
        size_t len = end_quote - ptr;
        strncpy(ast->ast_word.directive_word.directive_option.string, ptr, len);
        ast->ast_word.directive_word.directive_option.string[len] = '\0';
        ptr += len + 1; /* Skip the '"' */
    }
    ptr = skip_spaces(ptr);
    if (ptr[0] != '\0' && ptr[0] != '\n' && ptr[0] != EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_TRAILING_CHARACTERS);
        return;
    }
}

/**
 * Extracts data integers from the input line and updates the AST accordingly.
 * @param ptr Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
static void get_data(char *ptr, ast *ast) {
    char *end_ptr;
    int value;
    int num_of_ints = 0;

    do {
        ptr = skip_spaces(ptr);
        if (ptr[0] == ',') {
            HANDLE_AST_ERROR(&ast, ERROR_LEADING_COMMA);
            return;
        } else if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
            HANDLE_AST_ERROR(&ast, ERROR_DATA_MISSING_INT);
            return;
        }
        value = strtol(ptr, &end_ptr, 10);
        if (end_ptr == ptr) {
            HANDLE_AST_ERROR(&ast, ERROR_INVALID_NUMBER);
            return;
        }
        ast->ast_word.directive_word.directive_option.machine_code.machine_code_array[num_of_ints] = value;
        ast->ast_word.directive_word.directive_option.machine_code.machine_code_count++;
        num_of_ints++;
        ptr = end_ptr;
        ptr = skip_spaces(ptr);
        if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
            return;
        } else {
            if (ptr[0] != ',') {
                HANDLE_AST_ERROR(&ast, ERROR_ARGS_FOR_DATA_MUST_BE_INTEGERS);
                return;
            }
            ptr += 1; /* Skip the ',' */
            ptr = skip_spaces(ptr);
            if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
                HANDLE_AST_ERROR(&ast, ERROR_TRAILING_COMMA);
                return;
            } else if (ptr[0] == ',') {
                HANDLE_AST_ERROR(&ast, ERROR_CONSECUTIVE_COMMAS);
                return;
            }
        }

    } while (*end_ptr != '\n' && *end_ptr != '\0' && *end_ptr != EOF);

}

/**
 * Extracts entry/extern symbol from the input line and updates the AST accordingly.
 * @param ptr Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
static void get_ent_extern_symbol(char *ptr, ast *ast) {
    char *symbol_name = NULL;
    int len;
    ptr = skip_spaces(ptr);
    if (ast->ast_symbol[0] != '\0') {
        ast->ast_symbol[0] = '\0';
    }
    if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_ENTRY_EXTERN_WITHOUT_SYMBOL);
        return;
    }
    for (len = 0; ptr[len] != '\0' && ptr[len] != '\n' &&
                  ptr[len] != EOF && !(isspace(ptr[len])); len++);
    symbol_name = (char *) malloc(sizeof(char) * (len + 1));
    if (symbol_name == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(symbol_name);
        exit(1);
    }
    strncpy(symbol_name, ptr, len);
    symbol_name[len] = '\0';
    ptr += len;

    if (!is_symbol_valid(symbol_name)) {
        HANDLE_AST_ERROR(&ast, ERROR_INVALID_SYMBOL);
        free(symbol_name);
        return;
    }
    strcpy(ast->ast_word.directive_word.directive_option.symbol, symbol_name);
    free(symbol_name);
    ptr = skip_spaces(ptr);
    if (ptr[0] != '\0' && ptr[0] != '\n' && ptr[0] != EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_TRAILING_CHARACTERS);
        free(symbol_name);
        return;
    }
}

group_type check_group(instruction_name instruction_name) {
    if (instruction_name == MOV_TYPE || instruction_name == CMP_TYPE || instruction_name == ADD_TYPE ||
        instruction_name == SUB_TYPE || instruction_name == LEA_TYPE) {
        return GROUP_A;
    } else if (instruction_name == NOT_TYPE || instruction_name == CLR_TYPE || instruction_name == INC_TYPE ||
               instruction_name == DEC_TYPE || instruction_name == JMP_TYPE || instruction_name == BNE_TYPE ||
               instruction_name == RED_TYPE || instruction_name == PRN_TYPE || instruction_name == JSR_TYPE) {
        return GROUP_B;
    } else if (instruction_name == RTS_TYPE || instruction_name == STOP_TYPE) {
        return GROUP_C;
    }
    return 0;
}

/**
 * Extracts the code instruction from the input line and updates the AST accordingly.
 * @param line Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 * @return Updated pointer to the input line.
 */
char *get_code_instruction(char *line, ast *ast) {
    int len;
    char *line_ptr = line;
    char *command = NULL;
    int i;
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])); len++);
    command = malloc(sizeof(char) * len + 1);
    if (command == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(command);
        exit(1);
    }
    strncpy(command, line_ptr, len);
    command[len] = '\0';
    line_ptr += len;

    ast->ast_word_type = INSTRUCTION;

    for (i = 0; i < 16; ++i) {
        if (strncmp(command, instructions_map[i].name, strlen(command)) == 0) {
            ast->ast_word.instruction_word.instruction_name = instructions_map[i].type;
            free(command);
            return line_ptr;
        }
    }
    HANDLE_AST_ERROR(&ast, ERROR_COMMAND_DOES_NOT_EXIST);
    free(command);
    return line_ptr;
}

/**
 * Checks the operand type of an operand and provides an error message if needed.
 * @param operand Operand string to be checked.
 * @param error_msg Pointer to the error message to be populated if needed.
 * @return The operand type.
 */
operand_type check_operand_type(char *operand, error *error_msg) {
    char *endptr;
    long value;
    if (strlen(operand) == 0) {
        strcpy(*error_msg, ERROR_MISSING_OPERAND);
        return ERROR_OPERAND_TYPE;
    }
    /* check if operand is a number */
    if (is_operand_a_number(operand)) {
        value = strtol(operand, &endptr, 10);
        if (value >= -(1 << 12) && value < (1 << 12)) {
            /* Valid immediate operand */
            return NUMBER_OPERAND_TYPE;
        } else {
            /* Error: value out of range */
            strcpy(*error_msg, ERROR_INTEGER_OUT_OF_RANGE);
            return ERROR_OPERAND_TYPE;
        }

    } else if (is_operand_a_register(operand)) {
        /* Register */
        int register_num = operand[2] - '0';
        if (register_num >= FIRST_REGISTER_NUM && register_num <= LAST_REGISTER_NUM) {
            return REGISTER_OPERAND_TYPE;
        } else {
            strcpy(*error_msg, ERROR_INVALID_REGISTER);
            return ERROR_OPERAND_TYPE;
        }
    } else if (is_symbol_valid(operand)) {
        /* Might be a defined symbol or not, we'll save it for now as a symbol */
        return SYMBOL_OPERAND_TYPE;
    } else {
        strcpy(*error_msg, ERROR_ILLEGAL_OPERAND);
        return ERROR_OPERAND_TYPE;
    }
}

/**
 * Checks operands for group A instructions and updates the AST accordingly.
 * @param line Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
void check_operands_for_group_a(char *line, ast *ast) {
    char *line_ptr = line;
    char *endptr;
    int len;
    char *operand;

    error *error_msg = NULL;
    error_msg = (error *) malloc(sizeof(error));
    if (error_msg == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(error_msg);
        exit(1);
    }

    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] == ',') {
        HANDLE_AST_ERROR(&ast, ERROR_LEADING_COMMA);
        free(error_msg);
        return;
    }
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);
    /* Store the first operand */
    operand = malloc(sizeof(char) * len + 1);
    if (operand == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(operand);
        exit(1);
    }
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    ast->ast_word.instruction_word.instruction_union.group_a.source_type = check_operand_type(operand, error_msg);
    /* Check that the first operand is valid */
    if (ast->ast_word_type == ERROR) {
        HANDLE_AST_ERROR(&ast, *error_msg);
        free(error_msg);
        free(operand);
        return;
    } else {
        if (ast->ast_word.instruction_word.instruction_union.group_a.source_type == NUMBER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_a.source_value.number = strtol(operand, &endptr, 10);
        } else if (ast->ast_word.instruction_word.instruction_union.group_a.source_type == SYMBOL_OPERAND_TYPE) {
            strcpy(ast->ast_word.instruction_word.instruction_union.group_a.source_value.symbol, operand);
        } else if (ast->ast_word.instruction_word.instruction_union.group_a.source_type == REGISTER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_a.source_value.register_num = operand[2];
        }
    }
    free(operand);
    line_ptr += len;
    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] != ',') {
        free(error_msg);
        HANDLE_AST_ERROR(&ast, ERROR_MISSING_COMMA);
        return;
    }
    line_ptr += 1; /* skip comma */
    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] == ',') {
        free(error_msg);
        HANDLE_AST_ERROR(&ast, ERROR_CONSECUTIVE_COMMAS);
        return;
    }
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);
    /* Store the second operand */
    operand = malloc(sizeof(char) * len + 1);
    if (operand == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(operand);
        exit(1);
    }
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    ast->ast_word.instruction_word.instruction_union.group_a.target_type = check_operand_type(operand, error_msg);

    /* Check that the second operand is valid */
    if (ast->ast_word_type == ERROR) {
        HANDLE_AST_ERROR(&ast, *error_msg);
        free(error_msg);
        free(operand);
        return;
    } else {
        if (ast->ast_word.instruction_word.instruction_union.group_a.target_type == NUMBER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_a.target_value.number = strtol(operand, &endptr, 10);
        } else if (ast->ast_word.instruction_word.instruction_union.group_a.target_type == SYMBOL_OPERAND_TYPE) {
            strcpy(ast->ast_word.instruction_word.instruction_union.group_a.target_value.symbol, operand);
        } else if (ast->ast_word.instruction_word.instruction_union.group_a.target_type == REGISTER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_a.target_value.register_num = operand[2];
        }
    }
    free(operand);
    line_ptr += len;
    /* Check for extra trailing characters at the end */
    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] != '\0' && line_ptr[0] != '\n' && line_ptr[0] != EOF) {
        free(error_msg);
        HANDLE_AST_ERROR(&ast, ERROR_TRAILING_CHARACTERS);
        return;
    }
}

/**
 * Checks operands for group B instructions and updates the AST accordingly.
 * @param line Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
void check_operands_for_group_b(char *line, ast *ast) {
    char *line_ptr = line;
    char *endptr;
    int len;
    char *operand;
    error *error_msg = NULL;
    error_msg = (error *) malloc(sizeof(char));
    if (error_msg == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(error_msg);
        exit(1);
    }

    line_ptr = skip_spaces(line_ptr);

    if (line_ptr[0] == ',') {
        free(error_msg);
        HANDLE_AST_ERROR(&ast, ERROR_LEADING_COMMA);
        return;
    }

    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);

    /* Store the operand */
    operand = malloc(sizeof(char) * len + 1);
    if (operand == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(operand);
        exit(1);
    }
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    line_ptr += len;
    line_ptr = skip_spaces(line_ptr);

    if (line_ptr[0] != '\0' && line_ptr[0] != '\n' && line_ptr[0] != EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_TRAILING_CHARACTERS);
        free(operand);
        return;
    }
    ast->ast_word.instruction_word.instruction_union.group_b.target_type = check_operand_type(operand, error_msg);

    if (ast->ast_word_type == ERROR) {
        HANDLE_AST_ERROR(&ast, *error_msg);
        free(error_msg);
        free(operand);
        return;
    } else {
        if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == NUMBER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_b.target_value.number = strtol(operand, &endptr, 10);
        } else if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == SYMBOL_OPERAND_TYPE) {
            strcpy(ast->ast_word.instruction_word.instruction_union.group_b.target_value.symbol, operand);
        } else if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == REGISTER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_b.target_value.register_num = operand[2];
        }
        free(operand);
        free(error_msg);
        return;
    }
}

/**
 * Checks operands for group C instructions and updates the AST accordingly.
 * @param line Pointer to the input line.
 * @param ast Pointer to the AST structure to be updated.
 */
void check_operands_for_group_c(char *line, ast *ast) {
    char *line_ptr = line;

    line_ptr = skip_spaces(line_ptr);
    if (*line_ptr != '\0' && *line_ptr != '\n' && *line_ptr != EOF) {
        HANDLE_AST_ERROR(&ast, ERROR_TRAILING_CHARACTERS);
        return;
    }
}

ast get_ast_line_info(char *line, int line_number) {
    ast ast = {0};
    char *line_ptr = line;
    char *symbol_name = NULL;
    ast.line_number = line_number;

    /* is a symbol definition */
    line_ptr = skip_spaces(line_ptr);
    if (is_symbol(line_ptr)) {
        char *colon_ptr = strchr(line, ':');
        size_t symbol_length = colon_ptr - line_ptr;  /* Calculate the length of the string before ':' */
        symbol_name = malloc(sizeof(char) * (symbol_length + 1));
        if (symbol_name == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            free(symbol_name);
            exit(1);
        }
        strncpy(symbol_name, line_ptr, symbol_length);
        symbol_name[symbol_length] = '\0';
        if (is_symbol_valid(symbol_name)) {
            strcpy(ast.ast_symbol, symbol_name);
            free(symbol_name);
            line_ptr += symbol_length + 1; /* Skip the ':' */
        } else {
            HANDLE_AST_ERROR_NON_POINTER(&ast, ERROR_INVALID_SYMBOL);
            free(symbol_name);
            return ast;
        }
    } else {
        ast.ast_symbol[0] = '\0'; /* not a symbol definition */
    }

    /* Check if the line is a directive (.string .data .entry .extern) */
    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] == '.') {
        line_ptr = get_directive(line_ptr, &ast);
        line_ptr = skip_spaces(line_ptr);
        if (ast.ast_word_type == ERROR) {
            return ast;
        } else if (ast.ast_word.directive_word.directive_type == STRING_TYPE) {
            get_string(line_ptr, &ast);
            return ast;
        } else if (ast.ast_word.directive_word.directive_type == DATA_TYPE) {
            get_data(line_ptr, &ast);
        } else if (ast.ast_word.directive_word.directive_type == ENTRY_TYPE ||
                   ast.ast_word.directive_word.directive_type == EXTERN_TYPE) {
            get_ent_extern_symbol(line_ptr, &ast);
            return ast;
        }
    } else {
        line_ptr = get_code_instruction(line_ptr, &ast);
        if (ast.ast_word_type == ERROR) {
            return ast;
        }
        line_ptr = skip_spaces(line_ptr);

        if (check_group(ast.ast_word.instruction_word.instruction_name) == GROUP_A) {
            check_operands_for_group_a(line_ptr, &ast);
            return ast;
        }
        if (check_group(ast.ast_word.instruction_word.instruction_name) == GROUP_B) {
            check_operands_for_group_b(line_ptr, &ast);
            return ast;
        }
        if (check_group(ast.ast_word.instruction_word.instruction_name) == GROUP_C) {
            check_operands_for_group_c(line_ptr, &ast);
            return ast;
        }
    }
    return ast;
}
