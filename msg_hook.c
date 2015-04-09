/*
 * msg_hook.c
 *
 *  Created on: 2015Äê4ÔÂ8ÈÕ
 *      Author: jimm
 */

#include "msg_hook.h"
#include <stdlib.h>
#include <string.h>
#include "../common/common_export.h"
#include "msg_event.h"
#include "system_event.h"
#include "event_format.h"
#include "list.h"
#include "net_interface.h"

#define MAX_MSG_PROC_SIZE		64

struct sync_context
{
	int32_t				proc_count;
	int32_t				msgid_array[MAX_MSG_PROC_SIZE];
	sync_msg_proc		proc_array[MAX_MSG_PROC_SIZE];
	struct list_head	*sync_msg_list;
	int32_t				msg_count;
	uint32_t				sync_seq;
	int32_t				sync_step;
};

EXPORT struct sync_context *g_syncctx;

int32_t init_msg_hook()
{
	g_syncctx = (struct sync_context *)malloc(sizeof(struct sync_context));
	g_syncctx->proc_count = 0;
	g_syncctx->msg_count = 0;
	g_syncctx->sync_seq = 0;
	g_syncctx->sync_step = 1000;

	int32_t proc_count = 0;
	g_syncctx->msgid_array[proc_count] = MSGID_STATUSSYNC_NOTI;
	g_syncctx->proc_array[proc_count] = handle_statussync_noti;
	proc_count++;

	g_syncctx->msgid_array[proc_count] = MSGID_MSGPUSH_NOTI;
	g_syncctx->proc_array[proc_count] = handle_msgpush_noti;
	proc_count++;

	g_syncctx->msgid_array[proc_count] = MSGID_MSGFINACK_RESP;
	g_syncctx->proc_array[proc_count] = handle_msgfinack_resp;
	proc_count++;

	g_syncctx->proc_count = proc_count;

	g_syncctx->sync_msg_list = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_syncctx->sync_msg_list);

	return 0;
}

void uninit_msg_hook()
{
}

int32_t msg_hook(int32_t msgid, SessionID nSessionID, uint8_t *buf, int32_t buf_size)
{
	sync_msg_proc proc = get_msg_hook(msgid);
	if(proc != NULL)
	{
		proc(nSessionID, buf, buf_size);
		return 1;
	}
	return 0;
}

sync_msg_proc get_msg_hook(int32_t msgid)
{
	int32_t i = 0;
	for(; i < g_syncctx->proc_count; ++i)
	{
		if(msgid == g_syncctx->msgid_array[i])
		{
			return g_syncctx->proc_array[i];
		}
	}

	return NULL;
}

void clear_msg_list()
{
	struct list_head *pos, *backup;
	struct PacketList *packet;

	list_for_each_safe(pos, backup, g_syncctx->sync_msg_list)
	{
		packet = list_entry(pos, struct PacketList, list);

		list_del(pos);

		free(packet->pPacketData);
		free(packet);
	}

	g_syncctx->msg_count = 0;
}

int32_t handle_statussync_noti(uint32_t sessionid, uint8_t *buf, int32_t buf_size)
{
	uint32_t offset;

	struct event_head head;
	struct statussync_noti noti;

	struct event_head send_head;
	struct msgsync_req send_req;

	uint8_t send_buf[1024];

	clear_msg_list();

	offset = 0;
	decode_event_head(buf, buf_size, &offset, &head);
	decode_statussync_noti(buf, buf_size, &offset, &noti);

    send_head.event_id = MSGID_MSGSYNC_REQ;
    send_head.seq = 0;
	send_head.src_uin = head.dst_uin;
    send_head.dst_uin = 0;

	send_req.sync_seq= (g_syncctx->sync_seq + (rand() % g_syncctx->sync_step) + 1);

	offset = 0;

	encode_event_head(send_buf, sizeof(send_buf), &offset, &send_head);
	encode_msgsync_req(send_buf, sizeof(send_buf), &offset, &send_req);

	return func_net_write(sessionid, send_buf, offset);
}

int32_t handle_msgpush_noti(uint32_t sessionid, uint8_t *buf, int32_t buf_size)
{
	uint32_t offset;

	struct event_head head;
	struct msgpush_noti noti;

	struct event_head send_head;
	struct msgfinack_req send_req;

	uint8_t send_buf[1024];

	struct PacketList *packet;

	offset = 0;
	decode_event_head(buf, buf_size, &offset, &head);
	decode_msgpush_noti(buf, buf_size, &offset, &noti);

	packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = sessionid;
	packet->pPacketData = (uint8_t *)malloc(noti.m_nMsgSize);
	memcpy(packet->pPacketData, noti.m_arrMsg, noti.m_nMsgSize);
	packet->nPacketSize = noti.m_nMsgSize;

	list_add_tail(&packet->list, g_syncctx->sync_msg_list);
	++g_syncctx->msg_count;

	if(noti.m_nSyncFlag == enmSyncFlag_Fin)
	{
		offset = 0;

        send_head.event_id = MSGID_MSGFINACK_REQ;
        send_head.seq = 0;
        send_head.src_uin = head.dst_uin;
        send_head.dst_uin = 0;
		encode_event_head(send_buf, sizeof(send_buf), &offset, &send_head);

		send_req.sync_count = g_syncctx->msg_count;
		send_req.sync_seq = noti.m_nSyncFlag;
		encode_msgfinack_req(send_buf, sizeof(send_buf), &offset, &send_req);

		func_net_write(sessionid, send_buf, offset);
	}

	return 0;
}

int32_t handle_msgfinack_resp(uint32_t sessionid, uint8_t *buf, int32_t buf_size)
{
	struct list_head *pos, *backup;
	struct PacketList *packet;

	list_for_each_safe(pos, backup, g_syncctx->sync_msg_list)
	{
		packet = list_entry(pos, struct PacketList, list);
		push_read_queue(sessionid, packet->pPacketData, packet->nPacketSize);
	}

	clear_msg_list();

	return 0;
}


