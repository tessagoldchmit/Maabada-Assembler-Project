#include "output.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void binary_to_base64(int binary_number){
    char[] base64_string = "";

    // Extracting the 6 left bits
    unsigned int left_bits = binary_number >> 6;

    if (left_bits >= 0 && left_bits <= 25) {
        base64_string='A' + left_bits; //'A' to 'Z'
    } else if (left_bits >= 26 && left_bits <= 51) {
        base64_string='a' + (left_bits - 26); // 'a' to 'z'
    } else if (left_bits >= 52 && left_bits <= 61) {
        base64_string='0' + (left_bits - 52); // '0' to '9'
    } else if (left_bits == 62) {
        base64_string='+'; // Base64 character '+'
    } else if (left_bits == 63) {
        base64_string='/'; // Base64 character '/'
    } else {
        base64_string='='; // Padding character '='
    }

    // Extracting the 6 right bits
    unsigned int right_bits = binary_number & 0b111111;

    if (right_bits >= 0 && right_bits <= 25) {
        base64_string='A' + right_bits; // 'A' to 'Z'
    } else if (right_bits >= 26 && right_bits <= 51) {
        base64_string='a' + (right_bits - 26); // 'a' to 'z'
    } else if (right_bits >= 52 && right_bits <= 61) {
        base64_string='0' + (right_bits - 52); // '0' to '9'
    } else if (right_bits == 62) {
        base64_string='+'; // Base64 character '+'
    } else if (right_bits == 63) {
        base64_string='/'; // Base64 character '/'
    } else {
        base64_string='='; // Padding character '='
    }
    fputc('\n', file);
}


int main(){
    int binaryNumbers[] = {
            0b101000001100,
            0b000110000000,
            0b000111110110,
            0b000100101100,
            0b000111000110,
            0b000110000100,
            0b111111101100,
            0b000101001100,
            0b000000000001,
            0b101001110100,
            0b000010010000,
            0b000101001100,
            0b000000000001,
            0b000011101100,
            0b001000000010,
            0b000100101100,
            0b000000000001,
            0b000111100000,
            0b000001100001,
            0b000001100010,
            0b000001100011,
            0b000001100100,
            0b000001100101,
            0b000001100110,
            0b000000000000,
            0b000000000110,
            0b111111110111,
            0b000000001111,
            0b000000010110
    };
    int i=0;
    int IC=18;
    int DC=11;

    FILE* file = fopen("C:\\Users\\Maayan\\CLionProjects\\assembler\\Base64.txt", "w"); // Open the file in write mode

    fprintf(file, "%d ", IC);
    fprintf(file, "%d\n", DC);

    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    for(i; i<IC+DC; i++) {
        binary_to_base64(binaryNumbers[i]);
    }
    fclose(file);
    printf("Data written to the file.\n");

    return 0;
}



/* TODO support writing .ob file, .ent file and .ext file */
