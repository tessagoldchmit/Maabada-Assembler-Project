assembler: assembler.o pre_assembler.o first_pass.o second_pass.o symbols.o utils.o output.o ast.o data_structures.o
	gcc -ansi -Wall -pedantic assembler.o pre_assembler.o first_pass.o second_pass.o symbols.o utils.o output.o ast.o data_structures.o -o assembler
assembler.o: assembler.c pre_assembler.h first_pass.h utils.h output.h second_pass.h symbols.h logs.h 
	gcc -g -c -ansi -Wall -pedantic assembler.c -o assembler.o

pre_assembler.o: pre_assembler.c pre_assembler.h utils.h logs.h
	gcc -g -c -ansi -Wall -pedantic pre_assembler.c -o pre_assembler.o

first_pass.o: first_pass.c first_pass.h globals.h ast.h logs.h
	gcc -g -c -ansi -Wall -pedantic first_pass.c -o first_pass.o

second_pass.o: second_pass.c second_pass.h data_structures.h symbols.h logs.h
	gcc -g -c -ansi -Wall -pedantic second_pass.c -o second_pass.o

symbols.o: symbols.c symbols.h logs.h
	gcc -g -c -ansi -Wall -pedantic symbols.c -o symbols.o

utils.o: utils.c utils.h
	gcc -g -c -ansi -Wall -pedantic utils.c -o utils.o

output.o: output.c output.h utils.h
	gcc -g -c -ansi -Wall -pedantic output.c -o output.o

ast.o: ast.c ast.h utils.h symbols.h logs.h
	gcc -g -c -ansi -Wall -pedantic ast.c -o ast.o

data_structures.o: data_structures.c data_structures.h
	gcc -g -c -ansi -Wall -pedantic data_structures.c -o data_structures.o
