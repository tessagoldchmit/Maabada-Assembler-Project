#include <malloc.h>
#include <string.h>
#include "data_structures.h"
#include "utils.h"

/**
 * Create a new symbol node.
 *
 * @param symbol_name The name of the symbol.
 * @param decimal_address The decimal address associated with the symbol.
 * @param symbol_type The type of the symbol (DATA / EXTERNAL / CODE).
 * @return A pointer to the newly created symbol node.
 */
symbol_node *create_symbol(char *symbol_name, int decimal_address, symbol_type symbol_type) {
    symbol_node *new_symbol = (symbol_node *) malloc(sizeof(symbol_node));
    new_symbol->symbol_name = strdup(symbol_name);
    new_symbol->decimal_address = decimal_address;
    new_symbol->symbol_type = symbol_type;
    new_symbol->next_symbol = NULL;
    return new_symbol;
}

/**
 * Initialize an empty symbol table.
 *
 * @param symbol_table The symbol table to initialize.
 */
void initialize_symbol_table(symbol_table *symbol_table) {
    symbol_table->first = NULL;
    symbol_table->last = NULL;
}

/**
 * Add a symbol to the symbol table.
 *
 * @param table The symbol table.
 * @param symbol_name The name of the symbol to add.
 * @param decimal_address The decimal address associated with the symbol.
 * @param symbol_type The type of the symbol (DATA / EXTERNAL / CODE).
 * @return True if the symbol was successfully added, False if the symbol already exists in the table.
 */
bool add_symbol(symbol_table *table, char *symbol_name, int decimal_address, symbol_type symbol_type) {
    /* Check if the symbol already exists in the table */
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            /* Symbol already exists, return false */
            return FALSE;
        }
        current = current->next_symbol;
    }

    /* Create a new symbol node */
    symbol_node *new_symbol = create_symbol(symbol_name, decimal_address, symbol_type);

    if (table->first == NULL) {
        /* If the symbol table is empty, set the new symbol as both head and tail */
        table->first = new_symbol;
        table->last = new_symbol;
    } else {
        /* Otherwise, add the new symbol at the end and update the tail */
        table->last->next_symbol = new_symbol;
        table->last = new_symbol;
    }
    return TRUE;
}


/* Function to create a new code node */
code_node *create_code_node(int L, ast ast) {
    code_node *new_node = (code_node *) malloc(sizeof(code_node));
    new_node->word = NULL;
    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    return new_node;
}

/**
 * Initialize an empty code image.
 *
 * @param code_image The code image to initialize.
 */
void initialize_code_image(code_image *code_image) {
    code_image->first = NULL;
    code_image->last = NULL;
}

/* Function to add a code node to the code image */
void add_code_node(code_image *code_image, int L, ast ast) {
    code_node *new_node = create_code_node(L, ast);

    if (code_image->first == NULL) {
        code_image->first = new_node;
        code_image->last = new_node;
    } else {
        code_image->last->next = new_node;
        code_image->last = new_node;
    }
}

/* Creates a new data node */
data_node *create_data_node(int L, ast ast) {
    data_node *new_node = (data_node *) malloc(sizeof(data_node));
    new_node->word = NULL;
    new_node->L = L;
    new_node->ast = ast;
    new_node->next_node = NULL;
    return new_node;
}

/**
 * Initialize an empty data image.
 *
 * @param data_image The data image to initialize.
 */
data_node* initialize_data_image() {
    data_image* new_data_image = malloc(sizeof(data_image));
    new_data_image->head = NULL;
    new_data_image->last_node = NULL;

    return new_data_image;
}

/* Adds a data node to the data image */
void add_data_node(data_image *data_image, data_node *new_node) {
    if (data_image->head == NULL) {
        data_image->head = new_node;
        data_image->last_node = new_node;
    } else {
        data_image->last_node->next_node = new_node;
        data_image->last_node = new_node;
    }
}