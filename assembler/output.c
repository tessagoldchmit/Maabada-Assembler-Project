#include "output.h"
#include "utils.h"


char binary_to_base64(unsigned int binary_number) {
    unsigned int six_bit_group = binary_number & 0b111111;

    if (six_bit_group <= 25) {
        return 'A' + six_bit_group; /* 'A' to 'Z' */
    } else if (six_bit_group <= 51) {
        return 'a' + (six_bit_group - 26); /* 'a' to 'z' */
    } else if (six_bit_group <= 61) {
        return '0' + (six_bit_group - 52); /* '0' to '9'  */
    } else if (six_bit_group == 62) {
        return '+'; /* Base64 character '+' */
    } else if (six_bit_group == 63) {
        return '/'; /* Base64 character '/' */
    } else {
        return '='; /* Padding character '=' */
    }
}

void write_object_file(char *filename, code_image *my_code_image, int *ic, data_image *my_data_image, int *dc) {
    FILE *file;
    char *filename_with_obj_suffix = concatenate_strings(filename, ".ob");
    unsigned int binary_numbers_arr[*ic + *dc];
    int i = 0;

    /* Open .ob file */
    file = fopen(filename_with_obj_suffix, "w");
    if (file == NULL) {
        fprintf(stderr, "Error: Failed to open .ob file '%s' for writing.\n", filename_with_obj_suffix);
        return;
    }
    fprintf(file, "%d %d\n", *ic, *dc);

    /* Add code image's binary numbers to the array */
    code_node *current_code_node = my_code_image->first;
    while (current_code_node != NULL) {
        int j;
        for (j = 0; j < current_code_node->L; j++) {
            binary_numbers_arr[i] = current_code_node->word[j];
            i++;
        }
        current_code_node = current_code_node->next;
    }

    /* Add data image's binary numbers to the array */
    data_node *current_data_node = my_data_image->head;
    while (current_data_node != NULL) {
        int j;
        for (j = 0; j < current_data_node->L; j++) {
            binary_numbers_arr[i] = current_data_node->word[j];
            i++;
        }
        current_data_node = current_data_node->next_node;
    }

    /* Write the binary numbers to the file as base64 characters */
    for (i = 0; i < *ic + *dc; i++) {
        unsigned int binary_number = binary_numbers_arr[i];

        unsigned int left_bits = (unsigned int)binary_number >> 6;
        unsigned int right_bits = binary_number & 0b111111;

        fputc(binary_to_base64(left_bits), file);
        fputc(binary_to_base64(right_bits), file);
        fputc('\n', file);
    }

    /* Free the allocated memory */
    fclose(file);
}
