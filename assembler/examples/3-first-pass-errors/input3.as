;file test for first pass errors.
.entry LENGTH
.extern W
MAIN:   mov @r1, LENGTH
mov @	r5, K
add LENGTH,, @r2 
LOOP:   jmp L1
	mcro m1
		prn -28
		cmp @r1, K
	endmcro
	prn @r9
	bne LOOP
	bme L3
	m1
L1: 	inc K
prn -550
prn
sub 2, 2
lea @r1, K
lea @r2 LENGTH
inc 1
stop K
K: .data 22
lONG2: .string "this line is toooo looooooooooooooooooooooooooooooooooooooooooooooooooong"
STR: .string "abcdef"
.data 3.5, 5.4, 5.5
.dat 3
LENGTH: .data 6,	-9,15
123INVALIDSYMBOL: .data 60
ARR: .data 2049, 2048, 2047
K: .string "symbol duplicated"
.extern L3

.data ,2 ,3
.data 2,3,
.string 'nfjr"
.string "fsdfasd'
NOSTRING: .string
.data A
.data
.entry
.extern