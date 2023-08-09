#include <ctype.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include "ast.h"
#include "utils.h"
#include "symbols.h"
#include "errors.h"

int set_binary_value(int value) {
    int binary = 0;
    int bit_position = 0;

    while (bit_position < 12) {
        int bit = value & 1;  /* Extract the least significant bit */
        binary |= (bit << bit_position);  /* Set the bit at the corresponding position */
        value >>= 1;  /* Right-shift the value to process the next bit */
        bit_position++;  /* Move to the next bit position */
    }
    return binary;
}

int ascii_to_binary(char parameter) {
    int param_int_value = (int) parameter;
    return set_binary_value(param_int_value);
}

int *num_to_binary(char *parameters, int *machine_code, int binary_length) {
    char *token;
    int *numbers = malloc(binary_length * sizeof(int));
    int count = 0;
    int i;

    char *parameter_copy = strdup(parameters); /* Create a copy of the original string */
    char *parameter_ptr = skip_spaces(parameter_copy); /* Skip initial spaces */

    while (count < binary_length) {
        /* Find the next comma */
        char *comma_ptr = strchr(parameter_ptr, ',');
        if (comma_ptr != NULL) {
            /* Null-terminate the token */
            *comma_ptr = '\0';
        }

        /* Convert token to integer and store in numbers array */
        numbers[count] = atoi(parameter_ptr);
        count++;

        if (comma_ptr == NULL) {
            /* No more commas found, exit the loop */
            break;
        }

        /* Move the parameter pointer to the next token */
        parameter_ptr = skip_spaces(comma_ptr + 1);
    }

    free(parameter_copy); /* Free the dynamically allocated memory */

    for (i = 0; i < count; i++) {
        machine_code[i] = set_binary_value(numbers[i]);
    }
    free(numbers); /* Free the dynamically allocated memory */
    return machine_code;
}

directive *initialize_directive(char *symbol, directive_type directive_type, char *parameters, int binary_length) {
    directive *new_directive = malloc(sizeof(directive));
    /* Handle memory allocation failure */
    if (new_directive == NULL) {
        return NULL;
    }

    /* Allocate memory and copy symbol */
    strcpy(new_directive->directive_option.symbol, symbol);

    /* Assign directive_type */
    new_directive->directive_type = directive_type;

    /* Create machine code */
    int new_machine_code[binary_length];
    int *array_ptr;
    int i;
    if (directive_type == STRING_TYPE) {
        for (i = 0; i < binary_length - 1; i++) {
            new_machine_code[i] = ascii_to_binary(parameters[i]);
        }
        new_machine_code[binary_length - 1] = ascii_to_binary('\0');
    } else if (directive_type == DATA_TYPE) {
        array_ptr = num_to_binary(parameters, new_machine_code, binary_length);
        memcpy(new_directive->directive_option.machine_code.machine_code_array, array_ptr, binary_length * sizeof(int));
    }

    if (new_directive->directive_option.machine_code.machine_code_array == NULL) {
        /* Handle memory allocation failure */
        free(new_directive->directive_option.symbol);
        free(new_directive);
        return NULL;
    }

    return new_directive;
}

char *get_directive(char *line_ptr, ast *ast) {
    /* Skip '.' */
    (line_ptr)++;
    char *directive = line_ptr;

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
        HANDLE_ERROR(&ast, ERROR_INVALID_INSTRUCTION)
    }
    return line_ptr;
}


static void get_string(char *ptr, ast *ast) {
    char *end_quote;
    ptr = skip_spaces(ptr);
    if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
        HANDLE_ERROR(&ast, ERROR_STRING_MISSING)
        return;
    }
    if (ptr[0] != '"') {
        HANDLE_ERROR(&ast, ERROR_STRING_START_QUOTES)
        return;
    }
    ptr += 1; /* Skip " */

    /* Check for closing quotes */
    end_quote = strchr(ptr, '"');
    if (end_quote == NULL) {
        HANDLE_ERROR(&ast, ERROR_STRING_END_QUOTES)
        return;
    } else {
        size_t len = end_quote - ptr;
        strncpy(ast->ast_word.directive_word.directive_option.string, ptr, len);
        ast->ast_word.directive_word.directive_option.string[len] = '\0';
        ptr += len + 1; /* Skip the '"' */
    }
    ptr = skip_spaces(ptr);
    if (ptr[0] != '\0' && ptr[0] != '\n' && ptr[0] != EOF) {
        HANDLE_ERROR(&ast, ERROR_TRAILING_CHARACTERS)
        return;
    }
}

static void get_data(char *ptr, ast *ast) {
    char *end_ptr;
    int value;
    int num_of_ints = 0;

    do {
        ptr = skip_spaces(ptr);
        if (ptr[0] == ',') {
            HANDLE_ERROR(&ast, ERROR_LEADING_COMMA)
            return;
        } else if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
            HANDLE_ERROR(&ast, ERROR_DATA_MISSING_INT)
            return;
        }
        value = strtol(ptr, &end_ptr, 10);
        if (end_ptr == ptr) {
            HANDLE_ERROR(&ast, ERROR_INVALID_NUMBER)
            return;
        }
        ast->ast_word.directive_word.directive_option.machine_code.machine_code_array[num_of_ints] = value;
        ast->ast_word.directive_word.directive_option.machine_code.machine_code_count++;
        num_of_ints++;
        ptr += end_ptr - ptr;  /* TODO tessag update */
        ptr = skip_spaces(ptr);
        if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
            return;
        } else {
            if (ptr[0] != ',') {
                HANDLE_ERROR(&ast, ERROR_MISSING_COMMA)
                return;
            }
            ptr += 1; /* Skip the ',' */
            ptr = skip_spaces(ptr);
            if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
                HANDLE_ERROR(&ast, ERROR_TRAILING_COMMA)
                return;
            } else if (ptr[0] == ',') {
                HANDLE_ERROR(&ast, ERROR_CONSECUTIVE_COMMAS)
                return;
            }
        }

    } while (*end_ptr != '\n' && *end_ptr != '\0' && *end_ptr != EOF);

}

static void get_ent_extern_symbol(char *ptr, ast *ast) {
    char *symbol_name = NULL;
    int len = 0;
    ptr = skip_spaces(ptr);
    if (ast->ast_symbol[0] != '\0') {
        ast->ast_symbol[0] = '\0';
    }
    if (ptr[0] == '\0' || ptr[0] == '\n' || ptr[0] == EOF) {
        HANDLE_ERROR(&ast, ERROR_ENTRY_EXTERN_WITHOUT_SYMBOL)
        return;
    }
    for (len = 0; ptr[len] != '\0' && ptr[len] != '\n' &&
                  ptr[len] != EOF && !(isspace(ptr[len])); len++);
    symbol_name = (char *) malloc(sizeof(char) * (len + 1));
    strncpy(symbol_name, ptr, len);
    symbol_name[len] = '\0';
    ptr += len;


    if (!is_symbol_valid(symbol_name)) {
        HANDLE_ERROR(&ast, ERROR_INVALID_SYMBOL)
        free(symbol_name);
        return;
    }
    strcpy(ast->ast_word.directive_word.directive_option.symbol, symbol_name);
    free(symbol_name);
    ptr = skip_spaces(ptr);
    if (ptr[0] != '\0' && ptr[0] != '\n' && ptr[0] != EOF) {
        HANDLE_ERROR(&ast, ERROR_TRAILING_CHARACTERS)
        return;
    }
}

bool is_symbol(char *line) {
    return strchr(line, ':') != NULL;
}

group_type check_group(instruction_name instruction_name) {
    if (instruction_name <= LEA_TYPE) {
        return GROUP_A;
    } else if (instruction_name <= JSR_TYPE) {
        return GROUP_B;
    } else if (instruction_name == RTS_TYPE || instruction_name == STOP_TYPE) {
        return GROUP_C;
    }
    return 0;
}


char *get_code_instruction(char *line, ast *ast) {
    int len;
    char *line_ptr = line;
    char *command = NULL;
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])); len++);
    command = malloc(sizeof(char) * len + 1);
    strncpy(command, line_ptr, len);
    command[len] = '\0';
    line_ptr += len;

    ast->ast_word_type = INSTRUCTION;

    int i;
    for (i = 0; i < 16; ++i) {
        if (strncmp(command, instructions_map[i].name, strlen(command)) == 0) {
            ast->ast_word.instruction_word.instruction_name = instructions_map[i].type;
            return line_ptr;
        }
    }
    HANDLE_ERROR(&ast, ERROR_COMMAND_DOES_NOT_EXIST)
    return line_ptr;
}

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

void check_operands_for_group_a(char *line, ast *ast) {
    char *line_ptr = line;
    char *endptr;
    int len;
    char *operand;

    error *error_msg = NULL;
    error_msg = (error*)malloc(sizeof(char));

    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] == ',') {
        HANDLE_ERROR(&ast, ERROR_LEADING_COMMA)
        return;
    }
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);
    /* Store the first operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    ast->ast_word.instruction_word.instruction_union.group_a.source_type = check_operand_type(operand, error_msg);
    HANDLE_ERROR(&ast, *error_msg)
    /* Check that the first operand is valid */
    if (ast->ast_word_type == ERROR) {
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
        HANDLE_ERROR(&ast, ERROR_MISSING_COMMA)
        return;
    }
    line_ptr += 1; /* skip comma */
    line_ptr = skip_spaces(line_ptr);
    if (line_ptr[0] == ',') {
        HANDLE_ERROR(&ast, ERROR_CONSECUTIVE_COMMAS)
        return;
    }
    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);
    /* Store the second operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    ast->ast_word.instruction_word.instruction_union.group_a.target_type = check_operand_type(operand, error_msg);
    HANDLE_ERROR(&ast, *error_msg)

    /* Check that the second operand is valid */
    if (ast->ast_word_type == ERROR) {
        HANDLE_ERROR(&ast, ERROR_ILLEGAL_OPERAND)
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
        HANDLE_ERROR(&ast, ERROR_TRAILING_CHARACTERS)
        return;
    }
}


void check_operands_for_group_b(char *line, ast *ast) {
    char *line_ptr = line;
    char *endptr;
    int len;
    char *operand;
    error *error_msg = NULL;
    error_msg = (error*)malloc(sizeof(char));

    line_ptr = skip_spaces(line_ptr);

    if (line_ptr[0] == ',') {
        HANDLE_ERROR(&ast, ERROR_LEADING_COMMA)
        return;
    }

    for (len = 0; line_ptr[len] != '\0' && line_ptr[len] != '\n' &&
                  line_ptr[len] != EOF && !(isspace(line_ptr[len])) && line_ptr[len] != ','; len++);

    /* Store the operand */
    operand = malloc(sizeof(char) * len + 1);
    strncpy(operand, line_ptr, len);
    operand[len] = '\0';
    line_ptr += len;
    line_ptr = skip_spaces(line_ptr);

    if (line_ptr[0] != '\0' && line_ptr[0] != '\n' && line_ptr[0] != EOF) {
        HANDLE_ERROR(&ast, ERROR_TRAILING_CHARACTERS)
        free(operand);
        return;
    }
    ast->ast_word.instruction_word.instruction_union.group_b.target_type = check_operand_type(operand, error_msg);
    HANDLE_ERROR(&ast, *error_msg)

    if (ast->ast_word_type == ERROR) {
        free(operand);
        return;
    } else {
        if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == NUMBER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_b.target_value.number =  strtol(operand, &endptr, 10);
        } else if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == SYMBOL_OPERAND_TYPE) {
            strcpy(ast->ast_word.instruction_word.instruction_union.group_b.target_value.symbol, operand);
        } else if (ast->ast_word.instruction_word.instruction_union.group_b.target_type == REGISTER_OPERAND_TYPE) {
            ast->ast_word.instruction_word.instruction_union.group_b.target_value.register_num = operand[1];
        }
        free(operand);
        return;
    }
}

void check_operands_for_group_c(char *line, ast *ast) {
    char *line_ptr = line;

    line_ptr = skip_spaces(line_ptr);
    if (*line_ptr != '\0' && *line_ptr != '\n' && *line_ptr != EOF) {
        HANDLE_ERROR(&ast, ERROR_TRAILING_CHARACTERS)
        return;
    }
}

ast get_ast_line_info(char *line, int line_number) {
    ast ast = {0};
    char *line_ptr = line;
    char *symbol_name = NULL;
    ast.line_number=line_number;

    /* is a symbol definition */
    line_ptr = skip_spaces(line_ptr);
    if (is_symbol(line_ptr)) {
        char *colon_ptr = strchr(line, ':');
        size_t symbol_length = colon_ptr - line_ptr;  /* Calculate the length of the string before ':' */
        symbol_name = malloc(sizeof(char) * (symbol_length + 1));
        strncpy(symbol_name, line, symbol_length);
        symbol_name[symbol_length] = '\0';
        if (is_symbol_valid(symbol_name)) {
            strcpy(ast.ast_symbol, symbol_name);
            free(symbol_name);
            line_ptr += symbol_length + 1; /* Skip the ':' */
        } else {
            ast.ast_word_type = ERROR;
            strcpy(ast.ast_word.error_word, ERROR_INVALID_SYMBOL);
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
        } else if (ast.ast_word.directive_word.directive_type == DATA) {
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
