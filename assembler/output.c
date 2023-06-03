#include "output.h"
#include <stdio.h>
#include <stdlib.h>


char binary_to_base64(int bits) {
    if (bits >= 0 && bits <= 25) {
        return 'A' + bits; //'A' to 'Z'
    } else if (bits >= 26 && bits <= 51) {
        return 'a' + (bits - 26); // 'a' to 'z'
    } else if (bits >= 52 && bits <= 61) {
        return '0' + (bits - 52); // '0' to '9'
    } else if (bits == 62) {
        return '+'; // Base64 character '+'
    } else if (bits == 63) {
        return '/'; // Base64 character '/'
    } else {
        return '='; // Padding character '='
    }
}


int main() {
    int binary_numbers_arr[] = {
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
    int IC = 18;
    int DC = 11;

    int i;

    FILE *file = fopen("outputs/output.obj", "w"); // Open the file in write mode

    fprintf(file, "%d %d\n", IC, DC);

    if (file == NULL) {
        printf("Failed to open the file.\n");
        exit(1);
    }

    for (i = 0; i < IC + DC; i++) {
        int binary_number = binary_numbers_arr[i];

        unsigned int left_bits = binary_number >> 6;
        unsigned int right_bits = binary_number & 0b111111;

        fputc(binary_to_base64(left_bits), file);
        fputc(binary_to_base64(right_bits), file);
        fputc('\n', file);

    }
    fclose(file);
    printf("Data written to the file.\n");

    return 0;
}



/* TODO support writing .obj file, .ent file and .ext file */
