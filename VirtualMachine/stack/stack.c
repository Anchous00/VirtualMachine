#include "stack.h"
#define ON_LOG(...)

return_status stack_ctor( stack_t *stk, size_t capacity ON_DEBUG(, const char *filename, const char *name, int line ))
{
    if (!is_valid(stk))
        return ERROR;
    if (capacity > SIZE_MAX / 2)
    {
        ON_DEBUG(stk->errs = stk->errs | NEGATIVE_STACK_CAPACITY;)
        return ERROR;
    }

    stk->capacity = capacity;
    stk->data = (stackelem_t*)calloc(stk->capacity + 2, sizeof(stackelem_t));
    if (!is_valid(stk->data))
    {
        ON_DEBUG(stk->errs = stk->errs | WRONG_STACK_DATA;)
        return ERROR;
    }


    for (size_t i = 1; i < 1 + capacity; i++)
        stk->data[i] = POIZON;
    stk->size = 0;

ON_DEBUG(
            stk->data[0]                 = LEFT_DATA_CANARY;
            stk->data[1 + stk->capacity] = RIGHT_DATA_CANARY;
            stk->left_struct_canary      = LEFT_STRUCT_CANARY;
            stk->right_struct_canary     = RIGHT_STRUCT_CANARY;

            stack_count_data_hash(stk);

            stk->filename = filename;
            stk->name     = name;
            stk->line     = line;

            if ((stk->errs = stack_ok(stk)) != STACK_OK)
            {
                return ERROR;
            }
        )
ON_LOG(
        LOG("created stack in file %s:%d\n", __FILE__, __LINE__);
       )
    return SUCCESS;
}

return_status stack_detor( stack_t *stk )
{
    if (!is_valid(stk))
    {
        return ERROR;
    }

    ON_LOG(
            LOG("stack destroyed in file %s:%d\n", __FILE__, __LINE__);
            )

   ON_DEBUG
   (
        if (stack_data_ok(stk) != STACK_OK)
        {
            stack_print_error(stk);
            return ERROR;
        }
   )
    stk->capacity = 0;
    stk->size = 0;
    free(stk->data);
    stk->data = NULL;
    ON_DEBUG(
            stk->data_hash = 0;
            stk->errs = stk->errs | STACK_DESTROYED;
            )

    return SUCCESS;
}

return_status stack_push( stack_t *stk, stackelem_t x )
{
    if (stack_ok(stk) != STACK_OK)
        return ERROR;

    if (stk->size >= stk->capacity)
        stack_resize_up(stk);
    stk->data[1 + stk->size++] = x;

    ON_DEBUG(
            stack_count_data_hash(stk);
            if ((stk->errs = stack_ok(stk)) != STACK_OK)
                return ERROR;
            )

    ON_LOG(
            LOG("pushed " stack_specifier" in stack in file %s:%d\n", x, __FILE__, __LINE__);
            )

    return SUCCESS;
}

return_status stack_pop( stack_t *stk, stackelem_t *x )
{
    if (stack_ok(stk) != STACK_OK)
    {
        *x = POIZON;
        return ERROR;
    }
    if (stk->size <= 0)
    {
        *x = POIZON;
        ON_DEBUG(stk->errs = stk->errs | NEGATIVE_STACK_SIZE;)
        return ERROR;
    }

    // if (stk->size == stk->capacity / 4)
    //     stack_resize_down(stk);

    stackelem_t val = stk->data[1 + --stk->size];
    stk->data[1 + stk->size]= POIZON;

    *x = val;
    ON_DEBUG(
            stack_count_data_hash(stk);
            if ((stk->errs = stack_ok(stk)) != STACK_OK)
                return ERROR;
            )

    ON_LOG(
            LOG("poped " stack_specifier" from stack in file %s:%d\n", *x, __FILE__, __LINE__);
            )

    return SUCCESS;
}

return_status stack_resize_up( stack_t *stk )
{
    if (stack_ok(stk) != STACK_OK)
        return ERROR;
    stk->data = (stackelem_t*)realloc(stk->data,
                                       (stk->capacity * 2 + 2) * sizeof(stackelem_t));

    stk->capacity *= 2;
    for (size_t i = stk->size; i < stk->capacity; i++)
        stk->data[1 + i] = POIZON;

    ON_DEBUG(
            stk->data[1 + stk->capacity] = RIGHT_DATA_CANARY;
            if ((stk->errs = stack_ok(stk)) != STACK_OK)
                return ERROR;
            )
    return SUCCESS;
}

return_status stack_resize_down( stack_t *stk )
{
    if (stack_ok(stk) != STACK_OK)
        return ERROR;

    stk->data = (stackelem_t*)realloc(stk->data,
                                       (stk->capacity / 2 + 2) * sizeof(stackelem_t));

    stk->capacity /= 2;

    ON_DEBUG(
            stk->data[1 + stk->capacity] = RIGHT_DATA_CANARY;
            if ((stk->errs = stack_ok(stk)) != STACK_OK)
                return ERROR;
            )
    return SUCCESS;
}

errs_t stack_ok( stack_t *stk )
{
    errs_t errs = STACK_OK;
    if (!is_valid(stk))
    {
        errs = errs | WRONG_STACK_PTR;
        return errs;
    }

    ON_DEBUG(
            if (stk->capacity > SIZE_MAX / 2)
                errs = errs | NEGATIVE_STACK_CAPACITY;

            if (stk->size > SIZE_MAX / 2)
                errs = errs | NEGATIVE_STACK_SIZE;

            if (!is_equal(stk->left_struct_canary, LEFT_STRUCT_CANARY))
                errs = errs | LEFT_STRUCT_CANARY_DEAD;

            if (!is_equal(stk->right_struct_canary, RIGHT_STRUCT_CANARY))
                errs = errs | RIGHT_STRUCT_CANARY_DEAD;
            errs = errs | stack_data_ok(stk);
            )

    return errs;
}

ON_DEBUG(
hash_t stack_count_data_hash( stack_t *stk )
{
    return stk->data_hash = djb2(stk->data, (stk->capacity + 2) * sizeof(stackelem_t)); //2 canaries
}

void stack_dump( stack_t *stk )
{
    #ifndef LOG_ON
        LOG_FILE = stdout;
    #endif

    if (!is_valid(stk))
    {
        fprintf(LOG_FILE, "ERROR:wrong stack pointer\n");
        return;
    }

    ON_LOG(log(myLOG_ACTION, "stack dump:\n\n");)

    stk->errs = stack_ok(stk);
    stack_print_error(stk);



   if (is_valid(stk->data) && is_valid(stk->filename))
   fprintf(LOG_FILE, "struct_t %s [%p] created by %s at %s:%d\n"
           "\tcapacity = %zu\n"
           "\tsize = %zu\n"
           "\tdata [%p]\n",
            stk->name,
            stk,
            stk->filename,
            stk->filename,
            stk->line,
            stk->capacity,
            stk->size,
            stk->data + 1);
    else
    {
        fprintf(LOG_FILE, "ERROR: cant dump stack info");
    }

    if ((stk->errs & WRONG_STACK_DATA) == STACK_OK)
    {
//print left canary
    fprintf(LOG_FILE, "\t\t[%d] " stack_specifier" %s\n",
            -1,
            stk->data[0],
            is_equal(stk->data[0], LEFT_DATA_CANARY)
            ?
            "[canary]"
            :
            "[DEAD CANARY]");
//print data
    for (size_t i = 0; i < stk->capacity; i++)
        fprintf(LOG_FILE, "\t\t[%zu] " stack_specifier" %s\n",
                i,
                stk->data[i + 1],
                isnan(stk->data[i + 1]) ? "[poison]" : "");
//print right canary
    fprintf(LOG_FILE, "\t\t[%zu] " stack_specifier" %s\n",
            stk->capacity,
            stk->data[1 + stk->capacity],
            is_equal(stk->data[1 + stk->capacity], RIGHT_DATA_CANARY)
            ?
            "[canary]"
            :
            "[DEAD CANARY]");
    }
    fprintf(LOG_FILE, "\t\tdata hash = 0x%lx\n", stk->data_hash);

    return;
}

return_status stack_print_error( stack_t *stk )
{
        if (!is_valid(stk))
        {
            fprintf(LOG_FILE, "error: wrong stack pointer\n");
            return ERROR;
        }

        #ifndef LOG_ON
            LOG_FILE = stdout;
        #endif

        if (!(stk->errs | STACK_OK))
        {
            fprintf(LOG_FILE, "correct stack\n" );
            return SUCCESS;
        }
        if (stk->errs & WRONG_STACK_PTR)                        // (error & ERROR) == ERROR
            log(myLOG_ERROR, "fatal error: wrong stack pointer");
        if (stk->errs & STACK_DESTROYED)
            log(myLOG_ERROR, "destructed stack");
        if (stk->errs & WRONG_STACK_DATA)
            log(myLOG_ERROR, "wrong stack data pointer\n" );
        if (stk->errs & NEGATIVE_STACK_SIZE)
            log(myLOG_ERROR, "negative stack size\n" );
        if (stk->errs & NEGATIVE_STACK_CAPACITY)
            log(myLOG_ERROR, "negative stack capacity\n" );
        if (stk->errs & RIGHT_DATA_CANARY_DEAD)
            log(myLOG_ERROR, "right data canary is dead, data may be corrupted");
        if (stk->errs & LEFT_DATA_CANARY_DEAD)
            log(myLOG_ERROR, "left data canary is dead, data may be corrupted");
        if (stk->errs & RIGHT_STRUCT_CANARY_DEAD)
            log(myLOG_ERROR, "right stack canary is dead, stack may be corrupted");
        if (stk->errs & LEFT_STRUCT_CANARY_DEAD)
            log(myLOG_ERROR, "left stack canary is dead, stack may be corrupted");
        if (stk->errs & WRONG_DATA_HASH)
            log(myLOG_ERROR, "wrong data hash, data may be corrupted");

    return SUCCESS;
}

errs_t stack_data_ok( stack_t *stk )
{
    errs_t errs = STACK_OK;
    if (!is_valid(stk))
    {
        errs = errs | WRONG_STACK_PTR;
        return errs;
    }

    if (!is_valid(stk->data))
    {
        errs = errs | WRONG_STACK_DATA;
        return errs;
    }

    if (!is_equal(stk->data[0], LEFT_DATA_CANARY))
        errs = errs | LEFT_DATA_CANARY_DEAD;

    if (!is_equal(stk->data[1 + stk->capacity], RIGHT_DATA_CANARY))
        errs = errs | RIGHT_DATA_CANARY_DEAD;

    stack_count_data_hash(stk);
    if (stk->data_hash != stack_count_data_hash(stk))
        errs = errs | WRONG_DATA_HASH;

    return errs;
}

bool is_equal( stackelem_t x, stackelem_t y )
{
    switch (sizeof(stackelem_t))
    {
        case 1:
            return *(uint8_t*)(&x)  == *(uint8_t*)(&y);
        case 2:
            return *(uint16_t*)(&x) == *(uint16_t*)(&y);
        case 4:
            return *(uint32_t*)(&x) == *(uint32_t*)(&y);
        case 8:
            return *(uint64_t*)(&x) == *(uint64_t*)(&y);
        default:
            return *(uint64_t*)(&x) == *(uint64_t*)(&y);
    }
}
)


