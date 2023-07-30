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

/* Defines a boolean ast_word_type (T/F) */
typedef enum {
    FALSE,
    TRUE
} bool;

#endif /* GLOBALS_H */
