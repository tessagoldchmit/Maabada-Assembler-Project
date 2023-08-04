#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <stdlib.h>
#include "data_structures.h"

void write_object_file(char *filename, code_image *my_code_image, int *ic, data_image *my_data_image, int *dc);

#endif /* OUTPUT_H */