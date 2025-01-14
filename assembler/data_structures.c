#include "data_structures.h"
#include "logs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

unsigned short insert_bits(unsigned short value, int num, int start_bit, int end_bit) {
    unsigned short mask = ((1 << (end_bit - start_bit + 1)) - 1) << start_bit;
    unsigned short num_bits = num << start_bit;
    return (value & ~mask) | num_bits;
}

code_node *create_code_node_registers(char *line, int L, ast ast, int new_operand_code, int new_operand_target,
                                      int new_operand_source) {
    int i;

    code_node *new_node = (code_node *) malloc(sizeof(code_node));
    if (new_node == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node);
        exit(1);
    }
    new_node->word = (int *) malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node->word);
        exit(1);
    }
    for (i = 0; i < L; i++)
        new_node->word[i] = 0; /* Making sure zeroed */

    new_node->word[0] = insert_bits(new_node->word[0], new_operand_target, 2, 4);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_code, 5, 8);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_source, 9, 11);

    new_node->word[1] = insert_bits(new_node->word[1],
                                    atoi(&ast.ast_word.instruction_word.instruction_union.group_a.target_value.register_num),
                                    2, 6);
    new_node->word[1] = insert_bits(new_node->word[1],
                                    atoi(&ast.ast_word.instruction_word.instruction_union.group_a.source_value.register_num),
                                    7, 11);

    new_node->original_line = (char *) malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->original_line, line);

    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    new_node->been_here = FALSE;
    return new_node;
}

code_node *
create_code_node(char *line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source) {
    int i;

    code_node *new_node = (code_node *) malloc(sizeof(code_node));
    if (new_node == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node);
        exit(1);
    }
    new_node->word = (int *) malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node->word);
        exit(1);
    }
    for (i = 0; i < L; i++)
        new_node->word[i] = 0; /* Making sure zeroed */

    new_node->word[0] = insert_bits(new_node->word[0], new_operand_target, 2, 4);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_code, 5, 8);
    new_node->word[0] = insert_bits(new_node->word[0], new_operand_source, 9, 11);

    new_node->original_line = (char *) malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node->word);
        free(new_node);
        return NULL;
    }
    strcpy(new_node->original_line, line);

    new_node->L = L;
    new_node->ast = ast;
    new_node->next = NULL;
    new_node->been_here = FALSE;
    return new_node;
}

code_image *initialize_code_image() {
    code_image *new_code_image = malloc(sizeof(code_image));
    if (new_code_image == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_code_image);
        exit(1);
    }
    new_code_image->first = NULL;
    new_code_image->last = NULL;

    return new_code_image;
}

void add_code_node(code_image *code_image, code_node *new_node) {
    if (code_image->first == NULL) {
        code_image->first = new_node;
        code_image->last = new_node;
    } else {
        code_image->last->next = new_node;
        code_image->last = new_node;
    }
}

data_node *create_data_node(char *line, int L, ast ast) {
    int i;
    data_node *new_node = (data_node *) malloc(sizeof(data_node));
    if (new_node == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node);
        exit(1);
    }
    new_node->word = (int *) malloc(L * sizeof(int));
    if (new_node->word == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node->word);
        exit(1);
    }
    if (ast.ast_word.directive_word.directive_type == STRING_TYPE)
        for (i = 0; i < L; i++)
            new_node->word[i] = (int) (ast.ast_word.directive_word.directive_option.string[i]);
    else
        for (i = 0; i < L; i++)
            new_node->word[i] = ast.ast_word.directive_word.directive_option.machine_code.machine_code_array[i];

    new_node->original_line = (char *) malloc(strlen(line) + 1);
    if (new_node->original_line == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
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

data_image *initialize_data_image() {
    data_image *new_data_image = malloc(sizeof(data_image));
    if (new_data_image == NULL) {
        PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_data_image);
        exit(1);
    }
    new_data_image->first = NULL;
    new_data_image->last = NULL;

    return new_data_image;
}

void add_data_node(data_image *data_image, data_node *new_node) {
    if (data_image->first == NULL) {
        data_image->first = new_node;
        data_image->last = new_node;
    } else {
        data_image->last->next_node = new_node;
        data_image->last = new_node;
    }
}

code_node *find_code_node_by_line(code_image *code_image, char *line) {
    code_node *current;
    size_t line_length = strlen(line);
    if (line_length > 0 && line[line_length - 1] == '\n') {
        line[line_length - 1] = '\0';  /* Replace newline with null terminator */
    }
    current = code_image->first;
    while (current != NULL) {
        if (strcmp(current->original_line, line) == 0) {
            if(current->been_here==FALSE) {
                current->been_here=TRUE;
                return current;
            }
        }
        current = current->next;
    }
    return NULL;
}

void free_data_node(data_node *node) {
    if (node) {
        free(node->original_line);
        free(node->word);
        free(node);
    }
}

void free_code_node(code_node *node) {
    if (node) {
        free(node->original_line);
        free(node->word);
        free(node);
    }
}

void free_data_image(data_image *image) {
    if (image) {
        data_node *current = image->first;
        while (current) {
            data_node *temp = current;
            current = current->next_node;
            free_data_node(temp);
        }
        free(image);
    }
}

void free_code_image(code_image *image) {
    if (image) {
        code_node *current = image->first;
        while (current) {
            code_node *temp = current;
            current = current->next;
            free_code_node(temp);
        }
        free(image);
    }
}

void free_all_data_structures(code_image *code_img, data_image *data_img) {
    if (code_img) {
        free_code_image(code_img);
    }
    if (data_img) {
        free_data_image(data_img);
    }
}
