PUSH 2
CALL :SQR
OUT
HALT

; ============================================================
; :SQR
; Вход:  верх стека = a (double, a > 0)
; Выход: верх стека = sqrt(a)
; Метод Ньютона: x_{n+1} = (x_n + a / x_n) / 2
; ============================================================
:SQR
    POP r9              ; r9 = a

    ; Начальное приближение: x = a / 2.0
    PUSH 2.0            ; second = 2.0
    PUSH r9             ; top = a
    DIV                 ; a / 2.0
    POP r10             ; r10 = x

    ; Счётчик итераций
    PUSH 10
    POP r12             ; r12 = 10

:SQR_LOOP
    ; Вычисляем x_new = (x + a / x) / 2.0
    PUSH 2.0            ; second = 2.0
    PUSH r10            ; second = x
    PUSH r9             ; top = a
    DIV                 ; a / x
    PUSH r10            ; top = x
    ADD                 ; x + a / x
    DIV                 ; (x + a / x) / 2.0
    POP r11             ; r11 = x_new

    ; x = x_new
    PUSH r11
    POP r10

    ; counter = counter - 1
    PUSH 1
    PUSH r12
    SUB
    POP r12

    ; if counter > 0, loop
    PUSH 0
    PUSH r12
    JA :SQR_LOOP

    ; Возврат результата
    PUSH r10
    RET
