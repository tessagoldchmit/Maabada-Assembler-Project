#include "utils.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

int count_line_length(char *p) {
    int i = 0;
    while (p[i] != '\n' && p[i])
        i++;
    return i + 1;
}

char *skip_spaces(char *str) {
    int i = 0;
    while (str[i] == ' ' || str[i] == '\t')
        i++;
    return str + i;
}

bool empty_string(char *str) {
    char *p = skip_spaces(str);
    if (*p == '\n' || !*p)
        return TRUE;
    return FALSE;
}

int word_length(char *str) {
    int i;
    for (i = 0; !isspace(str[i]) && str[i]; i++);
    return i;
}

char *concatenate_strings(char *base, char *ext) {
    /* Calculate the length of the resulting string */
    size_t base_len = strlen(base);
    size_t ext_len = strlen(ext);
    size_t total_len = base_len + ext_len;

    /* Allocate memory for the resulting string */
    char *concatenated_string = malloc(total_len + 1);
    if (concatenated_string == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(concatenated_string);
        exit(1);
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
