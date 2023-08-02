#ifndef GLOBALS_H
#define GLOBALS_H

/* Maximum file name length */
#define MAX_FILE_NAME 80

/* Total memory size */
#define MEMORY_SIZE 1024

/* Max line length */
#define MAX_LINE_LENGTH 80

/* Max symbol length*/
#define MAX_SYMBOL_LENGTH 31

#define MAX_STRING_LENGTH 80
#define MAX_NUMBER_LENGTH 80
#define MAX_AST_ERROR_LENGTH 80

#define FIRST_REGISTER_NUM 0
#define LAST_REGISTER_NUM 7

#define NO_OPERAND 0 /* For words that doesn't take operand */
#define GROUP_A_WORD_LENGTH 3 /* The amount of words take place in the memory for group a */
#define DEFAULT_WORD_LENGTH 2 /* The default amount of words take place in the memory */
#define GROUP_C_WORD_LENGTH 1 /* The amount of words take place in the memory for group c */
#define IMMEDIATE_ADDRESS 1 /* Address method */
#define DIRECT_ADDRESS 3 /* Address method */
#define REGISTER_ADDRESS 5 /* Address method */
#define EXTERN_DECIMAL_ADDRESS -1 /* First pass - no address for .extern symbols */

/* Defines a boolean ast_word_type (T/F) */
typedef enum {
    FALSE,
    TRUE
} bool;

#endif /* GLOBALS_H */
