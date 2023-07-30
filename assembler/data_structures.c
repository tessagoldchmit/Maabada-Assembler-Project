#include "data_structures.h"
#include <malloc.h>

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
code_image *initialize_code_image() {
    code_image *new_code_image = malloc(sizeof(code_image));
    new_code_image->first = NULL;
    new_code_image->last = NULL;

    return new_code_image;
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
data_image *initialize_data_image() {
    data_image *new_data_image = malloc(sizeof(data_image));
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