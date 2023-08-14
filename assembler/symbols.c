#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbols.h"
#include "logs.h"


bool is_symbol(char *line) {
    return strchr(line, ':') != NULL;
}

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
    if (new_symbol == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_symbol);
        exit(1);
    }

    /* Allocate memory for the symbol_name and copy the string */
    new_symbol->symbol_name = (char *) malloc(strlen(symbol_name) + 1);
    if (new_symbol->symbol_name == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_symbol);
        exit(1);
    }
    strcpy(new_symbol->symbol_name, symbol_name);

    new_symbol->decimal_address = decimal_address;
    new_symbol->symbol_type = symbol_type;
    new_symbol->next_symbol = NULL;
    return new_symbol;
}

symbol_table *initialize_symbol_table() {
    symbol_table *new_symbol_table = malloc(sizeof(symbol_table));
    if (new_symbol_table == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_symbol_table);
        exit(1);
    }
    new_symbol_table->first = NULL;
    new_symbol_table->last = NULL;

    return new_symbol_table;
}

bool add_symbol(symbol_table *table, char *symbol_name, int *decimal_address, symbol_type symbol_type) {
    /* Check if the symbol already exists in the table */
    symbol_node *current;
    symbol_node *new_symbol;
    current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            /* Symbol already exists, return false */
            return FALSE;
        }
        current = current->next_symbol;
    }

    /* Create a new symbol node */
    if (symbol_type == EXTERNAL)
        new_symbol = create_symbol(symbol_name, EXTERN_DECIMAL_ADDRESS, symbol_type);
    else
        new_symbol = create_symbol(symbol_name, *decimal_address, symbol_type);

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

bool check_entry_symbol_duplication(symbol_table *table, char *symbol_name) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            if(current->symbol_type == EXTERNAL) {
                return FALSE;
            }
        }
        current = current->next_symbol;
    }
    return TRUE;
}

void mark_symbol_as_entry(symbol_table *table, char *symbol_name) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            current->symbol_type = ENTRY;
            return;
        }
        current = current->next_symbol;
    }
}

int get_symbol_address(symbol_table *table, symbol symbol_name) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol_name) == 0) {
            if (current->decimal_address == -1) return -1;
            return current->decimal_address;
        }
        current = current->next_symbol;
    }
    return NON_EXIST_SYMBOL_ADDRESS;
}

bool has_entry_symbol(symbol_table *table) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (current->symbol_type == ENTRY) {
            return TRUE;
        }
        current = current->next_symbol;
    }
    return FALSE;
}

bool has_extern_symbol(symbol_table *table) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (current->symbol_type == EXTERNAL) {
            return TRUE;
        }
        current = current->next_symbol;
    }
    return FALSE;
}

bool is_symbol_extern(symbol_table *table, symbol symbol) {
    symbol_node *current = table->first;
    while (current != NULL) {
        if (strcmp(current->symbol_name, symbol)==0) {
            if(current->symbol_type == EXTERNAL)
                return TRUE;
        }
        current = current->next_symbol;
    }
    return FALSE;
}

extern_table *initialize_extern_table() {
    extern_table *new_extern_table = malloc(sizeof(extern_table));
    if (new_extern_table == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_extern_table);
        exit(1);
    }
    new_extern_table->first = NULL;
    new_extern_table->last = NULL;

    return new_extern_table;
}

void add_extern_node(extern_table *table, char *symbol_name, int address) {
    extern_node *new_node = (extern_node *)malloc(sizeof(extern_node));
    if (new_node == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        exit(EXIT_FAILURE);
    }

    /* Allocate memory for the symbol_name and copy the string */
    new_node->symbol_name = (char *)malloc(strlen(symbol_name) + 1);
    if (new_node->symbol_name == NULL) {
         PRINT_MESSAGE(ERROR_MSG_TYPE, ERROR_FAILED_TO_ALLOCATE_MEM);
        free(new_node);
        exit(EXIT_FAILURE);
    }
    strcpy(new_node->symbol_name, symbol_name);

    new_node->address = address;
    new_node->next = NULL;

    if (table->first == NULL) {
        table->first = new_node;
        table->last = new_node;
    } else {
        table->last->next = new_node;
        table->last = new_node;
    }
}


void free_symbol_node(symbol_node *node) {
    if (node != NULL) {
        free(node->symbol_name);
        free(node);
    }
}

void free_symbol_table(symbol_table *table) {
    symbol_node *current = table->first;
    while (current != NULL) {
        symbol_node *temp = current;
        current = current->next_symbol;
        free_symbol_node(temp);
    }
    free(table);
}

void free_extern_node(extern_node *node) {
    if (node != NULL) {
        free(node->symbol_name);
        free(node);
    }
}

void free_extern_table(extern_table *table) {
    extern_node *current = table->first;
    while (current != NULL) {
        extern_node *temp = current;
        current = current->next;
        free_extern_node(temp);
    }
    free(table);
}

void free_symbols(symbol_table *symbol_table, extern_table *extern_table) {
    free_symbol_table(symbol_table);
    free_extern_table(extern_table);
}
