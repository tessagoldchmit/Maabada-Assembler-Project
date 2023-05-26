#include "second_pass.h"
#include "tables.h"
#include "globals.h"

/*
    Processes a line during the second pass of the assembler.

    @param ast The ast tree representing the structure of the line.
    @param table The symbol table used for tracking symbols.
    @param line Information about the processed line.
    @return TRUE if the line is processed successfully, FALSE otherwise.
*/
bool second_pass_line_process();

/*
    Encodes the labels into the code image.

    @param symbol_table The symbol table containing label information.
    @param code_image The code image used for storing the encoded machine code instructions.
    @param ic The instruction counter representing the current instruction address.
*/
void second_pass_encode_labels();
