#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>
#include <stddef.h>
//#include "colors.h"

typedef uint64_t hash_t;

enum return_status
{
    ERROR = 1,
    SUCCESS = 0
};

bool is_valid( const void *p );
uint64_t djb2( void *ptr, size_t size );

#endif
