IN
POP AX ; AX = N
;zzzz
PUSH 1
POP BX
PUSH 1
PUSH 1
POP [BX]
CALL :BX++
POP [BX]
CALL :BX++          ; BX = 3, RAM = [1, 1, ...]

:FOR
CALL :FIB

PUSH AX
PUSH BX
JBE :FOR

PUSH [BX + -1]
OUT
HALT


:BX++
PUSH BX
PUSH 1
ADD
POP BX

RET

:FIB
PUSH [BX + -1]
PUSH [BX + -2]
ADD
POP [BX]
CALL :BX++

RET
