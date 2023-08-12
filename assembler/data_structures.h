#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "ast.h"

typedef struct code_node {
    char *original_line;
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
    char *original_line;
    int *word;
    int L;
    ast ast;
    struct data_node *next_node;
} data_node;

typedef struct {
    data_node *first;
    data_node *last;
} data_image;


code_node *create_code_node(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

code_node *create_code_node_registers(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

code_image *initialize_code_image();

void add_code_node(code_image *code_image, code_node *new_node);

data_node *create_data_node(char* line, int L, ast ast);

data_image *initialize_data_image();

void add_data_node(data_image *data_image, data_node *new_node);

unsigned short insert_bits(unsigned short value, int num, int start_bit, int end_bit);

code_node* find_code_node_by_line(code_image* code_image, char* line);

#endif /* DATA_STRUCTURES_H */
