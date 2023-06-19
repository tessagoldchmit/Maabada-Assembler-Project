#include "first_pass.h"
#include "string.h"
#include "globals.h"
#include "tables.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>


bool is_symbol(char* line, char* char_ptr){
    char_ptr = strchr(line, ':');  // Find the first occurrence of ':'
    if (char_ptr != NULL)
        return TRUE;
    return FALSE;
}

bool is_data_string(char* line, char* char_ptr){
    char_ptr++;
    skip_spaces(char_ptr);
    if(strncmp(char_ptr, ".data", 5)==0){
        char_ptr+=5;
        return TRUE;
    }
    if(strncmp(char_ptr, ".string", 7)==0) {
        char_ptr+=7;
        return TRUE;
    }
    return FALSE;
}

void decode_data(symbol_type symbol_type, char *char_ptr) {

}

bool is_extern_entry(char *line, char *char_ptr) {
    return 0;
}

bool is_extern(char *line, char *char_ptr) {
    return 0;
}

void add_extern_symbols() {

}

int analyze_operands() {

}

void update_data_dc(symbol_table *symbol_table, long ic) {

}

int get_symbol_length(char line[80]) {
    // Find the index of the colon
    const char *colon = strchr(line, ':');

    // Calculate the length of the word
    int length = colon - line;

    return length;
}

bool first_pass_process(char *base_filename, long ic, long dc) {
    char *char_ptr=NULL;
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    bool symbol_flag = FALSE;
    symbol_table symbol_table;
    initialize_symbol_table(&symbol_table);
    char *symbol_name;
    bool error_flag = FALSE;
    int L;
    char *filename_with_am_suffix;

    /* Create filename with .am suffix */
    filename_with_am_suffix = malloc(strlen(base_filename) + 4); /* +4 for ".am\0" */
    strcpy(filename_with_am_suffix, base_filename);
    strcat(filename_with_am_suffix, ".am");

    // Open .am file
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", base_filename);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        char_ptr=line;
        if(is_symbol(line, char_ptr)){
            int symbol_length = get_symbol_length(line);  // Calculate the length of the string before ':'
            symbol_name[symbol_length + 1];  // Allocate a new string to store the symbol
            strncpy(symbol_name, line, symbol_length);  // Copy the characters before ':' to the symbol string
            symbol_name[symbol_length] = '\0';  // Null-terminate the symbol string
            symbol_flag = TRUE;
        }
        if(is_data_string(line, char_ptr)){
            if(symbol_flag){
                add_symbol(&symbol_table, symbol_name, dc, DATA);
            }
            decode_data(DATA, char_ptr);
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