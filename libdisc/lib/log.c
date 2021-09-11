

#include "log.h"

#define __LD_LOG(logtype)          \
    if ((_ld_ll & (logtype)) == 0) \
    {                              \
        return;                    \
    }                              \
    va_list myargs;                \
    va_start(myargs, message);     \
    _ld_log(logtype, _ld_ll, message, myargs)

static unsigned long _ld_ll = LD_LOG_ERROR | LD_LOG_WARNING | LD_LOG_NOTICE | LD_LOG_INFO;

unsigned long ld_set_logging_level(unsigned long log_level)
{
    _ld_ll = log_level;
    return _ld_ll;
}

void ld_error(const char *message, ...)
{
    __LD_LOG(LD_LOG_ERROR);
}

void ld_warning(const char *message, ...)
{
    __LD_LOG(LD_LOG_WARNING);
}

void ld_info(const char *message, ...)
{
    __LD_LOG(LD_LOG_INFO);
}

void ld_notice(const char *message, ...)
{
    __LD_LOG(LD_LOG_NOTICE);
}

void ld_debug(const char *message, ...)
{
    __LD_LOG(LD_LOG_DEBUG);
}

const char *ld_log_level_string(unsigned long ll)
{
    switch (ll)
    {
    case LD_LOG_ERROR:
        return "ERROR";
    case LD_LOG_WARNING:
        return "WARNING";
    case LD_LOG_INFO:
        return "INFO";
    case LD_LOG_NOTICE:
        return "NOTICE";
    case LD_LOG_DEBUG:
        return "DEBUG";
    default:

        return "UNKNOWN(?)";
    }
}

void _ld_log(unsigned long ll, unsigned long enabled_levels, const char *log_message, va_list arg)
{
    if ((ll & enabled_levels) == 0)
        return;

    char *message;

    message = ld_log_generate_string(ll, log_message, arg);
    if (message == NULL)
    {
        perror("libdiscord counldn't print an error message");
        return;
    }
    fprintf(stderr, "%s\n", message);
    free(message);
}

char *ld_log_generate_string(unsigned long ll, const char *log_message, va_list arg)
{
    time_t raw_time;
    struct tm *current_time;
    char *message, *time_string;

    time_string = malloc(30 * sizeof(char));
    time(&raw_time);
    current_time = localtime(&raw_time);
    asctime_r(current_time, time_string);
    time_string[24] = '\0';

    char msg[16000];
    int ret;

    ret = vsprintf(msg, log_message, arg);

    if (ret < 0)
    {
        perror("libdiscord: couldn't create log message");
        return NULL;
    }

    message = malloc(strlen(time_string) + strlen(ld_log_level_string(ll)) + strlen(msg) + 15);
    sprintf(message, "[%s] LD_%s: %s", time_string, ld_log_level_string(ll), msg);
    free(time_string);
    return message;
}

unsigned long ld_get_logging_level() { return _ld_ll; };
