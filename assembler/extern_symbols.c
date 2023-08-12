#include <stdlib.h>
#include <string.h>
#include "stdio.h"
#include "extern_symbols.h"

extern_table *initialize_extern_table() {
    extern_table *new_extern_table = malloc(sizeof(extern_table));
    new_extern_table->first = NULL;
    new_extern_table->last = NULL;

    return new_extern_table;
}

void add_extern_node(extern_table *table, char *symbol_name, int address) {
    extern_node *new_node = (extern_node *)malloc(sizeof(extern_node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    new_node->symbol_name = strdup(symbol_name);
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
