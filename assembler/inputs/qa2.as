;invalid mcro name
; ILLEAGAL MACRO NAME - SAVED WORD
mcro lea
prn -8
endmcro

;invalid mcro name
; ILLEAGAL MACRO NAME - SAVED WORD
mcro .data
prn -8
endmcro

;valid
MAIN: mov @r3 ,LENGTH
LOOP: jmp L1
prn -5
bne LOOP

;INVALID COMMAND
m1

;valid
mcro m1
sub @r1, @r4
    bne END
endmcro
m1 

; valid
L1: inc K
bne LOOP
END: stop
STR: .string "abcdef"

;empty line with only tabs and spaces VVV
                                                       
;empty line with only tabs and spaces ^^^

;valid
LENGTH: .data 6,-9,15
K: .data 22

;94 characters long
; LINE IS TOO LONG
S: .string "fdsddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddddd"
