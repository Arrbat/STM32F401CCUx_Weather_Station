#ifndef APP_H
#define APP_H

enum app_status
{
    APP_OK = 0,
    APP_ERR_LOGGER,
};

enum app_status app_init(void);

#endif /* APP_H */
