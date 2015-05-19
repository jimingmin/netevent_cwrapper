/*
 * net_health.c
 *
 *  Created on: Mar 17, 2015
 *      Author: jimm
 */

#include <time.h>
#include "net_health.h"
#include "net_timer.h"
#include "system_event.h"
#include "event_format.h"
#include "net_interface.h"
#include "net_extern.h"
#include "logger_extern.h"

extern struct NetContext *g_pNetContext;

int32_t send_ping(SessionID nSessionID, uint32_t uin)
{
	uint8_t szPacket[MAX_PACKET_SIZE];
	time_t now;
	uint32_t offset = 0;

	struct event_head head;
	head.total_size = 0;
	head.event_id = NETEVT_PING;
	head.seq = 0;
	head.src_uin = uin;
	head.dst_uin = 0;

	encode_event_head(szPacket, sizeof(szPacket), &offset, &head);

	time(&now);
	encode_uint32_t(szPacket, sizeof(szPacket), &offset, uin);
	encode_uint32_t(szPacket, sizeof(szPacket), &offset, (uint32_t)now);

	return func_net_write(nSessionID, szPacket, offset);
}

int32_t recv_pong(SessionID nSessionID)
{
	NetTimer *net_timer = net_find_timer(nSessionID);
	if(net_timer != NULL)
	{
		struct HeartbeatTimerData *pHeartbeat = (struct HeartbeatTimerData *)(net_timer->pTimerData);
		pHeartbeat->nMissCount = 0;
	}
	return 0;
}

int32_t check_net_health(void *pTimerData)
{
	struct HeartbeatTimerData *pHeartbeat = (struct HeartbeatTimerData *)pTimerData;
	if(pHeartbeat->nMissCount >= 3)
	{
		if(g_pNetContext->pLogName != NULL)
		{
			write_debug_log(g_pNetContext->pLogName, "heartbeat is miss more than 3");
		}
		func_net_close(pHeartbeat->nSessionID);
	}
   else
    {
	   time_t now = time(NULL);
	   if((get_last_send_time(g_pNetContext->pNetHandler, pHeartbeat->nSessionID) + 30) < now)
	   {
		   send_ping(pHeartbeat->nSessionID, pHeartbeat->nUin);
			pHeartbeat->nMissCount++;
	   }
    }
	return 0;
}

