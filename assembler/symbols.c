#include "symbols.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

const char *reserved_words[] = {
        "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7",
        "and", "mov", "cmp", "add", "sub", "not", "clr", "lea",
        "inc", "dec", "jmp", "bne", "red", "prn", "jsr", "rts", "stop"
};

/**
 * Create a new symbol node.
 *
 * @param symbol_name The name of the symbol.
 * @param decimal_address The decimal address associated with the symbol.
 * @param symbol_type The ast_word_type of the symbol (DATA / EXTERNAL / CODE).
 * @return A pointer to the newly created symbol node.
 */
symbol_node *create_symbol(char *symbol_name, int decimal_address, symbol_type symbol_type) {
    symbol_node *new_symbol = (symbol_node *) malloc(sizeof(symbol_node));
    new_symbol->symbol_name = strdup(symbol_name);
    new_symbol->decimal_address = decimal_address;
    new_symbol->symbol_type = symbol_type;
    new_symbol->next_symbol = NULL;
    return new_symbol;
}

/**
 * Initialize an empty symbol table.
 *
 * @param symbol_table The symbol table to initialize.
 */
symbol_table *initialize_symbol_table() {
    symbol_table *new_symbol_table = malloc(sizeof(symbol_table));
    new_symbol_table->first = NULL;
    new_symbol_table->last = NULL;

    return new_symbol_table;
}

/**
 * Add a symbol to the symbol table.
 *
 * @param table The symbol table.
 * @param symbol_name The name of the symbol to add.
 * @param decimal_address The decimal address associated with the symbol.
 * @param symbol_type The ast_word_type of the symbol (DATA / EXTERNAL / CODE).
 * @return True if the symbol was successfully added, False if the symbol already exists in the table.
 */
bool add_symbol(symbol_table *table, char *symbol_name, int decimal_address, symbol_type symbol_type) {
    /* Check if the symbol already exists in the table */
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            /* Symbol already exists, return false */
            return FALSE;
        }
        current = current->next_symbol;
    }

    /* Create a new symbol node */
    symbol_node *new_symbol = create_symbol(symbol_name, decimal_address + 100, symbol_type);

    if (table->first == NULL) {
        /* If the symbol table is empty, set the new symbol as both head and tail */
        table->first = new_symbol;
        table->last = new_symbol;
    } else {
        /* Otherwise, add the new symbol at the end and update the tail */
        table->last->next_symbol = new_symbol;
        table->last = new_symbol;
    }
    return TRUE;
}

/**
 * Checks if a symbol is valid, satisfying the following conditions:
 * - Length is within the maximum symbol length limit.
 * - Does not match any reserved words.
 * - Starts with an alphabetic character.
 * - Contains only alphanumeric characters.
 *
 * @param symbol_name The symbol to check
 * @return true if the symbol is valid, false otherwise
*/
bool is_symbol_valid(char *symbol_name) {
    int i;

    /* Length is within the maximum symbol length limit */
    if (strlen(symbol_name) > MAX_SYMBOL_LENGTH) {
        return FALSE;
    }

    /* Does not match any reserved words */
    for (i = 0; reserved_words[i] != NULL; i++) {
        if (strcmp(symbol_name, reserved_words[i]) == 0) {
            return FALSE;
        }
    }

    /* Starts with an alphabetic character */
    if (!isalpha(symbol_name[0])) {
        return FALSE;
    }

    /* Contains only alphanumeric characters */
    for (i = 0; symbol_name[i] != '\0'; i++) {
        if (!isalnum(symbol_name[i])) {
            return FALSE;
        }
    }

    return TRUE;
}


void print_symbol_table(symbol_table *table) {
    symbol_node *current = table->first;

    printf("-------------------------------------------------------\n");
    printf("| %-15s | %-15s | %-15s |\n", "Symbol Name", "Decimal Address", "Symbol Type");
    printf("-------------------------------------------------------\n");

    while (current != NULL) {
        printf("| %-15s | %-15d | ", current->symbol_name, current->decimal_address);

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