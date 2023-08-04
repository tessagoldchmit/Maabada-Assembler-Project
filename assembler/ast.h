#ifndef AST_H
#define AST_H

#include "globals.h"

typedef enum {
    /* Group A */
    MOV_TYPE,
    CMP_TYPE,
    ADD_TYPE,
    SUB_TYPE,
    LEA_TYPE,
    /* Group B */
    NOT_TYPE,
    CLR_TYPE,
    INC_TYPE,
    DEC_TYPE,
    JMP_TYPE,
    BNE_TYPE,
    RED_TYPE,
    PRN_TYPE,
    JSR_TYPE,
    /* Group C */
    RTS_TYPE,
    STOP_TYPE,
} instruction_name;

struct instruction_map {
    const char *name;
    instruction_name type;
};

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

typedef enum {
    GROUP_A = 1,
    GROUP_B = 2,
    GROUP_C = 3
} group_type;

typedef enum {
    ERROR_OPERAND_TYPE,
    REGISTER_OPERAND_TYPE,
    SYMBOL_OPERAND_TYPE,
    NUMBER_OPERAND_TYPE
} operand_type;

typedef union {
    char symbol[MAX_SYMBOL_LENGTH+1];
    char register_num;
    int number;
} operand_union;

/** src  opcode  trg   ARE **/
/** XXX   XXXX   XXX   XX **/
typedef struct {
    operand_type source_type;
    operand_union source_value;
    operand_type target_type;
    operand_union target_value;
} group_a;

/** src  opcode  trg   ARE **/
/** 000   XXXX   XXX   XX **/
typedef struct {
    operand_type target_type;
    operand_union target_value;
} group_b;


typedef struct {
    char *symbol; /* optional */
    instruction_name instruction_name;
    union instruction_union {
        group_a group_a;
        group_b group_b;
    } instruction_union;
} instruction;

typedef enum {
    DATA_TYPE,
    STRING_TYPE,
    ENTRY_TYPE,
    EXTERN_TYPE
} directive_type;

typedef struct {
    directive_type directive_type;
    union {
        char string[MAX_STRING_LENGTH];  /* for string type */
        char symbol[MAX_SYMBOL_LENGTH];  /* for extern / entry type */
        /** 000000000110 -> 6 **/
        /** 111111110111 -> -9 (Two's complement, because negative) **/
        /** 000000001111 -> 15 **/
        struct {
            int machine_code_array[MAX_NUMBER_LENGTH];
            int machine_code_count;
        } machine_code;  /* for data type */
    } directive_option;
} directive;

typedef char error[MAX_AST_ERROR_LENGTH + 1];
typedef char symbol[MAX_SYMBOL_LENGTH + 1];


typedef enum {
    INSTRUCTION,
    DIRECTIVE,
    ERROR
} word_type;

typedef union {
    instruction instruction_word;
    directive directive_word;
    error error_word;
} word_union;

typedef struct ast {
    symbol ast_symbol;
    word_type ast_word_type;
    word_union ast_word;
} ast;


ast get_ast_line_info(char *line);

group_type check_group(instruction_name instruction_name);

#endif /* AST_H */
