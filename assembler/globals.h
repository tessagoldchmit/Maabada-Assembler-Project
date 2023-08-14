#ifndef GLOBALS_H
#define GLOBALS_H


/* Maximum length of a file name */
#define MAX_FILE_NAME 80

/* Total memory size in words */
#define MEMORY_SIZE 1024

/* Maximum length of a line in the input file */
#define MAX_LINE_LENGTH 80

/* Maximum length of a symbol */
#define MAX_SYMBOL_LENGTH 31

/* Starting memory address available for use */
#define START_OF_MEMORY_ADDRESS 100

/* Maximum length of a string */
#define MAX_STRING_LENGTH 80

/* Maximum length of a number */
#define MAX_NUMBER_LENGTH 80

/* Maximum length of an AST error message */
#define MAX_AST_ERROR_LENGTH 80

/* First and last register numbers */
#define FIRST_REGISTER_NUM 0
#define LAST_REGISTER_NUM 7

/* Operand addressing methods */
#define NO_OPERAND 0
#define GROUP_A_WORD_LENGTH 3
#define DEFAULT_WORD_LENGTH 2
#define GROUP_C_WORD_LENGTH 1
#define IMMEDIATE_ADDRESS 1
#define DIRECT_ADDRESS 3
#define REGISTER_ADDRESS 5
#define EXTERN_DECIMAL_ADDRESS (-1)
#define NON_EXIST_SYMBOL_ADDRESS (-2)

static const char *reserved_words[] = {
        "@r0", "@r1", "@r2", "@r3", "@r4", "@r5", "@r6", "@r7",
        "mov", "cmp", "add", "sub", "lea", "not", "clr", "inc",
        "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

/* Boolean values */
typedef enum {
    FALSE,
    TRUE
} bool;


#endif /* GLOBALS_H */
