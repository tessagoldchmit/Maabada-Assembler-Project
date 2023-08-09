#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include "globals.h"
#include "symbols.h"

int count_line_length(char *);

char *skip_spaces(char *);

bool empty_string(char *);

int word_length(char *);

char *concatenate_strings(char *, char *);

void *safe_malloc(size_t size);

bool is_operand_a_number(char *operand);

bool is_operand_a_register(char *operand);

void write_entries_file(char *filename, symbol_table *table);

void write_externals_file(char *filename, symbol_table *table);

#endif /* UTILS_H */