#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include "ast.h"

int set_binary_value(int value) {
    int binary = 0;
    int bit_position = 0;

    while (bit_position < 12) {
        int bit = value & 1;  // Extract the least significant bit
        binary |= (bit << bit_position);  // Set the bit at the corresponding position
        value >>= 1;  // Right-shift the value to process the next bit
        bit_position++;  // Move to the next bit position
    }
    return binary;
}

int ascii_to_binary(char parameter) {
    int param_int_value=(int)parameter;
    return set_binary_value(param_int_value);
}

/*temp*/
void printBinary(int num) {
    int i;
    for (i = 11; i >= 0; i--) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

int* num_to_binary(char *parameters, int machine_code[], int binary_length) {
    char* token;
    int numbers[binary_length];
    int count = 0;
    int i;

    /* TODO extremly difficult mission replace all tokens with pointers*/
    // Tokenize the input string
    token = strtok(parameters, ",");
    while (token != NULL && count < binary_length) {
        // Convert token to integer and store in numbers array
        numbers[count] = atoi(token);
        count++;

        // Get next token
        token = strtok(NULL, ",");
    }
    for (i = 0; i < count; i++) {
        machine_code[i]=set_binary_value(numbers[i]);
        printBinary(machine_code[i]);
    }
    return machine_code;
}

directive* initialize_directive(char* label, directive_type kind, char* parameters, int binary_length) {
    directive* new_directive = malloc(sizeof(directive));
    /* TODO malloc that handles NULL memory problems */
    // Handle memory allocation failure
    if (new_directive == NULL) {
        return NULL;
    }

    // Allocate memory and copy label
    new_directive->label = malloc(strlen(label) + 1);
    if (new_directive->label == NULL) {
        // Handle memory allocation failure
        free(new_directive);
        return NULL;
    }
    strcpy(new_directive->label, label);

    //Assign kind
    new_directive->kind = kind;

    //Create machine code
    int new_machine_code[binary_length];
    int* array_ptr;
    int i;
    if(kind==STRING_TYPE) {
        for (i = 0; i < binary_length-1; i++) {
            new_machine_code[i] = ascii_to_binary(parameters[i]);
        }
        new_machine_code[binary_length-1] = ascii_to_binary('\0');
    }
    else if(kind==DATA_TYPE){
        array_ptr=num_to_binary(parameters, new_machine_code, binary_length);
        memcpy(new_directive->machine_code, array_ptr, binary_length * sizeof(int));
    }

    if(new_directive->machine_code == NULL) {
        // Handle memory allocation failure
        free(new_directive->label);
        free(new_directive);
        return NULL;
    }

    return new_directive;
}

group_a initialize_group_a_instruction(instruction_type kind, const char* source_operand, operand_type srctype, const char* target_operand, operand_type trgtype) {
    group_a new_ins_union_grpa;

    new_ins_union_grpa.kind = kind;

    new_ins_union_grpa.source_operand = malloc(strlen(source_operand) + 1);
    strcpy(new_ins_union_grpa.source_operand, source_operand);

    new_ins_union_grpa.srctype = srctype;

    new_ins_union_grpa.target_operand = malloc(strlen(target_operand) + 1);
    strcpy(new_ins_union_grpa.target_operand, target_operand);

    new_ins_union_grpa.trgtype = trgtype;

    return new_ins_union_grpa;
}

ast* initialize_ast(word_type word_type, union word word){
    ast* new_ast = malloc(sizeof(ast));
    new_ast->type = word_type;
    new_ast->word=word; /* TODO ask tessa about terminology */
    return new_ast;
}