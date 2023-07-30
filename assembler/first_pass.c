#include <string.h>
#include <stdio.h>

#include "first_pass.h"
#include "globals.h"
#include "ast.h"
#include "print_ast_utils.c"


void decode_data(ast ast_line_info, long *dc, data_image *my_data_image) {
    data_node *new_node;
    int binary_length = 0;
    int i=0;

    /* handle directive */
    if (ast_line_info.ast_word.directive_word.directive_type == STRING_TYPE) {
        while (ast_line_info.ast_word.directive_word.directive_option.string[i] != '\000') {
            i++;
            binary_length++;
        }
        binary_length++; /* for null terminating */
    } else if (ast_line_info.ast_word.directive_word.directive_type == DATA_TYPE) {
        binary_length = ast_line_info.ast_word.directive_word.directive_option.machine_code.machine_code_count;
    }

    *dc += binary_length; /* update memory length */

    new_node = create_data_node(binary_length, ast_line_info);
    add_data_node(my_data_image, new_node);
}


void update_data_dc(symbol_table *symbol_table, long ic) {

}


bool is_valid_instruction(char *char_ptr) {
    if (strncmp(char_ptr, "mov", 3) == 0 ||
        strncmp(char_ptr, "cmp", 3) == 0 ||
        strncmp(char_ptr, "add", 3) == 0 ||
        strncmp(char_ptr, "sub", 3) == 0 ||
        strncmp(char_ptr, "lea", 3) == 0 ||
        strncmp(char_ptr, "not", 3) == 0 ||
        strncmp(char_ptr, "clr", 3) == 0 ||
        strncmp(char_ptr, "dec", 3) == 0 ||
        strncmp(char_ptr, "jmp", 3) == 0 ||
        strncmp(char_ptr, "bne", 3) == 0 ||
        strncmp(char_ptr, "red", 3) == 0 ||
        strncmp(char_ptr, "prn", 3) == 0 ||
        strncmp(char_ptr, "jsr", 3) == 0 ||
        strncmp(char_ptr, "rts", 3) == 0 ||
        strncmp(char_ptr, "stop", 4) == 0) {
        return TRUE;
    }
    return FALSE;
}

bool first_pass_process(char *filename_with_am_suffix, long ic, long dc, data_image *my_data_image,
                        code_image *my_code_image, symbol_table *symbol_table) {
    FILE *am_file;
    char line[MAX_LINE_LENGTH];
    bool symbol_flag = FALSE;
    bool error_flag = FALSE;
    int L;

    /* Open .am file */
    am_file = fopen(filename_with_am_suffix, "r");
    if (am_file == NULL) {
        fprintf(stderr, "Error: Failed to open .am file '%s' for reading.\n", filename_with_am_suffix);
        return FALSE;
    }

    /* Process each line of the source file */
    while (fgets(line, MAX_LINE_LENGTH, am_file)) {
        printf("\n------------------------------------------------------------------------------\n");
        printf(line);
        ast ast_line_info = get_ast_line_info(line);
        print_ast(&ast_line_info);

        if (ast_line_info.ast_symbol[0] != '\0') {
            symbol_flag = TRUE;
        }
        if (ast_line_info.ast_word.directive_word.directive_type == DATA_TYPE ||
            ast_line_info.ast_word.directive_word.directive_type == STRING_TYPE) {
            if (symbol_flag) {
                add_symbol(symbol_table, ast_line_info.ast_symbol, dc, DATA);
            }
            decode_data(ast_line_info, &dc, my_data_image);
        } else if (ast_line_info.ast_word.directive_word.directive_type == EXTERN_TYPE ||
                   ast_line_info.ast_word.directive_word.directive_type == ENTRY_TYPE) {

        } else if (symbol_flag) {
            add_symbol(symbol_table, ast_line_info.ast_symbol, ic, CODE);
            if (is_valid_instruction(line)) {
//                L = analyze_operands(line, my_code_image);
//                ic += L;
            }
        }
        symbol_flag = FALSE;
    }
    print_symbol_table(symbol_table);
    if (error_flag) {
        printf("## First pass encountered errors.\n");
        return FALSE;
    } else {
        update_data_dc(symbol_table, ic);
        printf("## First pass completed successfully.\n");
        return TRUE;
    }
}
