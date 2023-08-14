#ifndef AST_H
#define AST_H

#include "globals.h"

/* Enumeration of supported assembly instructions */
typedef enum {
    MOV_TYPE,
    CMP_TYPE,
    ADD_TYPE,
    SUB_TYPE,
    NOT_TYPE,
    CLR_TYPE,
    LEA_TYPE,
    INC_TYPE,
    DEC_TYPE,
    JMP_TYPE,
    BNE_TYPE,
    RED_TYPE,
    PRN_TYPE,
    JSR_TYPE,
    RTS_TYPE,
    STOP_TYPE
} instruction_name;

/**
 * Represents a mapping of instruction names to their corresponding types.
 */
struct instruction_map {
    const char *name; /* The name of the instruction. */
    instruction_name type; /* The type of the instruction (enum value). */
};

/**
 * An array containing mappings of instruction names to their types.
 */
static const struct instruction_map instructions_map[] = {
        {"mov",  MOV_TYPE},
        {"cmp",  CMP_TYPE},
        {"add",  ADD_TYPE},
        {"sub",  SUB_TYPE},
        {"lea",  LEA_TYPE},
        {"not",  NOT_TYPE},
        {"clr",  CLR_TYPE},
        {"inc",  INC_TYPE},
        {"dec",  DEC_TYPE},
        {"jmp",  JMP_TYPE},
        {"bne",  BNE_TYPE},
        {"red",  RED_TYPE},
        {"prn",  PRN_TYPE},
        {"jsr",  JSR_TYPE},
        {"rts",  RTS_TYPE},
        {"stop", STOP_TYPE}
};

/* Enumeration of different instruction group types */
typedef enum {
    GROUP_A = 1,
    GROUP_B = 2,
    GROUP_C = 3
} group_type;

/* Enumeration of operand types */
typedef enum {
    ERROR_OPERAND_TYPE,
    REGISTER_OPERAND_TYPE,
    SYMBOL_OPERAND_TYPE,
    NUMBER_OPERAND_TYPE
} operand_type;

/* Union for storing different types of operands */
typedef union {
    char symbol[MAX_SYMBOL_LENGTH + 1];
    char register_num;
    int number;
} operand_union;

/* Structure for group A instructions */
typedef struct {
    operand_type source_type;
    operand_union source_value;
    operand_type target_type;
    operand_union target_value;
} group_a;

/* Structure for group B instructions */
typedef struct {
    operand_type target_type;
    operand_union target_value;
} group_b;

/* Structure for representing an assembly instruction */
typedef struct {
    char *symbol; /* optional */
    instruction_name instruction_name;
    union instruction_union {
        group_a group_a;
        group_b group_b;
    } instruction_union;
} instruction;

/* Enumeration of directive types */
typedef enum {
    DATA_TYPE,
    STRING_TYPE,
    ENTRY_TYPE,
    EXTERN_TYPE
} directive_type;

/* Structure for representing assembly directives */
typedef struct {
    directive_type directive_type;
    union {
        char string[MAX_STRING_LENGTH];  /* for string type */
        char symbol[MAX_SYMBOL_LENGTH];  /* for extern & entry types */
        struct {
            int machine_code_array[MAX_NUMBER_LENGTH];
            int machine_code_count;
        } machine_code;  /* for data type */
    } directive_option;
} directive;

/* Typedef for error messages */
typedef char error[MAX_AST_ERROR_LENGTH + 1];

/* Typedef for symbols */
typedef char symbol[MAX_SYMBOL_LENGTH + 1];

/* Enumeration of word types */
typedef enum {
    INSTRUCTION,
    DIRECTIVE,
    ERROR
} word_type;

/* Union for storing different types of words in the AST */
typedef union {
    instruction instruction_word;
    directive directive_word;
    error error_word;
} word_union;

/* Structure for representing nodes in the Abstract Syntax Tree */
typedef struct ast {
    symbol ast_symbol;
    word_type ast_word_type;
    word_union ast_word;
    int line_number;
} ast;

/**
 * Parses an input line and populates an Abstract Syntax Tree (AST) structure.
 * @param line The input line to be parsed.
 * @param line_number The line number of the input line.
 * @return The populated AST structure.
 */
ast get_ast_line_info(char *line, int line_number);

/**
 * Checks the group type of an instruction based on its instruction name.
 * @param instruction_name The name of the instruction.
 * @return The group type of the instruction.
 */
group_type check_group(instruction_name instruction_name);

#endif /* AST_H */
