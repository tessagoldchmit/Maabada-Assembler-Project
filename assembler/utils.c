#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <malloc.h>

/**
 * Counts the length of a line in characters.
 *
 * @param p: The pointer to the input string.
 * @return: The length of the line in characters.
 */
int count_line_length(char *p) {
    int i = 0;
    while (p[i] != '\n' && p[i])
        i++;
    return i + 1;
}

/**
 * Skips leading spaces and tabs in a string and returns a pointer to the first non-space character.
 *
 * @param str: The input string.
 * @return: A pointer to the first non-space character in the string.
 */
char *skip_spaces(char *str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    return str + i;
}

/**
 * Checks if a string is empty or consists of only spaces and tabs.
 *
 * @param str: The input string.
 * @return: True if the string is empty or consists of only spaces and tabs, False otherwise.
 */
bool empty_string(char *str) {
    char *p = skip_spaces(str);
    if (*p == '\n' || !*p)
        return TRUE;
    return FALSE;
}

/**
 * Computes the length of the first ast_word in a string.
 *
 * @param str: The input string.
 * @return: The length of the first ast_word in the string.
 */
int word_length(char *str) {
    int i;
    for (i = 0; !isspace(str[i]) && str[i]; i++);
    return i;
}

/**
 * Allocate memory with ast_error handling.
 * The returned pointer should be freed using the standard free function when it is no longer needed.
 *
 * @param size The size of the memory block to allocate.
 * @return A pointer to the allocated memory block.
 */
void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        fprintf(stderr, "Failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    return ptr;
}


/**
 * Concatenates a base string with an extension.
 *
 * This function takes two strings, `base` and `ext`, and concatenates them to create
 * a new string. The resulting string will have the content of `base` followed by the
 * content of `ext`.
 *
 * @param base The base string.
 * @param ext The extension string.
 * @return A dynamically allocated string containing the concatenated result.
 *         The caller is responsible for freeing the memory allocated for the returned string.
 */
char *concatenate_strings(char *base, char *ext) {
    /* Calculate the length of the resulting string */
    size_t base_len = strlen(base);
    size_t ext_len = strlen(ext);
    size_t total_len = base_len + ext_len;

    /* Allocate memory for the resulting string */
    char *concatenated_string = malloc(total_len + 1);
    if (concatenated_string == NULL) {
        return NULL;
    }

    strcpy(concatenated_string, base);
    strcat(concatenated_string, ext);
    return concatenated_string;
}


bool is_operand_a_number(char *operand) {
    if (*operand == '-' || *operand == '+') {
        operand++; /* Move past the sign character */
    }
    bool has_digits = FALSE;
    while (*operand != '\0') {
        if (!isdigit(*operand)) {
            return FALSE;
        }
        has_digits = TRUE;
        operand++;
    }

    return has_digits;
}

bool is_operand_a_register(char *operand) {
    if (strlen(operand) == 3 && operand[0] == '@' && operand[1] == 'r' && isdigit(operand[2]))
        return TRUE;
    return FALSE;
}