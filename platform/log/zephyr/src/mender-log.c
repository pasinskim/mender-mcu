/**
 * @file      mender-log.c
 * @brief     Mender logging interface for Zephyr platform
 *
 * Copyright joelguittet and mender-mcu-client contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(mender, CONFIG_MENDER_LOG_LEVEL);

#include "mender-log.h"

mender_err_t
mender_log_init(void) {

    /* Nothing to do */
    return MENDER_OK;
}

/* IMPORTANT: Please note that the default size of internal Zephyr logger buffer is 1024 bytes; if we need log buffer size 
              to be more than that there is CONFIG_LOG_BUFFER_SIZE option allowing to extend the buffer size. */
#define LOG_BUFFER_SIZE_BYTES (128)

mender_err_t
mender_log_print(uint8_t level, const char *filename, const char *function, int line, char *format, ...) {

    (void)filename;
    (void)function;

    va_list args;
    va_start(args, format);

    char log_buff[LOG_BUFFER_SIZE_BYTES];
    int ret = vsnprintf(log_buff, LOG_BUFFER_SIZE_BYTES, format, args);

    if (ret < 0) {
        LOG_ERR("logger error: log message formatting failed: %d", ret);
        va_end(args);
        return MENDER_FAIL;
    }

    /* Switch depending log level */
    switch (level) {
        case MENDER_LOG_LEVEL_ERR:
            LOG_ERR("(%d): %s", line, log_buff);
            break;
        case MENDER_LOG_LEVEL_WRN:
            LOG_WRN("(%d): %s", line, log_buff);
            break;
        case MENDER_LOG_LEVEL_INF:
            LOG_INF("(%d): %s", line, log_buff);
            break;
        case MENDER_LOG_LEVEL_DBG:
            LOG_DBG("(%d): %s", line, log_buff);
            break;
        default:
            break;
    }

    va_end(args);
    return MENDER_OK;
}

mender_err_t
mender_log_exit(void) {

    /* Nothing to do */
    return MENDER_OK;
}
