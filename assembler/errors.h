#ifndef MMN14_ERRORS_H
#define MMN14_ERRORS_H

#include <stdio.h>
#include <string.h>

/* Macro definition for the error handling code */
#define HANDLE_ERROR(ast, error_message) \
    { \
    (*(ast))->ast_word_type = ERROR;     \
        strcpy((*(ast))->ast_word.error_word, (error_message)); \
    }


/* Error messages */
char ERROR_INVALID_REGISTER[] = "Invalid register number";
char ERROR_INVALID_SYMBOL[] = "Invalid symbol";
char ERROR_INVALID_NUMBER[] = "Invalid number.";
char ERROR_INVALID_INSTRUCTION[] = "Invalid instruction";

char ERROR_CONSECUTIVE_COMMAS[] = "Consecutive commas detected.";
char ERROR_MISSING_COMMA[] = "Missing comma.";
char ERROR_LEADING_COMMA[] = "Leading comma detected.";
char ERROR_TRAILING_COMMA[] = "Trailing comma detected.";

char ERROR_STRING_MISSING[] = ".string instruction must have a string.";
char ERROR_STRING_START_QUOTES[] = "String must start with quotes(\").";
char ERROR_STRING_END_QUOTES[] = "String must end with quotes(\").";

char ERROR_DATA_MISSING_INT[] = ".data instruction must have at least one integer.";

char ERROR_MISSING_OPERAND[] = "Missing operand.";
char ERROR_ILLEGAL_OPERAND[] = "Illegal operand.";

char ERROR_INTEGER_OUT_OF_RANGE[] = "Integer is out of range.";
char ERROR_COMMAND_DOES_NOT_EXIST[] = "Command does not exist.";
char ERROR_TRAILING_CHARACTERS[] = "Detected trailing characters at the end.";
char ERROR_ENTRY_EXTERN_WITHOUT_SYMBOL[] = "Entry/Extern must be followed by a symbol.";

#endif /* MMN14_ERRORS_H */
