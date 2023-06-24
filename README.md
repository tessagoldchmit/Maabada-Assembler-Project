# Maabada-Assembler-Project

## Project Description:

This project is a C programming assignment aimed at developing an assembler for a custom 12-bit assembly language. 
The project was developed by @MaayanKaidar and @TessaGoldchmit.

## Project Modules:

1. `assembler`: Contains the core logic of the assembler, handling the main processing function.
2. `pre_assembler`: Responsible for performing initial file preprocessing tasks, including spreading macros and removing empty lines & comments.
3. `first_pass`: Includes functions for processing a single line during the first pass of the assembler.
4. `second_pass`: Handles the processing of code lines during the second pass of the assembler.
5. `globals`: Defines types and constants used throughout the project.
6. `ast`: The ast (Abstract Syntax Tree) module defines the data structure and functions related to the ast, which are utilized in both the first pass and second pass stages.
7. `data_structures`: Implements a linked list-based data structure used for managing macros and symbol_node tables.
8. `utils`: Contains general-purpose functions that are utilized across the project.
9. `output`: Provides methods for generating the output files: *.ob (object), *.ext (external), and *.ent (entry) files.