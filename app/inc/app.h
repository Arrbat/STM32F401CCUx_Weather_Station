#ifndef APP_H
#define APP_H

#include "gcc.h"

enum app_status
{
    APP_OK = 0,
    APP_ERR_LOGGER,
};

WS_MUST_CHECK enum app_status app_init(void);

#endif /* APP_H */
