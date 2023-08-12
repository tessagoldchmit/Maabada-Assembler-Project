#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "globals.h"
#include "symbols.h"
#include "extern_symbols.h"

int count_line_length(char *);

char *skip_spaces(char *);

bool empty_string(char *);

int word_length(char *);

char *concatenate_strings(char *, char *);

bool is_operand_a_number(char *operand);

bool is_operand_a_register(char *operand);


#endif /* UTILS_H */