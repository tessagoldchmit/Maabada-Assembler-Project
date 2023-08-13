#ifndef DATA_STRUCTURES_H
#define DATA_STRUCTURES_H

#include "ast.h"

/**
 * Represents a node for handling instructions in the code image.
 */
typedef struct code_node {
    char *original_line; /* The original line from which the code node is created. */
    int *word; /* Array of binary machine code representing instructions. */
    int L; /* Size of the array of binary machine code. */
    ast ast; /* Abstract syntax tree associated with the code node. */
    struct code_node *next; /* Pointer to the next code node in the linked list. */
} code_node;

/**
 * Represents a collection of code nodes, forming an image of instructions.
 */
typedef struct {
    code_node *first; /* Pointer to the first code node in the linked list. */
    code_node *last; /* Pointer to the last code node in the linked list. */
} code_image;

/**
 * Represents a node for storing data in the data image.
 */
typedef struct data_node {
    char *original_line; /* The original line from which the data node is created. */
    int *word; /* Array of binary machine code representing data. */
    int L; /* Size of the array of binary machine code. */
    ast ast; /* Abstract syntax tree associated with the data node. */
    struct data_node *next_node; /* Pointer to the next data node in the linked list. */
} data_node;

/**
 * Represents a collection of data nodes, forming an image of data storage.
 */
typedef struct {
    data_node *first; /* Pointer to the first data node in the linked list. */
    data_node *last; /* Pointer to the last data node in the linked list. */
} data_image;

/**
    Creates a code node with specified attributes for handling instructions.

    @param line The original line from which the code node is being created.
    @param L The length of the word array (i.e. how many words are there in the memory for this operation) in the code node.
    @param ast The abstract syntax tree associated with the code node.
    @param new_operand_code The new operand code to be inserted.
    @param new_operand_target The new operand target to be inserted.
    @param new_operand_source The new operand source to be inserted.
    @return A pointer to the created code node.
*/
code_node *create_code_node(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

/**
    Creates a code node for handling register-based instructions with specific operands.
    *This function already binaring all the words

    @param line The original line from which the code node is being created.
    @param L The length of the word array (i.e. how many words are there in the memory for this operation) in the code node.
    @param ast The abstract syntax tree associated with the code node.
    @param new_operand_code The new operand code to be inserted.
    @param new_operand_target The new operand target to be inserted.
    @param new_operand_source The new operand source to be inserted.
    @return A pointer to the created code node.
*/
code_node *create_code_node_registers(char* line, int L, ast ast, int new_operand_code, int new_operand_target, int new_operand_source);

/**
 * Initialize an empty code image.
 *
 * @param code_image The code image to initialize.
 */
code_image *initialize_code_image();

/**
    Adds a new code node to the code image.

    @param code_image The code image where the new node will be added.
    @param new_node The new code node to be added.
*/
void add_code_node(code_image *code_image, code_node *new_node);

/**
    Creates a data node with specified attributes for storing data.

    @param line The original line from which the data node is being created.
    @param L The length of the word array (i.e. how many words are there in the memory for this operation) in the code node.
    @param ast The abstract syntax tree associated with the data node.
    @return A pointer to the created data node.
*/
data_node *create_data_node(char* line, int L, ast ast);

/**
 * Initialize an empty data image.
 *
 * @param data_image The data image to initialize.
 */
data_image *initialize_data_image();

/**
    Adds a new data node to the data image.

    @param data_image The data image where the new node will be added.
    @param new_node The new data node to be added.
*/
void add_data_node(data_image *data_image, data_node *new_node);

/**
    Inserts a specified number of bits into a given value at the specified range of bits.

    @param value The original value where bits will be inserted.
    @param num The number to be inserted (bits will be taken from its binary representation).
    @param start_bit The starting bit position for insertion.
    @param end_bit The ending bit position for insertion.
    @return The updated value after inserting the bits.
*/
unsigned short insert_bits(unsigned short value, int num, int start_bit, int end_bit);

/**
    Finds a code node in the code image based on the original line content.

    @param code_image The code image to search for the code node.
    @param line The original line content to search for.
    @return A pointer to the found code node, or NULL if not found.
*/
code_node* find_code_node_by_line(code_image* code_image, char* line);

/**
    Frees the memory occupied by code and data image structures.

    This function releases the memory allocated for the provided code image and data image structures,
    along with any associated resources.

    @param code_img A pointer to the code image structure to be freed.
    @param data_img A pointer to the data image structure to be freed.
*/
void free_all_data_structures(code_image *code_img, data_image *data_img);

#endif /* DATA_STRUCTURES_H */
