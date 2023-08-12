#ifndef MMN14_SYMBOLS_H
#define MMN14_SYMBOLS_H

#include "globals.h"
#include "ast.h"

/**
 * Enumerates the possible types of symbols in the symbol table.
 */
typedef enum {
    DATA, /* Data symbol type */
    EXTERNAL, /* External symbol type */
    CODE, /* Code symbol type */
    ENTRY /* Entry symbol type */
} symbol_type;

/**
 * Represents a node in the symbol table.
 */
typedef struct symbol_node {
    char *symbol_name; /* The name of the symbol */
    int decimal_address; /* The decimal address of the symbol */
    symbol_type symbol_type; /* The type of the symbol */
    struct symbol_node *next_symbol; /* Pointer to the next symbol node */
} symbol_node;

/**
 * Represents a symbol table that stores information about symbols in the assembly program.
 */
typedef struct {
    symbol_node *first; /* Pointer to the first symbol node in the table */
    symbol_node *last; /* Pointer to the last symbol node in the table */
} symbol_table;



bool is_symbol(char *line);

symbol_node *create_symbol(char *symbol_name, int decimal_address, symbol_type symbol_type);

symbol_table *initialize_symbol_table();

bool add_symbol(symbol_table *table, char *symbol_name, int *decimal_address, symbol_type symbol_type);

bool is_symbol_valid(char *symbol_name);

void mark_symbol_as_entry(symbol_table *table, char* symbol_name);

int get_symbol_address(symbol_table *table, symbol symbol_name);

bool check_entry_symbol_duplication(symbol_table *table, char symbol[31]);

bool has_entry_symbol(symbol_table *table);

bool has_extern_symbol(symbol_table *table);

bool is_symbol_extern(symbol_table *table, symbol symbol);

#endif /* MMN14_SYMBOLS_H */
