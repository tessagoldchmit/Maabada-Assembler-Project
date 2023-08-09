#ifndef MMN14_SYMBOLS_H
#define MMN14_SYMBOLS_H

#include "globals.h"
#include "ast.h"

typedef enum {
    DATA,
    EXTERNAL,
    CODE,
    ENTRY
} symbol_type;

typedef struct symbol_node {
    char *symbol_name;
    int decimal_address;
    symbol_type symbol_type;
    struct symbol_node *next_symbol;
} symbol_node;

typedef struct {
    symbol_node *first;
    symbol_node *last;
} symbol_table;


symbol_node *create_symbol(char *symbol_name, int decimal_address, symbol_type symbol_type);

symbol_table *initialize_symbol_table();

bool add_symbol(symbol_table *table, char *symbol_name, int *decimal_address, symbol_type symbol_type);

bool is_symbol_valid(char *symbol_name);

void print_symbol_table(symbol_table *table);

void mark_symbol_as_entry(symbol_table *table, char* symbol_name);

int get_symbol_address(symbol_table *table, symbol symbol_name);

bool check_entry_symbol_duplication(symbol_table *table, char symbol[31]);

#endif /* MMN14_SYMBOLS_H */
