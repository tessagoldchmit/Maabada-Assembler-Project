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
int count_line_length(const char *p) {
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

/**
 * Concatenates a string with an extension.
 *
 * This function takes two strings, `str1` and `ext`, and concatenates them to create
 * a new string. The resulting string will have the content of `str1` followed by the
 * content of `ext`.
 *
 * @param str1 The base string.
 * @param ext The extension string.
 * @return A dynamically allocated string containing the concatenated result.
 *         The caller is responsible for freeing the memory allocated for the returned string.
 */
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
char* concatenate_strings(char* base, char* ext) {
    /* Calculate the length of the resulting string */
    size_t base_len = strlen(base);
    size_t ext_len = strlen(ext);
    size_t total_len = base_len + ext_len;

    /* Allocate memory for the resulting string */
    char* concatenated_string = malloc(total_len + 1);
    if (concatenated_string == NULL) {
        return NULL;
    }

    strcpy(concatenated_string, base);
    strcat(concatenated_string, ext);
    return concatenated_string;
}