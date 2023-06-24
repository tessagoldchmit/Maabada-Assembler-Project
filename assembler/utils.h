#ifndef UTILS_H
#define UTILS_H

#include "globals.h"

int count_line_length(const char *);

char *skip_spaces(char *);

bool empty_string(char *);

int word_length(char *);

char* concatenate_strings(char*, char* );

#endif /* UTILS_H */