#ifndef TABLES_H
#define TABLES_H

#include "ast.h"

typedef enum {
    DATA,
    EXTERNAL,
    CODE
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

typedef struct code_node {
    int *word;
    int L;
    ast ast;
    struct code_node *next;
} code_node;

typedef struct {
    code_node *first;
    code_node *last;
} code_image;

typedef struct data_node {
    int *word;
    int L;
    ast ast;
    struct data_node *next;
} data_node;

typedef struct {
    data_node *first;
    data_node *last;
} data_image;

symbol_node *create_symbol(char *symbol_name, int decimal_address, symbol_type symbol_type);

void initialize_symbol_table(symbol_table *symbol_table);

void add_symbol(symbol_table *table, char *symbol_name, int decimal_address, symbol_type symbol_type);

code_node *create_code_node(int L, ast ast);

void initialize_code_image(code_image *code_image);

void add_code_node(code_image *code_image, int L, ast ast);

data_node *create_data_node(int L, ast ast);

void initialize_data_image(data_image *data_image);

void add_data_node(data_image *data_image, int L, ast ast);

#endif // TABLES_H
