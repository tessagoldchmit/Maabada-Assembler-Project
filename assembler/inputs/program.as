;this is comment

mcr m1
mov r2, r1
stop
endmcr


mcr m2
rts
endmcr



m1

m1

mov r2, r1
m2

MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP
sub @r1, @r4
    bne END
L1: inc K
bne LOOP
END: stop
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22