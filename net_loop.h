/*
 * net_loop.h
 *
 *  Created on: 2015Äê02ÔÂ02ÈÕ
 *      Author: jimm
 */

#include "net_ctype.h"
#include "../common/common_export.h"

//extern struct NetContext *g_pNetContext;

EXPORT int32_t net_init(const char *log_dir, const char *log_name);

EXPORT int32_t net_connect_server();

EXPORT int32_t net_start_server();

EXPORT int32_t net_stop();

EXPORT int32_t net_send_data();

EXPORT int32_t net_loop();

EXPORT void net_uninit();
