#include "utils.h"

#include <ctype.h>

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
 * Computes the length of the first word in a string.
 *
 * @param str: The input string.
 * @return: The length of the first word in the string.
 */
int word_length(char *str) {
    int i;
    for (i=0; !isspace(str[i]) && str[i]; i++);
    return i;
}
