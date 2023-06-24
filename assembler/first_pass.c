#include "first_pass.h"
#include "globals.h"
#include "data_structures.h"
#include "utils.h"
#include <string.h>
#include <stdio.h>
#include <malloc.h>


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

void decode_data(symbol_type symbol_type, char *char_ptr) {

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


bool first_pass_process(char *filename_with_am_suffix, long ic, long dc) {
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    bool symbol_flag = FALSE;
    symbol_table symbol_table;
    initialize_symbol_table(&symbol_table);
    bool error_flag = FALSE;
    int L;

    // Open .am file
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", filename_with_am_suffix);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        char* char_ptr = line;
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
            } else if (strncmp(char_ptr, ".string", 7) == 0) {
                char_ptr += strlen(".string");
            }
            if(symbol_flag){
                add_symbol(&symbol_table, symbol_name, dc, DATA);
            }
            decode_data(DATA, char_ptr);  // Todo what is this
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