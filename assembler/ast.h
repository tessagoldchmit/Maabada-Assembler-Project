#ifndef AST_H
#define AST_H

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
    DEC_TYPE,
    JMP_TYPE,
    BNE_TYPE,
    RED_TYPE,
    PRN_TYPE,
    JSR_TYPE,
    /* Group C */
    RTS_TYPE,
    STOP_TYPE
} instruction_type;

typedef enum{
    REGISTER_TYPE,
    LABEL_TYPE,
    NUMBER_TYPE
} operand_type;

/** src  opcode  des   ARE **/
/** XXX   XXXX   XXX   XX **/
typedef struct{
    instruction_type kind;

    char* source_operand;

    operand_type srctype;

    char* target_operand;

    operand_type trgtype;
} group_a;

/** src  opcode  des   ARE **/
/** 000   XXXX   XXX   XX **/
typedef struct{
    instruction_type kind;

    char* source_operand;

    operand_type srctype;

} group_b;

/** src  opcode  des   ARE **/
/** 000   XXXX   000   XX **/
typedef struct{
    instruction_type kind;
} group_c;

typedef struct {
    //condition: has : in the end of label
    char* label; //optional

    union instruction_union {
        group_a group_a;
        group_b group_b;
        group_c group_c;
    }instruction_union;

} instruction;

typedef enum {
    DATA_TYPE,
    STRING_TYPE,
    ENTRY_TYPE,
    EXTERN_TYPE
} directive_type;

typedef struct{
    char* label;

    directive_type kind;

    /** 000000000110 -> 6 **/
    /** 111111110111 -> -9 (Two's complement, because negative) **/
    /** 000000001111 -> 15 **/
    int machine_code[];

} directive;

typedef struct {
    char error[80];
}error;

typedef enum {
    INSTRUCTION,
    DIRECTIVE,
    ERROR
} word_type;

typedef union word{
    instruction instruction_word;
    directive directive_word;
}word;

typedef struct ast{
    /* TODO relocation to L and merge between data_node and ast.*/
    word_type type;
    word word;
    error error;
}ast;

ast* initialize_ast(word_type word_type, union word);
directive* initialize_directive(char* label, directive_type kind, char* parameters, int binary_length);
group_a initialize_group_a_instruction(instruction_type kind, const char* source_operand, operand_type srctype, const char* target_operand, operand_type trgtype);

#endif // AST_H
