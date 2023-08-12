
/* Macro definition for the error handling code */
#define HANDLE_AST_ERROR(ast, error_msg) \
    do { \
        (*(ast))->ast_word_type = ERROR; \
        strcpy((*(ast))->ast_word.error_word, error_msg); \
        printf("Error in line %d: %s\n", (*(ast))->line_number, error_msg); \
    } while (0)

#define HANDLE_AST_ERROR_NON_POINTER(ast, error_msg) \
    do { \
        (ast)->ast_word_type = ERROR; \
        strcpy((ast)->ast_word.error_word, error_msg); \
        printf("Error in line %d: %s\n", (ast)->line_number, error_msg); \
    } while (0)

typedef enum {
    INFO_MSG_TYPE,
    WARNING_MSG_TYPE,
    ERROR_MSG_TYPE
} message_type;

#define PRINT_MESSAGE(type, msg) \
    do { \
        const char *typeStr = ((type) == INFO_MSG_TYPE) ? "Info" : ((type) == WARNING_MSG_TYPE) ? "Warning" : "Error"; \
        printf("%s: %s\n", typeStr, msg); \
    } while (0)

/* Info */

#define INFO_FIRST_PASS   "First pass completed successfully."
#define INFO_SECOND_PASS  "Second pass completed successfully."
#define INFO_ALL_FILES_HAVE_BEEN_PROCESSED "All files have been processed!"

/* Warnings */

/* Errors */

#define ERROR_IN_PREPROCESSING     "Preprocessing step encountered errors. Macros could not be spread correctly."
#define ERROR_IN_FIRST_PASS        "First pass encountered errors."
#define ERROR_IN_SECOND_PASS       "Second pass encountered errors."

#define ERROR_FAILED_READING_AS_FILE       "Failed to open .as file for reading."
#define ERROR_FAILED_WRITING_TO_AM_FILE    "Failed to open .am file for writing."

#define ERROR_FAILED_TO_REALLOCATE_MEM     "Failed to reallocate memory."
#define ERROR_FAILED_TO_ALLOCATE_MEM       "Failed to allocate memory"
#define ERROR_MEMORY_SIZE_EXCEEDED       "Memory size exceeded"

#define ERROR_NO_FILES_PROVIDED      "No file names provided."
#define ERROR_FILE_NAME_TOO_LONG     "File name exceeds maximum length."

#define ERROR_INVALID_REGISTER     "Invalid register number."
#define ERROR_INVALID_SYMBOL       "Invalid symbol."
#define ERROR_INVALID_NUMBER       "Invalid number."
#define ERROR_INVALID_INSTRUCTION  "Invalid instruction."

#define ERROR_CONSECUTIVE_COMMAS   "Consecutive commas detected."
#define ERROR_MISSING_COMMA        "Missing comma."
#define ERROR_LEADING_COMMA        "Leading comma detected."
#define ERROR_TRAILING_COMMA       "Trailing comma detected."

#define ERROR_STRING_MISSING       ".string instruction must have a string."
#define ERROR_STRING_START_QUOTES  "String must start with quotes(\")."
#define ERROR_STRING_END_QUOTES    "String must end with quotes(\")."

#define ERROR_DATA_MISSING_INT     ".data instruction must have at least one integer."

#define ERROR_MISSING_OPERAND             "Missing operand."
#define ERROR_ILLEGAL_OPERAND             "Illegal operand."
#define ERROR_ILLEGAL_OPERAND_TYPE        "Illegal operand type."

#define ERROR_INTEGER_OUT_OF_RANGE        "Integer is out of range."
#define ERROR_COMMAND_DOES_NOT_EXIST      "Command does not exist."
#define ERROR_TRAILING_CHARACTERS         "Detected trailing characters at the end."
#define ERROR_ENTRY_EXTERN_WITHOUT_SYMBOL "Entry/Extern must be followed by a symbol."

#define ERROR_SYMBOL_ALREADY_EXISTS    "Symbol already exists."
#define ERROR_SYMBOL_DOES_NOT_EXIST    "Symbol does not exist."

