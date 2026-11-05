#include "utils.h"

bool is_valid( const void *p )
{
return p != nullptr && (long long int)p % (long long int)8 == 0;
// #ifdef __linux__
//     extern char _etext;
//    // printf("linux\n");
//     return ((const char*)p - &_etext > 0 &&
//             (const char*)((long long)16 * 1024 * 1024 * 1024) - (const char*)p > 0 );
// #endif
// #ifdef _WIN32
// //printf("windows\n");
//     return (const char*)p - (const char*)(4 * 1024 * 1024) > 0 &&
//            (const char*)((long long)16 * 1024 * 1024 * 1024) - (const char*)p > 0;
// #endif
}

hash_t djb2( void *ptr, size_t size )
{
    hash_t hash = 0xEDA;
    for (size_t i = 0; i < size; i++)
    {
        hash = ((hash << 5) + hash) + *((unsigned char*)ptr + i);
    }

    return hash;
}
