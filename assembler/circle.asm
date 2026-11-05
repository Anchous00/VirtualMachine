; AX - X, BX - Y, CX - nomer ram, DX - R, EX- size ram
; O = (16, 16)
; nomer ram = AX + 32 * BX

IN
POP DX

PUSH 0
POP BX
PUSH 0
POP CX


PUSH 600
POP EX
PUSH 2
PUSH EX
DIV
POP r15

:FOR_BX
PUSH 0
POP AX
    :FOR_AX


    PUSH EX
    PUSH BX
    MUL
    PUSH AX
    ADD
    POP CX
        ;PUSH CX
        ;OUT
    CALL :IFTOMATO
    POP [CX]



    CALL :AX++
    PUSH EX
    PUSH AX
    JB :FOR_AX
CALL :BX++
PUSH EX
PUSH BX
JB :FOR_BX


HALT


:AX++
PUSH AX + 1
POP AX
RET

:BX++
PUSH BX + 1
POP BX
RET

:IFTOMATO

    PUSH AX
    PUSH r15
    SUB
    PUSH AX
    PUSH r15
    SUB
MUL


    PUSH BX
    PUSH r15
    SUB
    PUSH BX
    PUSH r15
    SUB
MUL

ADD

PUSH DX
PUSH DX
MUL

JA :YES


PUSH 0
RET
:YES


PUSH EX
PUSH 256
PUSH AX ; b
MUL
DIV
PUSH 256
PUSH 256
MUL
MUL

PUSH EX
PUSH BX ; g
PUSH 256
MUL
DIV
PUSH 256
MUL

PUSH EX
PUSH EX
MUL
PUSH CX ;r
PUSH 256
MUL
DIV

ADD
ADD
RET
