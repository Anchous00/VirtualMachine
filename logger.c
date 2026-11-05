#include "logger.h"

FILE *LOG_FILE = NULL;


void log( log_status status, const char* log_message )
{
    time_t t = time(NULL);
    struct tm *time_info = localtime(&t);

    char time_str[64];
    strftime(time_str, sizeof(time_str), "%d.%m.%Y %H:%M:%S", time_info);
    if (LOG_FILE == NULL)
    {
        LOG_FILE = stderr;
    }

    fprintf(LOG_FILE, "[%s]\t", time_str);

    switch (status)
    {
        case myLOG_INFO:
            fprintf(LOG_FILE, "INFO");
            break;
        case myLOG_ERROR:
            fprintf(LOG_FILE, "ERROR");
            break;
        case myLOG_ACTION:
            fprintf(LOG_FILE, "ACTION");
            break;
        case myLOG_DEFAULT:
            break;
        default:
            break;
    }
    fprintf(LOG_FILE, "\t%s\n", log_message);

    return;
}

void log_file_open( const char* log_file_name )
{
    LOG_FILE = fopen(log_file_name, "w");
    if (LOG_FILE == NULL)
        fprintf(stderr, "ERROR: can't open %s log file\n", log_file_name);
    log(myLOG_ACTION, "Log start\n\n");
    return;
}

void log_file_close( void )
{
    log(myLOG_ACTION, "Log end");

    if (fclose(LOG_FILE) != 0)
        fprintf(LOG_FILE, "ERROR: can't close log file stream\n");

    return;
}

