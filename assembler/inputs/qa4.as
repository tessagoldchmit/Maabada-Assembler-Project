;useless labels (warning)
USELESS1: .entry HOME1
USELESS2: .extern HOME2

;duplicated labels
.extern DUPLICATE
.entry DUPLICATE

;using non exist label (known only after first pass!!!)
MAIN: mov @r3 ,NOTEXIST
LOOP: jmp L1

;too many arguemnts
prn -5, @r1, 5

;too many arguemnts
mov LENGTH, @r1, 5

bne LOOP
L1: inc K
bne LOOP
END: stop
STR: .string “abcdef”
LENGTH: .data 6,-9,15
K: .data 22

;another duplicated labels
DUP2: .data 1
DUP2: .data 2

;invalid labels
2NUMBER .data 1
@r1: .data 1
mov: .data 1

;far colon of label
FAR_COLON : .data 1

;using plus and minus in data
PLUS: .data +1, -1
