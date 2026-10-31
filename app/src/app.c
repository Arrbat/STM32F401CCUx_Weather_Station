#include "app.h"
#include "logger.h"

enum app_status
app_init(void)
{
    logger_init();
    if (logger_log(LOG_LEVEL_INFO, "boot done") != LOGGER_OK)
    {
        return APP_ERR_LOGGER;
    }

    return APP_OK;
}
