#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "ast.h"

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
    struct data_node *next_node;
} data_node;

typedef struct {
    data_node *head;
    data_node *last_node;
} data_image;


code_node *create_code_node(int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

code_node *create_code_node_registers(int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

code_image *initialize_code_image();

void add_code_node(code_image *code_image, code_node *new_node);

data_node *create_data_node(int L, ast ast);

data_image *initialize_data_image();

void add_data_node(data_image *data_image, data_node *new_node);

#endif /* DATA_STRUCTURES_H */
