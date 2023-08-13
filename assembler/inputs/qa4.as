;useless labels (warning)
USELESS1: .entry HOME1
USELESS2: .extern HOME2

;duplicated symbols
;SYMBOL ALREADY EXISTS
.extern DUPLICATE
.entry DUPLICATE

;using non exist label (known only after first pass!!!)
; SYMBOL DOES NOT EXIST
MAIN: mov @r3 ,NOTEXIST

;valid
LOOP: jmp L1

;too many arguemnts
; TRAILING CHARACTERS
prn -5, @r1, 5

;too many arguemnts
; TRAILING CHARACTERS
mov LENGTH, @r1, 5

;valid
bne LOOP
L1: inc K
bne LOOP
END: stop
STR: .string "abcdef"
    LENGTH: .data -6,-9, -15
K: .data 22

; invalid integer
; ARGUMENTS FOR DATA MUST BE INTEGERS
LENGTH: .data -6,-9, -15.0

;another duplicated symbols
; SYMBOL ALREADY EXIST
DUP2: .data 1
DUP2: .data 2

;invalid labels
; INVALID SYMBOL NAME
2NUMBER .data 1
@r1: .data 1
mov: .data 1

;far colon of label
; INVALID COMMAND
FARCOLON : .data 1

;using plus and minus in data
; valid ahusharmuta! 
PLUS: .data +1, -1
