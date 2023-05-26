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
} InstructionType;

typedef enum{
    REGISTER_TYPE,
    LABEL_TYPE,
    NUMBER_TYPE
} OperandType;

/** src  opcode  des   ARE **/
/** XXX   XXXX   XXX   XX **/
typedef struct{
    InstructionType kind;

    char* source_operand;

    OperandType srctype;

    char* target_operand;

    OperandType trgtype;
} GroupA;

/** src  opcode  des   ARE **/
/** 000   XXXX   XXX   XX **/
typedef struct{
    InstructionType kind;

    char* source_operand;

    OperandType srctype;

} GroupB;

/** src  opcode  des   ARE **/
/** 000   XXXX   000   XX **/
typedef struct{
    InstructionType kind;
} GroupC;

typedef struct{
    //condition: has : in the end of label
    char* label; //optional

    union Instruction_union {
        GroupA groupA;
        GroupB groupB;
        GroupC groupC;
    };

} Instruction;

typedef enum {
    DATA_TYPE,
    STRING_TYPE,
    ENTRY_TYPE,
    EXTERN_TYPE
} DirectiveType;

typedef struct{
    char* label;

    DirectiveType kind;

    /** 000000000110 -> 6 **/
    /** 111111110111 -> -9 (Two's complement, because negative) **/
    /** 000000001111 -> 15 **/
    char* parameters;

} Directive;

typedef struct {
    char error[80];
}Error;

typedef enum {
    INSTRUCTION,
    DIRECTIVE,
    ERROR
} WordType;

typedef struct AST{
    WordType type;
    union word{
        Instruction instruction_word;
        Directive directive_word;
    };
    Error error;
}AST;