;space inside register operand
MAIN: mov @     r5 ,LENGTH

;invalid register number
mov @r9 ,K

;dek instead of dec
dek LENGTH

;JMP instead of jmp
LOOP: JMP L1


STR: .string "abcdef"
LENGTH: .data 6,-9,15
K: .data 22
.extern L3