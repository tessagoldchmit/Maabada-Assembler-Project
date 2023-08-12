#ifndef PRE_ASSEMBLER_H
#define PRE_ASSEMBLER_H

#include "globals.h"

#define MAX_MACRO_LENGTH 20
#define MAX_MACRO_INST 100 /* Maximum length of content assumed. */

/**
 * strings that represent start and end of macros
 */
#define MACRO_START "mcro"
#define MACRO_END "endmcro"

/**
 * Represents a macro, consisting of content and a name.
 */
typedef struct {
    char *content;  /* The content of the macro. */
    char *name;     /* The name of the macro. */
} macro;

/**
 * Represents an array of macros.
 */
typedef struct {
    macro *array;  /* The array of macros. */
    int used;      /* The number of macros used in the array. */
    int size;      /* The size of the array. */
} macro_array;

/**
 * Enumerates the possible macro statuses.
 */
enum {
    START_OF_MACRO,  /* Indicates the start of a macro. */
    BODY_OF_MACRO,   /* Indicates the body of a macro. */
    END_OF_MACRO,    /* Indicates the end of a macro. */
    NOT_A_MACRO      /* Indicates that it is not a macro. */
};


bool preprocess_file(char *base_filename);

void init_macro_array(macro_array *, int);

void insert_macro_array(macro_array *arr, char *name, char *content);

void free_macro_array(macro_array *);

int current_macro_status(char *, bool, int);

#endif /* PRE_ASSEMBLER_H */
