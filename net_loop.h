/*
 * net_loop.h
 *
 *  Created on: 2015Äê02ÔÂ02ÈÕ
 *      Author: jimm
 */

#include "net_ctype.h"
#include "../common/common_export.h"

//extern struct NetContext *g_pNetContext;

int32_t net_init(const char *log_dir, const char *log_name);

int32_t net_connect_server();

int32_t net_start_server();

int32_t net_stop();

int32_t net_send_data();

int32_t net_loop();

void net_uninit();
