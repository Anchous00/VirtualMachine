#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>
#include "utils.h"
#include "../../logger.h"

#ifdef STKDEBUG
#define ON_DEBUG(...) __VA_ARGS__
#else
#define ON_DEBUG(...)
#endif



typedef uint64_t errs_t;
typedef int stackelem_t;

#define stack_specifier "%8d"

const stackelem_t POIZON = 0xEDA;

const int TEST_AMOUNT = 100;

ON_DEBUG
(
        const stackelem_t LEFT_DATA_CANARY    = 0xE1DADEDA;
        const stackelem_t RIGHT_DATA_CANARY   = 0xEBA1DEDA;
        const stackelem_t LEFT_STRUCT_CANARY  = 0xB1BA;
        const stackelem_t RIGHT_STRUCT_CANARY = 0xB0BA;
)

enum stack_err_code_t
{
        STACK_OK                 =  0,
        WRONG_STACK_PTR          =  1,
        STACK_DESTROYED          =  2,
        WRONG_STACK_DATA         =  4,
        ON_DEBUG
        (
        NEGATIVE_STACK_SIZE      =  8,
        NEGATIVE_STACK_CAPACITY  =  16,
        LEFT_DATA_CANARY_DEAD    =  32,
        RIGHT_DATA_CANARY_DEAD   =  64,
        LEFT_STRUCT_CANARY_DEAD  =  128,
        RIGHT_STRUCT_CANARY_DEAD =  256,
        WRONG_DATA_HASH          =  512
        )
};


struct stack_t
{
    ON_DEBUG(
            stackelem_t left_struct_canary;
           )
    stackelem_t *data;
    size_t size;
    size_t capacity;
    ON_DEBUG(
            errs_t errs;
            const char *filename;
            const char *name;
            int line;
            hash_t data_hash;

            stackelem_t right_struct_canary;
           )
};

return_status stack_ctor       ( stack_t *stk, size_t capacity ON_DEBUG(, const char *filename, const char *name, int line));
return_status stack_detor      ( stack_t *stk                 );
return_status stack_push       ( stack_t *stk, stackelem_t  x );
return_status stack_pop        ( stack_t *stk, stackelem_t *x );
return_status stack_resize_up  ( stack_t *stk                 );
return_status stack_resize_down( stack_t *stk                 );
errs_t        stack_ok         ( stack_t *stk );



ON_DEBUG(
        void stack_dump              ( stack_t *stk );

        return_status stack_print_error    ( stack_t *stk );
        errs_t        stack_data_ok        ( stack_t *stk );
        uint64_t      stack_count_data_hash( stack_t *stk );
        bool is_equal( stackelem_t x, stackelem_t y );
)

#endif
