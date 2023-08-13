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

/**
 * Represents a node for tracking external symbols.
 */
typedef struct extern_node {
    char *symbol_name; /* The name of the external symbol. */
    int address; /* The address associated with the external symbol. */
    struct extern_node *next; /* Pointer to the next extern node in the linked list. */
} extern_node;

/**
 * Represents a table for managing external symbols.
 */
typedef struct extern_table {
    extern_node *first; /* Pointer to the first extern node in the linked list. */
    extern_node *last; /* Pointer to the last extern node in the linked list. */
} extern_table;

/**
 * Checks if the line contains a symbol.
 * @param line Pointer to the input line.
 * @return Boolean indicating whether the line contains a symbol.
 */
bool is_symbol(char *line);

/**
 * Initialize an empty symbol table.
 *
 * @param symbol_table The symbol table to initialize.
 */
symbol_table *initialize_symbol_table();

/**
 * Add a symbol to the symbol table.
 *
 * @param table The symbol table.
 * @param symbol_name The name of the symbol to add.
 * @param decimal_address The decimal address associated with the symbol.
 * @param symbol_type The ast_word_type of the symbol (DATA / EXTERNAL / CODE).
 * @return True if the symbol was successfully added, False if the symbol already exists in the table.
 */
bool add_symbol(symbol_table *table, char *symbol_name, int *decimal_address, symbol_type symbol_type);

/**
 * Checks if a symbol is valid, satisfying the following conditions:
 * - Length is within the maximum symbol length limit.
 * - Does not match any reserved words.
 * - Starts with an alphabetic character.
 * - Contains only alphanumeric characters.
 *
 * @param symbol_name The symbol to check
 * @return true if the symbol is valid, false otherwise
*/
bool is_symbol_valid(char *symbol_name);

/**
 * Marks a symbol in the symbol table as an entry symbol.
 *
 * @param table Pointer to the symbol table.
 * @param symbol_name The name of the symbol to mark as an entry.
 */
void mark_symbol_as_entry(symbol_table *table, char* symbol_name);

/**
 * Retrieves the decimal address of a symbol in the symbol table.
 *
 * @param table Pointer to the symbol table.
 * @param symbol_name The name of the symbol to get the address for.
 * @return Returns the decimal address of the symbol, or NON_EXIST_SYMBOL_ADDRESS if not found.
 */
int get_symbol_address(symbol_table *table, symbol symbol_name);

/**
 * Checks if a duplicate entry symbol with the given name exists in the symbol table.
 *
 * @param table Pointer to the symbol table.
 * @param symbol_name The name of the symbol to check for.
 * @return Returns TRUE if a duplicate entry symbol is found, otherwise FALSE.
 */
bool check_entry_symbol_duplication(symbol_table *table, char symbol[31]);

/**
 * Checks if there is at least one entry symbol in the symbol table.
 *
 * @param table Pointer to the symbol table.
 * @return Returns TRUE if an entry symbol is found, otherwise FALSE.
 */
bool has_entry_symbol(symbol_table *table);

/**
 * Checks if there is at least one external symbol in the symbol table.
 *
 * @param table Pointer to the symbol table.
 * @return Returns TRUE if an external symbol is found, otherwise FALSE.
 */
bool has_extern_symbol(symbol_table *table);

/**
 * Checks if a symbol in the symbol table is marked as external.
 *
 * @param table Pointer to the symbol table.
 * @param symbol The symbol to check.
 * @return Returns TRUE if the symbol is marked as external, otherwise FALSE.
 */
bool is_symbol_extern(symbol_table *table, symbol symbol);

/**
    Initializes an extern table.

    @return A pointer to the initialized extern table.
*/
extern_table *initialize_extern_table();

/**
    Adds a new extern node to the extern table.

    @param table The extern table where the new node will be added.
    @param symbol_name The name of the external symbol.
    @param address The address of the external symbol.
*/
void add_extern_node(extern_table *table, char *symbol_name, int address);

void free_symbols(symbol_table *symbol_table, extern_table *extern_table);

#endif /* MMN14_SYMBOLS_H */
