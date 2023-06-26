#include "first_pass.h"
#include "globals.h"
#include "data_structures.h"
#include "ast.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <ctype.h>


bool is_symbol(char* line){
    return strchr(line, ':') != NULL;
}

bool is_data_or_string(char* line) {
    /* Check if the line represents either ".data" or ".string" type */
    if (strncmp(line, ".data", 5) == 0 || strncmp(line, ".string", 7) == 0) {
        return TRUE;
    }
    return FALSE;
}

int count_data_numbers(const char* str) {
    int count = 0;
    int len = strlen(str);
    int i = 0;
    int in_number = 0;

    while (i < len) {
        // Skip leading whitespace
        while (i < len && isspace(str[i])) {
            i++;
        }

        // Check if a number is encountered
        if (isdigit(str[i]) || (str[i] == '-' && isdigit(str[i+1]))) {
            if (!in_number) {
                count++;
                in_number = 1;
            }
        } else {
            in_number = 0;
        }

        i++;
    }

    return count;
}

void decode_data(directive_type directive_type, char *char_ptr, long *dc, char *symbol_name, data_image *my_data_image) {
    directive *directive;
    data_node *new_node;
    int binary_length=0;
    char *handler;
    char parameters[MAX_LINE_LENGTH]; /* NOT SURE ABOUT THE LENGTH, MUST BE INITIALIZED HERE BECAUSE OF SEGFAULT*/

    if(directive_type==STRING_TYPE) {
        char_ptr++; /* skip the first " */
        handler = char_ptr;
        while (*char_ptr != '\"') {
            char_ptr++;
            binary_length++;
        }
        binary_length++; //for null terminating
    }
    else if(directive_type==DATA_TYPE){
        handler = char_ptr;
        binary_length=count_data_numbers(char_ptr);
    }

    *dc+=binary_length; /* update memory length */

    if(directive_type==STRING_TYPE){
        *dc++;
        strncpy(parameters, handler, binary_length); /* save string parameters */
    }
    else{
        strcpy(parameters, handler); /* save number parameters */
    }

    if(directive_type==STRING_TYPE)
        directive = initialize_directive(symbol_name, STRING_TYPE, parameters, binary_length);
    else if(directive_type==DATA_TYPE)
        directive = initialize_directive(symbol_name, DATA_TYPE, parameters, binary_length);
    union word directive_union_word;
    directive_union_word.directive_word=*directive;
    ast *ast = initialize_ast(DIRECTIVE, directive_union_word);
    new_node=create_data_node(binary_length, *ast);
    add_data_node(my_data_image, new_node);
}

bool is_extern_entry(char *line, char *char_ptr) {
    return FALSE;
}

bool is_extern(char *line, char *char_ptr) {
    return FALSE;
}

void add_extern_symbols() {

}

int analyze_operands() {

}

void update_data_dc(symbol_table *symbol_table, long ic) {

}


bool first_pass_process(char *filename_with_am_suffix, long ic, long dc, data_image *my_data_image, code_image code_image, symbol_table symbol_table) {
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    bool symbol_flag = FALSE;
    bool error_flag = FALSE;
    int L;
    directive_type directive_type;


    // Open .am file
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", filename_with_am_suffix);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        char *char_ptr = line;
        char *symbol_name;

        if (is_symbol(char_ptr)) {
            char* colon_ptr = strchr(line, ':');
            int symbol_length = colon_ptr - char_ptr;  /* Calculate the length of the string before ':' */
            symbol_name = malloc(symbol_length + 1);  /* Allocate memory for symbol_name */
            strncpy(symbol_name, line, symbol_length);  /* Copy the characters before ':' to the symbol string */
            symbol_name[symbol_length] = '\0';  /* Null-terminate the symbol string */
            symbol_flag = TRUE;
            char_ptr = colon_ptr + 1; /* Move char_ptr to the character after ':' */
        }

        char_ptr = skip_spaces(char_ptr);  /* Update char_ptr to skip leading spaces */

        if(is_data_or_string(char_ptr)){
            if (strncmp(char_ptr, ".data", 5) == 0) {
                char_ptr += strlen(".data");
                directive_type = DATA_TYPE;
            } else if (strncmp(char_ptr, ".string", 7) == 0) {
                char_ptr += strlen(".string");
                directive_type = STRING_TYPE;
            }
            if(symbol_flag){
                add_symbol(&symbol_table, symbol_name, dc, DATA);
            }
            char_ptr=skip_spaces(char_ptr);
            decode_data(directive_type, char_ptr, &dc, symbol_name, my_data_image);
        }
        else if(is_extern_entry(line, char_ptr)){
            if(is_extern(line, char_ptr)){
                add_extern_symbols();
            }
        }
        else if(symbol_flag){
            add_symbol(&symbol_table, symbol_name, ic, CODE);
            L = analyze_operands();
            ic+=L;
        }

        symbol_flag = FALSE;
    }
    if(error_flag)
        return FALSE;
    else{
        update_data_dc(&symbol_table, ic);
        return TRUE;
    }
}