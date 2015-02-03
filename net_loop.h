/*
 * net_loop.h
 *
 *  Created on: 2015��02��02��
 *      Author: jimm
 */

#include "net_ctype.h"
#include "../common/common_export.h"

EXPORT struct NetContext *g_pNetContext;

int32_t net_init();

int32_t net_connect_server();

int32_t net_start_server();

int32_t net_send_data();

int32_t net_loop();

void net_uninit();
