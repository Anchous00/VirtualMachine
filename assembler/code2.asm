PUSH 1
POP BX
:7

CALL :BX^2
CALL :BX++


PUSH BX
PUSH 10
JNE :7
HALT

:BX++
PUSH BX ; BX++
PUSH 1
ADD
POP BX
RET

:BX^2
PUSH BX ; out BX^2
PUSH BX
MUL
OUT
RET



push 1        push  | KONST              X
push ax       push  | RX                 X
push ????     push  | RAM                ?
push [ax]     push  | RAM   | RX         X
push ax + 1   push  | KONST | RX         X
push [1]      push  | KONST | RAM        X
push [ax + 1] push  | KONST | RAM | RX   X

_ _ _     _ _ _ _ _
m r k     command

bytecode [1 , 1, 1] = PUSH 1 BX  = PUSH BX + 1

pop 1        pop  | KONST             ? OUT
pop ax       pop  | RX                X 1
pop ???      pop  | RAM               ?
pop [ax]     pop  | RAM   | RX        X  1
pop ax + 1   pop  | KONST | RX        ?
pop [1]      pop  | KONST | RAM       X  4
pop [ax + 1] pop  | KONST | RAM | RX  X  5


198 = 128 + 64 + 6


