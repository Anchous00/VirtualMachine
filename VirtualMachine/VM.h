#ifndef VM_H
#define VM_H

#include "stack/stack.h"
#include <string.h>
#include <sys/stat.h>
#include "../commands.h"

const int RAM_SIZE = 600;

struct processor_t
{
    byte         *buffer;
    size_t        ip;
    stack_t       stk;
    stack_t       call_stk;
    stackelem_t   rx[16];
    stackelem_t   *RAM;
};

void halt ( processor_t *proc ) __attribute__ ((noreturn));
void push ( processor_t *proc );
void add  ( processor_t *proc );
void sub  ( processor_t *proc );
void mul  ( processor_t *proc );
void out  ( processor_t *proc );
void pop  ( processor_t *proc );
void jump ( processor_t *proc );
void jae  ( processor_t *proc );
void ja   ( processor_t *proc );
void jbe  ( processor_t *proc );
void jb   ( processor_t *proc );
void je   ( processor_t *proc );
void jne  ( processor_t *proc );
void call ( processor_t *proc );
void ret  ( processor_t *proc );
void in   ( processor_t *proc );
void div  ( processor_t *proc );

void dump_ram( processor_t *proc );
void draw_ram( processor_t *proc );

void print_bytecode( processor_t *proc, size_t code_size );
void list_bytecode ( processor_t *proc, size_t code_size );

ssize_t file_size( const char* filename );
void get_value( stackelem_t *value, byte *ptr );

const char* reg_name( byte reg );
const char* cmd_name( byte cmd );
int         args_len( byte cmd );

#endif
