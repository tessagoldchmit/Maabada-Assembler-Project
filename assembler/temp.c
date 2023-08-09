#include <stdio.h>
#include "temp.h"
#include "symbols.h"
#include "ast.h"


void print_binary_12bits(int num) {
    int i=0;
    for (i = 11; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }
}

void print_symbol_table(symbol_table *table) {
    symbol_node *current = table->first;

    printf("-------------------------------------------------------\n");
    printf("| %-15s | %-15s | %-15s |\n", "Symbol Name", "Decimal Address", "Symbol Type");
    printf("-------------------------------------------------------\n");

    while (current != NULL) {
        if(current->symbol_type == EXTERNAL)
            printf("| %-15s | %-15d | ", current->symbol_name, EXTERN_DECIMAL_ADDRESS);
        else
            printf("| %-15s | %-15d | ", current->symbol_name, current->decimal_address + START_OF_MEMORY_ADDRESS);

        switch (current->symbol_type) {
            case DATA:
                printf("%-15s |\n", "DATA");
                break;
            case EXTERNAL:
                printf("%-15s |\n", "EXTERNAL");
                break;
            case CODE:
                printf("%-15s |\n", "CODE");
                break;
            default:
                printf("%-15s |\n", "Unknown");
                break;
        }

        current = current->next_symbol;
    }

    printf("-------------------------------------------------------\n");
}



const char* operand_type_strings[] = {
        "ERROR_OPERAND_TYPE",
        "REGISTER_OPERAND_TYPE",
        "SYMBOL_OPERAND_TYPE",
        "NUMBER_OPERAND_TYPE"
};

void print_ast_directive(const directive *directive_node) {
    int i;
    switch (directive_node->directive_type) {
        case DATA_TYPE:
            printf("Directive Type - DATA\n");
            printf("Data Values: ");
            for (i = 0; i < directive_node->directive_option.machine_code.machine_code_count; i++) {
                printf("%d ", directive_node->directive_option.machine_code.machine_code_array[i]);
            }
            printf("\n");
            break;

        case STRING_TYPE:
            printf("Directive Type - STRING\n");
            printf("String Value: %s\n", directive_node->directive_option.string);
            break;

        case ENTRY_TYPE:
            printf("Directive Type - ENTRY\n");
            printf("Entry Symbol: %s\n", directive_node->directive_option.symbol);
            break;

        case EXTERN_TYPE:
            printf("Directive Type - EXTERN\n");
            printf("Extern Symbol: %s\n", directive_node->directive_option.symbol);
            break;

        default:
            printf("Unknown directive type\n");
    }
}

void print_ast_instruction(const instruction *instruction_node) {
    /* Print instruction details */
    printf("Instruction Name: %s\n", instructions_map[instruction_node->instruction_name].name);

    /* Print instruction union based on the instruction name */
    switch (check_group(instruction_node->instruction_name)) {
        case GROUP_A:
            printf("Source Operand Type: %s\n", operand_type_strings[instruction_node->instruction_union.group_a.source_type]);
            printf("Source Operand Value: ");
            if (instruction_node->instruction_union.group_a.source_type == REGISTER_OPERAND_TYPE) {
                printf("Register: %c\n", instruction_node->instruction_union.group_a.source_value.register_num);
            } else if (instruction_node->instruction_union.group_a.source_type == SYMBOL_OPERAND_TYPE) {
                printf("Symbol: %s\n", instruction_node->instruction_union.group_a.source_value.symbol);
            } else if (instruction_node->instruction_union.group_a.source_type == NUMBER_OPERAND_TYPE) {
                printf("Number: %d\n", instruction_node->instruction_union.group_a.source_value.number);
            }

            printf("Target Operand Type: %s\n", operand_type_strings[instruction_node->instruction_union.group_a.target_type]);
            printf("Target Operand Value: ");
            if (instruction_node->instruction_union.group_a.target_type == REGISTER_OPERAND_TYPE) {
                printf("Register: %c\n", instruction_node->instruction_union.group_a.target_value.register_num);
            } else if (instruction_node->instruction_union.group_a.target_type == SYMBOL_OPERAND_TYPE) {
                printf("Symbol: %s\n", instruction_node->instruction_union.group_a.target_value.symbol);
            } else if (instruction_node->instruction_union.group_a.target_type == NUMBER_OPERAND_TYPE) {
                printf("Number: %d\n", instruction_node->instruction_union.group_a.target_value.number);
            }
            break;

        case GROUP_B:
            printf("Target Operand Type: %s\n", operand_type_strings[instruction_node->instruction_union.group_b.target_type]);
            printf("Target Operand Value: ");
            if (instruction_node->instruction_union.group_b.target_type == REGISTER_OPERAND_TYPE) {
                printf("Register: %c\n", instruction_node->instruction_union.group_b.target_value.register_num);
            } else if (instruction_node->instruction_union.group_b.target_type == SYMBOL_OPERAND_TYPE) {
                printf("Symbol: %s\n", instruction_node->instruction_union.group_b.target_value.symbol);
            } else if (instruction_node->instruction_union.group_b.target_type == NUMBER_OPERAND_TYPE) {
                printf("Number: %d\n", instruction_node->instruction_union.group_b.target_value.number);
            }
            break;

        case GROUP_C:
            break;

        default:
            printf("Unknown instruction group\n");
    }
}

void print_ast(ast *ast_line_info) {
    if (ast_line_info == NULL) {
        return;
    }

    /* Print the symbol (if present) */
    if (ast_line_info->ast_symbol[0] != '\0') {
        printf("Symbol: %s\n", ast_line_info->ast_symbol);
    }

    /* Print the word type */
    printf("Word Type: ");
    switch (ast_line_info->ast_word_type) {
        case INSTRUCTION:
            printf("Instruction\n");
            print_ast_instruction(&ast_line_info->ast_word.instruction_word);
            break;
        case DIRECTIVE:
            print_ast_directive(&ast_line_info->ast_word.directive_word);
            break;
        case ERROR:
            printf("Error\n");
            printf("Error String: Line %d | %s\n", ast_line_info->line_number, ast_line_info->ast_word.error_word);
            break;
        default:
            printf("Unknown\n");
    }
}