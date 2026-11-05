#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>
#include "../commands.h"
#include "../VirtualMachine/stack/stack.h"
#include <stdint.h>


typedef uint64_t hash_t;

const int LABELS_CNT = 128;

struct line
{
    char *index;
    size_t len;
};

const int MAX_LABEL_LEN = 20;

struct labels_t
{
    int  Labels[LABELS_CNT];
    hash_t Keys[LABELS_CNT];
    int labels_cnt;
};

struct assembler_t
{
    size_t string_cnt;
    line* lines;
    char* source;
    char command[8];
    stackelem_t value;
    size_t line_number;
    size_t ip;
    char reg;
    byte *byte_code;
    labels_t labels;
};

void assembler_init( assembler_t *asmb );
void place_label   ( assembler_t *asmb );
void get_label     ( assembler_t *asmb );
void push          ( assembler_t *asmb );
void pop           ( assembler_t *asmb );


size_t input_text_from_file( const char *file_name, char **buffer  );
size_t str_amount          ( const char *text,      size_t symbcnt );
ssize_t file_size          ( const char *filename                  );
void divide_text           ( char* text,            size_t symbcnt, line *line, size_t len );

void put_value( stackelem_t value, byte *ptr );
int binary_search_label( assembler_t *asmb, hash_t key );
#endif
