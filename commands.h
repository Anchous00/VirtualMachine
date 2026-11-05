#ifndef COMMANDS_H
#define COMMANDS_H

const int COMMANDS_CNT = 18;

typedef unsigned char byte;

enum COMMANDS
{
    PUSH  = 0x01,
    ADD   = 0x02,
    SUB   = 0x03,
    OUT   = 0x04,
    MUL   = 0x05,
    POP   = 0x06,
    JUMP  = 0x07,
    JA    = 0x08,
    JAE   = 0x09,
    JB    = 0x0A,
    JBE   = 0x0B,
    JE    = 0x0C,
    JNE   = 0x0D,
    CALL  = 0x0E,
    RET   = 0x0F,
    IN    = 0x10,
    DIV   = 0x11,
    HALT  = 0x00
};

enum param_t
{
    KONST        = 0x20,
    RAM          = 0x40,
    RX           = 0x80,
    COMMAND_MASK = 0x1F
};

enum RX
{
    AX = 0,
    BX = 1,
    CX = 2,
    DX = 3,
    EX = 4
};

#endif
