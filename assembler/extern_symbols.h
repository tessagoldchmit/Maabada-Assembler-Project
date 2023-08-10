#ifndef MMN14_EXTERN_SYMBOLS_H
#define MMN14_EXTERN_SYMBOLS_H

typedef struct extern_node {
    char *symbol_name;
    int address;
    struct extern_node *next;
} extern_node;

typedef struct extern_table {
    extern_node *first;
    extern_node *last;
} extern_table;

extern_table *initialize_extern_table();
void add_extern_node(extern_table *table, char *symbol_name, int address);

#endif /* MMN14_EXTERN_SYMBOLS_H */
