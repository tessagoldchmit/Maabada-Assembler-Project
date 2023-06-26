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
    if (strncmp(line, ".data", 5) == 0 || strncmp(line, ".string", 7) == 0)
        return TRUE;
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
    directive *new_directive;
    data_node *new_node;
    int binary_length=0;
    char *handler;
    char parameters[MAX_LINE_LENGTH]; /* TODO NOT SURE ABOUT THE LENGTH, MUST BE INITIALIZED HERE BECAUSE OF SEGFAULT*/

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
        strncpy(parameters, handler, binary_length); /* save string parameters */
    }
    else{
        strcpy(parameters, handler); /* save number parameters */
    }

    if(directive_type==STRING_TYPE)
        new_directive = initialize_directive(symbol_name, STRING_TYPE, parameters, binary_length);
    else if(directive_type==DATA_TYPE)
        new_directive = initialize_directive(symbol_name, DATA_TYPE, parameters, binary_length);
    word directive_union_word;
    directive_union_word.directive_word=*new_directive;
    ast *ast = initialize_ast(DIRECTIVE, directive_union_word);
    new_node=create_data_node(binary_length, *ast);
    add_data_node(my_data_image, new_node);
}

bool is_extern_entry(char *line, char *char_ptr) {
    if (strncmp(char_ptr, ".entry", 6) == 0 || strncmp(char_ptr, ".extern", 7) == 0)
        return TRUE;
    return FALSE;
}

bool is_extern(char *line, char *char_ptr) {
    if (strncmp(line, ".extern", 7) == 0)
        return TRUE;
    return FALSE;
}

int check_group(instruction_type kind) {
    /* TODO enum groups a, b, c*/
    if( kind==MOV_TYPE ||
        kind==CMP_TYPE ||
        kind==ADD_TYPE ||
        kind==SUB_TYPE ||
        kind==LEA_TYPE){
        return 1;
    }
    else if(kind==NOT_TYPE ||
            kind==CLR_TYPE ||
            kind==DEC_TYPE ||
            kind==JMP_TYPE ||
            kind==BNE_TYPE ||
            kind==RED_TYPE ||
            kind==PRN_TYPE ||
            kind==JSR_TYPE){
        return 2;
    }
    else if(kind==RTS_TYPE ||
            kind==STOP_TYPE){
        return 3;
    }
}

instruction_type check_instruction_type(char* char_ptr) {
    if(strncmp(char_ptr, "mov", 3)==0)
        return MOV_TYPE;
    if(strncmp(char_ptr, "cmp", 3)==0)
        return CMP_TYPE;
    if(strncmp(char_ptr, "add", 3)==0)
        return ADD_TYPE;
    if(strncmp(char_ptr, "sub", 3)==0)
        return SUB_TYPE;
    if(strncmp(char_ptr, "lea", 3)==0)
        return LEA_TYPE;
    if(strncmp(char_ptr, "not", 3)==0)
        return NOT_TYPE;
    if(strncmp(char_ptr, "clr", 3)==0)
        return CLR_TYPE;
    if(strncmp(char_ptr, "dec", 3)==0)
        return DEC_TYPE;
    if(strncmp(char_ptr, "jmp", 3)==0)
        return JMP_TYPE;
    if(strncmp(char_ptr, "bne", 3)==0)
        return BNE_TYPE;
    if(strncmp(char_ptr, "red", 3)==0)
        return RED_TYPE;
    if(strncmp(char_ptr, "prn", 3)==0)
        return PRN_TYPE;
    if(strncmp(char_ptr, "jsr", 3)==0)
        return JSR_TYPE;
    if(strncmp(char_ptr, "rts", 3)==0)
        return RTS_TYPE;
    if(strncmp(char_ptr, "stop", 4)==0)
        return STOP_TYPE;
}

void copy_string_until_space(char *source, char *destination, char delimiter, size_t dest_size) {
    size_t i = 0;

    while (source[i] != '\0' && source[i] != delimiter && i < dest_size - 1) {
        destination[i] = source[i];
        i++;
    }

    destination[i] = '\0'; // Null-terminate the copied string
}

int analyze_operands(char *char_ptr, code_image *my_code_image) {
    instruction_type kind;
    char* source_operand;
    operand_type srctype;
    char* target_operand;
    operand_type trgttype;
    union instruction_union instruction_union_word;
    instruction *new_instruction;
    char *backup;
    word new_word;

    kind=check_instruction_type(char_ptr);
    char_ptr+=4;
    skip_spaces(char_ptr);

    if(check_group(kind)==1) {
        backup = malloc(strlen(char_ptr) + 1);
        strcpy(backup, char_ptr);
        source_operand=strtok(backup, ",");
        if(strncmp(source_operand, "@r", 2)==0) {
            srctype = REGISTER_TYPE;
            char_ptr+=3;
        }
        else if(isalpha(*char_ptr)) {
            srctype = LABEL_TYPE;
        }
        else {
            srctype = NUMBER_TYPE;
        }

        skip_spaces(char_ptr);
        char_ptr+=2; //skip comma

        backup = malloc(strlen(char_ptr) + 1);
        strcpy(backup, char_ptr);
        target_operand=strtok(backup, ",");
        if(strncmp(target_operand, "@r", 2)==0)
            trgttype=REGISTER_TYPE;
        else if(isalpha(*char_ptr))
            trgttype=LABEL_TYPE;
        else
            trgttype=NUMBER_TYPE;

        /* TODO fix the constrction here*/
        instruction_union_word.group_a = initialize_group_a_instruction(kind, source_operand, srctype, target_operand, trgttype);
        new_instruction->instruction_union=instruction_union_word;
        new_word.instruction_word = *new_instruction;
        ast *ast = initialize_ast(INSTRUCTION, new_word);
    }
    else if(check_group(kind)==2) {
        instruction_union_word.group_b;
    }
    else if(check_group(kind)==3) {
        instruction_union_word.group_c;
    }
}

void update_data_dc(symbol_table *symbol_table, long ic) {

}


bool is_valid_ins(char *char_ptr) {
    /* TODO relocate to one dedicated container */
    if( strncmp(char_ptr, "mov", 3) ||
        strncmp(char_ptr, "cmp", 3) ||
        strncmp(char_ptr, "add", 3) ||
        strncmp(char_ptr, "sub", 3) ||
        strncmp(char_ptr, "lea", 3) ||
        strncmp(char_ptr, "not", 3) ||
        strncmp(char_ptr, "clr", 3) ||
        strncmp(char_ptr, "dec", 3) ||
        strncmp(char_ptr, "jmp", 3) ||
        strncmp(char_ptr, "bne", 3) ||
        strncmp(char_ptr, "red", 3) ||
        strncmp(char_ptr, "prn", 3) ||
        strncmp(char_ptr, "jsr", 3) ||
        strncmp(char_ptr, "rts", 3) ||
        strncmp(char_ptr, "stop", 4)){
        return TRUE;
    }
    return FALSE;
}

bool first_pass_process(char *filename_with_am_suffix, long ic, long dc, data_image *my_data_image, code_image *my_code_image, symbol_table symbol_table) {
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
            if (strncmp(char_ptr, ".extern", 7) == 0) {
                char_ptr += strlen(".extern");
                directive_type = EXTERN_TYPE;
                add_symbol(&symbol_table, char_ptr, -1, EXTERNAL);
                /* TODO what the fuck is happening here */
                /* TODO we have TEST as symbol, HEY as an external symbol. what goes into symbol table? what not? */
            }
        }
        else if(symbol_flag){
            add_symbol(&symbol_table, symbol_name, ic, CODE);
            if(is_valid_ins(char_ptr)){
                L = analyze_operands(char_ptr, my_code_image);
                ic+=L;
            }
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