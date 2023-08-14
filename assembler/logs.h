
/* Macro definitions for handling errors in the AST */

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

/* Message types for PRINT_MESSAGE macro */
typedef enum {
    INFO_MSG_TYPE,
    ERROR_MSG_TYPE
} message_type;

/* Macro to print messages with type and content */
#define PRINT_MESSAGE(type, msg) \
    do { \
        const char *typeStr = ((type) == INFO_MSG_TYPE) ? "Info" : "Error"; \
        printf("%s: %s\n", typeStr, msg); \
    } while (0)

/* Informational Messages */

#define INFO_START_OF_ASSEMBLER   "Start processing files..."
#define INFO_FIRST_PASS   "First pass completed successfully."
#define INFO_SECOND_PASS  "Second pass completed successfully."
#define INFO_ALL_FILES_HAVE_BEEN_PROCESSED "All files have been processed!"

/* Error Messages */

#define ERROR_IN_PREPROCESSING       "Preprocessing step encountered errors. Macros could not be spread correctly."
#define ERROR_INVALID_MACRO_NAME     "Invalid macro name, macro name can't be a reserved word."

#define ERROR_IN_FIRST_PASS        "First pass encountered errors."
#define ERROR_IN_SECOND_PASS       "Second pass encountered errors."

#define ERROR_FAILED_READING_AS_FILE       "Failed to open .as file for reading."
#define ERROR_FAILED_WRITING_TO_AM_FILE    "Failed to open .am file for writing."
#define ERROR_FAILED_READING_AM_FILE       "Failed to open .am file for reading."
#define ERROR_FAILED_WRITING_TO_OB_FILE    "Failed to open .ob file for reading."
#define ERROR_FAILED_WRITING_TO_ENT_FILE   "Failed to open entries file for reading."
#define ERROR_FAILED_WRITING_TO_EXT_FILE   "Failed to open externals file for reading."

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

#define ERROR_DATA_MISSING_INT                "The instruction .data must have at least one integer."
#define ERROR_ARGS_FOR_DATA_MUST_BE_INTEGERS  "Arguments for data must be integers."
#define ERROR_INTEGERS_NOT_IN_RANGE           "Integers for data must be between -2048 and 2047"

#define ERROR_MISSING_OPERAND             "Missing operand."
#define ERROR_ILLEGAL_OPERAND             "Illegal operand."
#define ERROR_ILLEGAL_OPERAND_TYPE        "Illegal operand type."

#define ERROR_INTEGER_OUT_OF_RANGE        "Integer is out of range."
#define ERROR_COMMAND_DOES_NOT_EXIST      "Command does not exist."
#define ERROR_TRAILING_CHARACTERS         "Detected trailing characters at the end."
#define ERROR_ENTRY_EXTERN_WITHOUT_SYMBOL "Entry/Extern must be followed by a symbol."

#define ERROR_SYMBOL_ALREADY_EXISTS    "Symbol already exists."
#define ERROR_SYMBOL_DOES_NOT_EXIST    "Symbol does not exist."

