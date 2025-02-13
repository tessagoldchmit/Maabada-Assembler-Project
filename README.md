# Maabada-Assembler-Project

## Project Description:

This project is a C programming assignment aimed at developing an assembler for a custom 12-bit assembly language. 
The project was developed by @MaayanKaidar and @TessaGoldchmit.

## Project Modules:

1. `assembler`: Contains the core logic of the assembler, handling the main processing function.
2. `ast`: The ast (Abstract Syntax Tree) module defines the data structure and functions related to the ast.
3. `data_structures`: Implements the data structures: code_image and data_image.
4. `first_pass`: Handles the processing of code lines during the first pass of the assembler.
5. `globals`: Defines types and constants used throughout the project.
6. `logs`: Contains log messages (infos and errors) and macros definitions related to them.
7. `output`: Provides methods for generating the output files: *.ob (object), *.ext (external), and *.ent (entry) files.
8. `pre_assembler`: Responsible for performing initial file preprocessing tasks, including spreading macros and removing empty lines & comments.
9. `second_pass`: Handles the processing of code lines during the second pass of the assembler.
10. `symbols`: Implements the symbols table and the extern symbols table.
11. `utils`: Contains general-purpose functions that are utilized across the project.


## Some examples

### Example 1 - A working example
![output1.png](assembler/examples/1-working-example/output1.png)

### Example 2 - errors in preprocessing
![output2.png](assembler/examples/2-preprocessing-errors/output2.png)

### Example 3 - errors in first pass
![output3.png](assembler/examples/3-first-pass-errors/output3.png)

### Example 4 - errors in second pass
![output4.png](assembler/examples/4-second-pass-errors/output4.png)
