; AX - X, BX - Y, CX - nomer ram, DX - R, EX- size ram
; O = (16, 16)
; nomer ram = AX + 32 * BX

IN
POP r6
IN
POP r7
IN
POP r8

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

; ============================================================
; Левый шар: центр (r15 - r6, r15 + r6), радиус r6
; ============================================================
PUSH AX
PUSH r6
PUSH r15
SUB
SUB                 ; (r15 - r6) - AX = dx
PUSH AX
PUSH r6
PUSH r15
SUB
SUB
MUL                 ; dx^2

PUSH BX
PUSH r15
PUSH r6
ADD
SUB                 ; (r15 + r6) - BX = dy
PUSH BX
PUSH r15
PUSH r6
ADD
SUB
MUL                 ; dy^2
ADD                 ; sum

PUSH r6
PUSH r6
MUL
PUSH 1
ADD                 ; r6^2 + 1
JA :YES

; ============================================================
; Правый шар: центр (r15 + r6, r15 + r6), радиус r6
; ============================================================
PUSH AX
PUSH r15
PUSH r6
ADD
SUB                 ; (r15 + r6) - AX = dx
PUSH AX
PUSH r15
PUSH r6
ADD
SUB
MUL                 ; dx^2

PUSH BX
PUSH r15
PUSH r6
ADD
SUB                 ; (r15 + r6) - BX = dy
PUSH BX
PUSH r15
PUSH r6
ADD
SUB
MUL                 ; dy^2
ADD                 ; sum

PUSH r6
PUSH r6
MUL
PUSH 1
ADD                 ; r6^2 + 1
JA :YES
; ============================================================
; Головка: центр (r15, r15 - 2*r6), радиус r8
; ============================================================
PUSH AX
PUSH r15
SUB                 ; r15 - AX = dx
PUSH AX
PUSH r15
SUB
MUL                 ; dx^2

PUSH BX
PUSH r6
PUSH r6
ADD                 ; 2*r6
PUSH r15
SUB                 ; r15 - 2*r6
SUB                 ; (r15 - 2*r6) - BX = dy
PUSH BX
PUSH r6
PUSH r6
ADD
PUSH r15
SUB
SUB
MUL                 ; dy^2
ADD                 ; sum

PUSH r8
PUSH r8
MUL
PUSH 1
ADD                 ; r8^2 + 1
JA :YES

; ============================================================
; Не попала
; ============================================================
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
PUSH 0 ;r
PUSH 256
MUL
DIV

ADD
ADD
RET
