cmake_minimum_required(VERSION 3.24)
project(mmn14 C)

set(CMAKE_C_STANDARD 90)


# Source files
set(SOURCE_FILES
        assembler/assembler.c
        assembler/pre_assembler.c
        assembler/first_pass.c
        assembler/second_pass.c
        assembler/data_structures.c
        assembler/utils.c
        assembler/output.c
        assembler/ast.c
        )

# Header files
set(HEADER_FILES
        assembler/globals.h
        assembler/pre_assembler.h
        assembler/first_pass.h
        assembler/second_pass.h
        assembler/data_structures.h
        assembler/utils.h
        assembler/output.h
        assembler/ast.h
        assembler/symbols.c)

# Add executable target
add_executable(mmn14 ${SOURCE_FILES} ${HEADER_FILES})