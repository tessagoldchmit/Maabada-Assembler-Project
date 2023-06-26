;this is comment
MAIN: mov @r3 ,LENGTH
TEST: .extern HEY
TEST2: .string "ab"
TEST1: .data 7,6
TEST3: .string "abd"
TEST4: .data 1,2


mcr m1
mov r2, r1
stop
endmcr


mcr m2
rts
endmcr


LOOP: jmp L1

m1

m1

mov r2, r1
m2

prn -5
bne LOOP
sub @r1, @r4
    bne END
L1: inc K
bne LOOP
END: stop
LENGTH: .data 6,-9,15
K: .data 22