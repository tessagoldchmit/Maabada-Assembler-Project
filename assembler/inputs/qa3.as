MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP
L1: inc K
bne LOOP
END: stop

;.STRING  instead of .string
STR: .STRING "abcdef"

;data instead of .data
LENGTH: data 6,-9,15

;decimal numbers in data
K: .data 7.5, 8, 9.5

;spaces and no spaces in data array
ARR: .data 7,        ,8,9

;commas not in place
ARRTEST: .data ,7,8
ARRTEST2: .data 7 8
ARRTEST3: .data 7,8,

;data and string without labels (need to be warning or error?) TESSA!?!?!
.data 1, 2, 3
.string "hello"

;charred string
STR2: 'helo'
