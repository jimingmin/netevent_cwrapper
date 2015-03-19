/*
 * net_health.h
 *
 *  Created on: Mar 17, 2015
 *      Author: jimm
 */

#ifndef NET_HEALTH_H_
#define NET_HEALTH_H_

#include "../netevent/net_typedef.h"
#include "net_ctype.h"

int32_t send_ping(SessionID nSessionID, uint32_t uin);

int32_t recv_pong(SessionID nSessionID);

int32_t check_net_health(void *pTimerData);


#endif /* NET_HEALTH_H_ */
