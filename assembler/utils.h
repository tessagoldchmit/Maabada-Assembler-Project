#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "globals.h"
#include "symbols.h"

/**
 * Counts the length of a line in characters.
 *
 * @param p: The pointer to the input string.
 * @return: The length of the line in characters.
 */
int count_line_length(char *);

/**
 * Skips leading spaces and tabs in a string and returns a pointer to the first non-space character.
 *
 * @param str: The input string.
 * @return: A pointer to the first non-space character in the string.
 */
char *skip_spaces(char *);

/**
 * Checks if a string is empty or consists of only spaces and tabs.
 *
 * @param str: The input string.
 * @return: True if the string is empty or consists of only spaces and tabs, False otherwise.
 */
bool empty_string(char *);

/**
 * Computes the length of the first ast_word in a string.
 *
 * @param str: The input string.
 * @return: The length of the first ast_word in the string.
 */
int word_length(char *);

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
char *concatenate_strings(char *, char *);

/**
 * Checks if the given operand is a valid numeric constant.
 * @param operand The operand to be checked.
 * @return Returns TRUE if the operand is a valid numeric constant, otherwise FALSE.
 */
bool is_operand_a_number(char *operand);

/**
 * Checks if the given operand is a valid register representation.
 * @param operand The operand to be checked.
 * @return Returns TRUE if the operand is a valid register representation, otherwise FALSE.
 */
bool is_operand_a_register(char *operand);


#endif /* UTILS_H */