#include <malloc.h>
#include <string.h>
#include "tables.h"

// Function to create a new symbol node
symbol_node* create_symbol(char* symbol_name, int decimal_address, symbol_type symbol_type) {
    symbol_node* new_symbol = (symbol_node*)malloc(sizeof(symbol_node));
    new_symbol->symbol_name = strdup(symbol_name);
    new_symbol->decimal_address = decimal_address;
    new_symbol->symbol_type = symbol_type;
    new_symbol->next_symbol = NULL;
    return new_symbol;
}

// Function to initialize an empty symbol table
void initialize_symbol_table(symbol_table* symbol_table) {
    symbol_table->first = NULL;
    symbol_table->last = NULL;
}

// Function to add a symbol to the symbol table
void add_symbol(symbol_table* table, char* symbol_name, int decimal_address, symbol_type symbol_type) {
    symbol_node* new_symbol = create_symbol(symbol_name, decimal_address, symbol_type);

    if (table->first == NULL) {
        // If the symbol table is empty, set the new symbol as both head and tail
        table->first = new_symbol;
        table->last = new_symbol;
    } else {
        // Otherwise, add the new symbol at the end and update the tail
        table->last->next_symbol = new_symbol;
        table->last = new_symbol;
    }
}

// Function to create a new code node
code_node* create_code_node(int L, ast ast) {
    code_node* new_node = (code_node*)malloc(sizeof(code_node));
    new_node->word = NULL;
    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    return new_node;
}

// Function to initialize an empty code image
void initialize_code_image(code_image* code_image) {
    code_image->first = NULL;
    code_image->last = NULL;
}

// Function to add a code node to the code image
void add_code_node(code_image* code_image, int L, ast ast) {
    code_node* new_node = create_code_node(L, ast);

    if (code_image->first == NULL) {
        code_image->first = new_node;
        code_image->last = new_node;
    } else {
        code_image->last->next = new_node;
        code_image->last = new_node;
    }
}

// Function to create a new data node
data_node* create_data_node(int L, ast ast) {
    data_node* new_node = (data_node*)malloc(sizeof(data_node));
    new_node->word = NULL;
    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    return new_node;
}

// Function to initialize an empty data image
void initialize_data_image(data_image* data_image) {
    data_image->first = NULL;
    data_image->last = NULL;
}

// Function to add a data node to the data image
void add_data_node(data_image* data_image, int L, ast ast) {
    data_node* new_node = create_data_node(L, ast);

    if (data_image->first == NULL) {
        data_image->first = new_node;
        data_image->last = new_node;
    } else {
        data_image->last->next = new_node;
        data_image->last = new_node;
    }
}


/* TODO implement symbol table + macros table */