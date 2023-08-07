;invalid mcro name
mcro lea
prn -8
endmcro

;invalid mcro name
mcro .data
prn -8
endmcro

MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP

m1

;macro declared after call
mcro m1
sub @r1, @r4
    bne END
endmcro


L1: inc K
bne LOOP
END: stop
STR: .string "abcdef"

;empty line with only tabs and spaces VVV

;empty line with only tabs and spaces ^^^

LENGTH: .data 6,-9,15
K: .data 22

;94 characters long
S: .string "fdsddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
