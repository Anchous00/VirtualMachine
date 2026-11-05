#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <time.h>

#ifdef LOG_ON
#define ON_LOG(...) __VA_ARGS__
#else
#define ON_LOG(...)
#endif

enum log_status
{
    myLOG_INFO    = 1,
    myLOG_ERROR   = 2,
    myLOG_ACTION  = 3,
    myLOG_DEFAULT = 4
};

const int MAX_LOG_MESSAGE_LEN = 100;
//TODO LOG(log_status, format_string, ...);
#define LOG(format_string, ...) do                                                  \
        {                                                                           \
        char log_message[MAX_LOG_MESSAGE_LEN] = {};                                 \
        snprintf(log_message, MAX_LOG_MESSAGE_LEN, format_string, __VA_ARGS__);     \
        log(myLOG_ACTION, log_message);                                               \
        } while (0);

extern FILE *LOG_FILE;

void log( log_status status, const char* log_message );
void log_file_open( const char* log_file_name );
void log_file_close( void );

#ifndef COLOR_H
#define COLOR_H

#define myRED "\x1b[31m"
#define myWHITE "\x1b[0m"
#define myGREEN "\x1b[32m"
#define myYELLOW "\x1b[33m"
#define myBLUE "\x1b[34m"

#endif

#endif
