; INVALID OPERAND (maayan)
; todo: why it prints Illegal oper?
;MAIN: mov @     r5 ,LENGTH

; INVALID REGISTER (maayan)
; todo: why it prints Illegal regi?
;mov @r9 ,K

; ARGUMENTS FOR DATA MUST BE INTEGERS (tessa)
; todo: handle decimal numbers
K: .data 7.5, 8, 9.5


;todo: there is a memory leak here, understand why (maayan)
;useless labels (warning)
;USELESS1: .entry HOME1
;USELESS2: .extern HOME2

;duplicated symbols (maayan)
;SYMBOL ALREADY EXISTS
;    .extern DUPLICATE
;    .entry DUPLICATE

;using non exist label (known only after first pass!!!) (maayan)
; todo check me in a working program
; SYMBOL DOES NOT EXIST
;MAIN: mov @r3 ,NOTEXIST


;todo: understand why this symbol is invalid (tessa)
; valid ahusharmuta!
    PLUS: .data +1, -1