#include "data_structures.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "temp.h"


int char_to_binary(char character) {
    return (int)character;
}

unsigned short insert_bits(unsigned short value, int num, int start_bit, int end_bit) {
    unsigned short mask = ((1 << (end_bit - start_bit + 1)) - 1) << start_bit;
    unsigned short num_bits = num << start_bit;
    return (value & ~mask) | num_bits;
}

/*this operation already binaring all the words*/
code_node *create_code_node_registers(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source) {
    int i;

    code_node *new_node = (code_node *) malloc(sizeof(code_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node);
        exit(1);
    }
    new_node->word = (int*)malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node->word);
        exit(1);
    }
    for (i = 0; i < L; i++)
        new_node->word[i]=0; /* Making sure zeroed */

    new_node->word[0] = insert_bits(new_node->word[0], new_operand_target, 2, 4);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_code, 5, 8);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_source, 9, 11);

    new_node->word[1] = insert_bits(new_node->word[1], atoi(&ast.ast_word.instruction_word.instruction_union.group_a.target_value.register_num), 2, 6);
    new_node->word[1] = insert_bits(new_node->word[1], atoi(&ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num), 7, 11);

    /** TEMP **/
    int t;
    for (t = 0; t < L; t++) {
        print_binary_12bits(new_node->word[t] & 0xFFF);
        printf("\n");
    }
    /** END OF TEMP **/

    new_node->original_line = (char*)malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->original_line, line);

    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    return new_node;
}

/* Function to create a new code node */
code_node *create_code_node(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source) {
    int i;

    code_node *new_node = (code_node *) malloc(sizeof(code_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node);
        exit(1);
    }
    new_node->word = (int*)malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node->word);
        exit(1);
    }
    for (i = 0; i < L; i++)
        new_node->word[i]=0; /* Making sure zeroed */

    new_node->word[0] = insert_bits(new_node->word[0], new_operand_target, 2, 4);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_code, 5, 8);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_source, 9, 11);

    /** TEMP **/
    int t;
    for (t = 0; t < L; t++) {
        print_binary_12bits(new_node->word[t] & 0xFFF);
        printf("\n");
    }
    /** END OF TEMP **/

    new_node->original_line = (char*)malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->original_line, line);

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
    if (new_code_image == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_code_image);
        exit(1);
    }
    new_code_image->first = NULL;
    new_code_image->last = NULL;

    return new_code_image;
}

/* Function to add a code node to the code image */
void add_code_node(code_image *code_image, code_node *new_node) {
    if (code_image->first == NULL) {
        code_image->first = new_node;
        code_image->last = new_node;
    } else {
        code_image->last->next = new_node;
        code_image->last = new_node;
    }
}

/* Creates a new data node */
data_node *create_data_node(char* line, int L, ast ast) {
    data_node *new_node = (data_node *) malloc(sizeof(data_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node);
        exit(1);
    }
    new_node->word = (int*)malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_node->word);
        exit(1);
    }
    int i;
    if(ast.ast_word.directive_word.directive_type == STRING_TYPE)
        for (i = 0; i <= L; i++)
            new_node->word[i] = char_to_binary(ast.ast_word.directive_word.directive_option.string[i]);
    else
        for (i = 0; i < L; i++)
            new_node->word[i] = ast.ast_word.directive_word.directive_option.machine_code.machine_code_array[i];

    /** TEMP **/
    int t;
    for (t = 0; t < L; t++) {
        print_binary_12bits(new_node->word[t] & 0xFFF);
        printf("\n");
    }
    /** END OF TEMP **/

    new_node->original_line = (char*)malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory.\n");
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->original_line, line);

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
    if (new_data_image == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        free(new_data_image);
        exit(1);
    }
    new_data_image->first = NULL;
    new_data_image->last = NULL;

    return new_data_image;
}

/* Adds a data node to the data image */
void add_data_node(data_image *data_image, data_node *new_node) {
    if (data_image->first == NULL) {
        data_image->first = new_node;
        data_image->last = new_node;
    } else {
        data_image->last->next_node = new_node;
        data_image->last = new_node;
    }
}

code_node* find_code_node_by_line(code_image* code_image, char* line) {
    size_t line_length = strlen(line);
    if (line_length > 0 && line[line_length - 1] == '\n') {
        line[line_length - 1] = '\0';  /* Replace newline with null terminator */
    }
    code_node* current = code_image->first;
    while (current != NULL) {
        if (strcmp(current->original_line, line) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

data_node* find_data_node_by_line(data_image* data_image, char* line) {
    size_t line_length = strlen(line);
    if (line_length > 0 && line[line_length - 1] == '\n') {
        line[line_length - 1] = '\0';  /* () Replace newline with null terminator */
    }
    data_node* current = data_image->first;
    while (current != NULL) {
        if (strcmp(current->original_line, line) == 0) {
            return current;
        }
        current = current->next_node;
    }
    return NULL;
}
