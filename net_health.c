/*
 * net_health.c
 *
 *  Created on: Mar 17, 2015
 *      Author: jimm
 */

#include "net_health.h"
#include "net_timer.h"
#include "system_event.h"
#include "event_format.h"
#include "net_interface.h"

int32_t send_ping(SessionID nSessionID)
{
	uint8_t szPacket[MAX_PACKET_SIZE];
	struct event_head head;
	head.total_size = 0;
	head.event_id = NETEVT_PING;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	uint32_t offset = 0;
	encode_event_head(szPacket, sizeof(szPacket), &offset, &head);

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
		func_net_close(pHeartbeat->nSessionID);
	}
    else
    {
        send_ping(pHeartbeat->nSessionID);
        pHeartbeat->nMissCount++;
    }
	return 0;
}

