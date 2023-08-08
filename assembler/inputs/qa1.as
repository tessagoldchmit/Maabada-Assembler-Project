;space inside register operand
; INVALID OPERAND 
MAIN: mov @     r5 ,LENGTH

;invalid register number
; INVALID REGISTER
mov @r9 ,K

; valid
STR: .string "abcdef"
LENGTH: .data 6,-9,15


;dek instead of dec
; INVALID COMMAND
dek LENGTH

;JMP instead of jmp
; INVALID COMMAND
LOOP: JMP L1


; valid
K: .data 22
.extern L3
