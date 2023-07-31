#include <stdio.h>
#include "second_pass.h"


/*
    Processes a line during the second pass of the assembler.

    @param ast The ast tree representing the structure of the line.
    @param table The symbol table used for tracking symbols.
    @param line Information about the processed line.
    @return TRUE if the line is processed successfully, FALSE otherwise.
*/
bool second_pass_process(char *filename_with_am_suffix, int *ic, int *dc, data_image *my_data_image,
                         code_image *my_code_image, symbol_table *symbol_table){
    printf("in second pass\n");
    return TRUE;
}

/*
    Encodes the symbols into the code image.

    @param symbol_table The symbol table containing symbols information.
    @param code_image The code image used for storing the encoded machine code instructions.
    @param ic The instruction counter representing the current instruction address.
*/
void second_pass_encode_symbols(){

}
