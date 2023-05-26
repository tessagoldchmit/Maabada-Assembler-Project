cmake_minimum_required(VERSION 3.25)
project(BarProject C)

set(CMAKE_C_STANDARD 90)


# Source files
set(SOURCE_FILES
        FinalProject/assembler.c
        FinalProject/preprocessor.c
        FinalProject/first_pass.c
        FinalProject/second_pass.c
        FinalProject/code.c
        FinalProject/directives.c
        FinalProject/tables.c
        FinalProject/utils.c
        FinalProject/write_files.c
        FinalProject/lexer.c
        )

# Header files
set(HEADER_FILES
        FinalProject/globals.h
        FinalProject/preprocessor.h
        FinalProject/first_pass.h
        FinalProject/second_pass.h
        FinalProject/code.h
        FinalProject/directives.h
        FinalProject/tables.h
        FinalProject/utils.h
        FinalProject/write_files.h
        FinalProject/lexer.h
        )

# Add executable target
add_executable(BarProject ${SOURCE_FILES} ${HEADER_FILES})

#add_executable(BarProject FinalProject/assembler.c)
