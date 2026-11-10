#ifndef LOGGER_H
#define LOGGER_H

#include "gcc.h"
#include <stddef.h>
#include <stdint.h>

enum log_level
{
    LOG_LEVEL_NONE = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_WARNING,
    LOG_LEVEL_INFO,
    LOG_LEVEL_DEBUG,
};

/* Maximum length of one output frame in bytes, including the final "\r\n". */
enum
{
    MAX_LOG_LENGTH = 128,
};

enum logger_status
{
    LOGGER_OK = 0,
    LOGGER_ERR_PARAM,
    LOGGER_ERR_NOT_INIT,
    LOGGER_ERR_ALREADY_INIT,
    LOGGER_ERR_BUSY,
    LOGGER_ERR_DRIVER,
};

/* Thread mode only. Not safe to call from ISR. */
WS_MUST_CHECK enum logger_status logger_log(enum log_level level,
                                            const char *message);
WS_MUST_CHECK enum logger_status logger_init(void);

#endif /* LOGGER_H */
