;file test for pre proccess errors.
MAIN:   mov @r1, LENGTH
LOOP:   jmp L1
	mcro lea
		sub @r1, @r4
		bne END
	endmcro
	mcro m1
		prn -28
		cmp @r1, K
	endmcro
	prn -5
	bne LOOP
	lea
L1: 	inc K
END: stop
STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
	.extern L3
