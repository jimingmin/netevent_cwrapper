/*
 * net_interface.c
 *
 *  Created on: 2015年02月01日
 *      Author: jimm
 */

#include <stdlib.h>
#include <string.h>

#include "net_interface.h"
#include "net_func_wrapper.h"
#include "net_ctype.h"
#include "code_extern.h"
#include "system_event.h"
#include "event_format.h"
#include "net_health.h"
#include "net_timer.h"
#include "msg_hook.h"
#include "event_dump.h"

extern struct NetContext *g_pNetContext;
extern LOCK_HANDLE g_hRecvLock;
extern uint32_t g_nMagicNum;

static char g_arrSSKey[16] = {'v', 'd', 'c', '$', 'a', 'u', 't', 'h', '@', '1', '7','9','.', 'c', 'o', 'm'};

struct NetFuncEntry *regist_interface(callback_net_parser func_net_parser, callback_net_accepted func_net_accepted,
	callback_net_connected func_net_connected, callback_net_connect_timeout func_net_connect_timeout,
	callback_net_read func_net_read, callback_net_writen func_net_writen,
	callback_net_closed func_net_closed, callback_net_error func_net_error)
{
	struct NetFuncEntry *pFuncEntry = (struct NetFuncEntry *)malloc(sizeof(struct NetFuncEntry));
	pFuncEntry->func_net_parser = func_net_parser;
	pFuncEntry->func_net_accepted = func_net_accepted;
	pFuncEntry->func_net_connected = func_net_connected;
	pFuncEntry->func_net_connect_timeout = func_net_connect_timeout;
	pFuncEntry->func_net_read = func_net_read;
	pFuncEntry->func_net_writen = func_net_writen;
	pFuncEntry->func_net_closed = func_net_closed;
	pFuncEntry->func_net_error = func_net_error;
	return pFuncEntry;
}

void unregist_interface(struct NetFuncEntry *pFuncEntry)
{
	free(pFuncEntry);
}

struct NetFuncEntry *GetNetFuncEntry()
{
	return g_pNetContext->pNetFuncEntry;
}

int32_t func_net_parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrOutputBuf[], int32_t nOutputBufSize)
{
	uint16_t nTotalSize = 0;
	uint32_t nOffset = 0;
	int32_t nRet = 0;
	nRet = decode_uint16_t((unsigned char *)arrBuf, nBufSize, &nOffset, &nTotalSize);
	if(nRet < 0)
	{
		return 0;
	}

	if(nTotalSize > nBufSize)
	{
		return 0;
	}

	memcpy(arrOutputBuf, arrBuf, nTotalSize);

	return nTotalSize;
}

int32_t func_net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_accepted accepted;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	head.event_id = SYSEVT_ACCEPTED;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	accepted.sessionid = nSessionID;
	strcpy(accepted.address, pPeerAddress);
	accepted.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_accepted(szPacket, sizeof(szPacket) - offset, &offset, &accepted);

	head.total_size = offset;
	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, head.total_size);

	return push_read_queue(nSessionID, szPacket, head.total_size);
}

int32_t func_net_connected(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_connected connected;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	event_dump_connected(g_pNetContext->pLogName, nSessionID, pPeerAddress, nPeerPort);

	head.event_id = SYSEVT_CONNECTED;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	connected.sessionid = nSessionID;
	strcpy(connected.address, pPeerAddress);
	connected.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_connected(szPacket, sizeof(szPacket) - offset, &offset, &connected);

	head.total_size = offset;
	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, head.total_size);

	return push_read_queue(nSessionID, szPacket, head.total_size);
}

int32_t func_net_connect_timeout(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_connecttimeout timeout;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	event_dump_connect_timeout(g_pNetContext->pLogName, nSessionID, pPeerAddress, nPeerPort);

	head.event_id = SYSEVT_CONNECTTIMEOUT;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	strcpy(timeout.address, pPeerAddress);
	timeout.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_connecttimeout(szPacket, sizeof(szPacket) - offset, &offset, &timeout);

	head.total_size = offset;
	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, head.total_size);

	return push_read_queue(nSessionID, szPacket, head.total_size);
}

int32_t func_net_read(SessionID *pSessionID, uint8_t *pData, int32_t *pBytes)
{
	struct list_head *pos, *backup;
	struct PacketList *packet;
	uint32_t offset;
	struct event_head head;

	lock(g_hRecvLock);

   if(g_pNetContext == NULL)
    {
	   unlock(g_hRecvLock);
	   return 0;
    }

	if(list_empty(g_pNetContext->pRecvList))
	{
		unlock(g_hRecvLock);
		return 0;
	}

	list_for_each_safe(pos, backup, g_pNetContext->pRecvList)
	{
		packet = list_entry(pos, struct PacketList, list);
		break;
	}

    offset = 0;
    decode_event_head(packet->pPacketData, packet->nPacketSize, &offset, &head);
	event_dump_head(g_pNetContext->pLogName, "read", &head);

	*pSessionID = packet->nSessionID;
	memcpy(pData, packet->pPacketData, packet->nPacketSize);
	*pBytes = packet->nPacketSize;

	list_del(&packet->list);

	free(packet->pPacketData);
	free(packet);

	unlock(g_hRecvLock);

	return packet->nPacketSize;
}

int32_t func_net_recved(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	uint8_t szPacket[MAX_PACKET_SIZE];
	uint16_t body_size;
	uint16_t packet_size;
	uint32_t offset;
	uint8_t result;
	struct HeartbeatTimerData *pTimerData;
	struct event_head head;
	uint16_t head_size = event_head_size();

	if(head_size < nBytes)
	{
		body_size = func_decrypt((char *)&pData[head_size], nBytes - head_size,
				(char *)&szPacket[head_size], sizeof(szPacket) - head_size, g_arrSSKey);
		if(body_size <= 0)
		{
			dump_string("decrypt failed!");
			return 0;
		}
	}

	memcpy(szPacket, pData, head_size);

	packet_size = (head_size + body_size);

	offset = 0;
	decode_event_head(szPacket, packet_size, &offset, &head);

	head.total_size = packet_size;
	event_dump_head(g_pNetContext->pLogName, "recv", &head);
	//如果是pong包
	if(head.event_id == NETEVT_PONG)
	{
		return recv_pong(nSessionID);
	}
	else if((head.event_id == MSG_VERIFY_CODE_RESP) || (head.event_id == MSG_USER_LOGIN_RESP))
	{
		offset = event_head_size();
		result = 0;
		decode_uint8_t(szPacket, packet_size, &offset, &result);
		if(result == 0)
		{
			pTimerData = (struct HeartbeatTimerData *)malloc(sizeof(struct HeartbeatTimerData));
			pTimerData->nUin = head.src_uin;
			pTimerData->nSessionID = nSessionID;
		    pTimerData->nMissCount = 0;

		    dump_string("start health timer");

			net_create_timer(check_net_health, pTimerData, 120, 1);
		}
	}

	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, packet_size);

	if(!msg_hook(head.event_id, nSessionID, szPacket, packet_size))
	{
		return push_read_queue(nSessionID, szPacket, packet_size);
	}

	return 0;
}

int32_t func_net_write(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	uint8_t szRawData[MAX_PACKET_SIZE];
	uint8_t szBody[MAX_PACKET_SIZE];

	uint8_t head_size;
	struct PacketList *packet;
	struct event_head head;
	int32_t nRawDataSize = nBytes;
	uint32_t offset = 0;
	uint16_t body_size = 0;

	memcpy(szRawData, pData, nBytes);
	++g_nMagicNum;
	memcpy(&szRawData[nRawDataSize], &g_nMagicNum, sizeof(g_nMagicNum));
	nRawDataSize += sizeof(g_nMagicNum);

	head_size = event_head_size();
	if(head_size < nBytes)
	{
		body_size = func_encrypt((char *)&szRawData[head_size], nRawDataSize - head_size,
				(char *)szBody, sizeof(szBody), g_arrSSKey);
		if(body_size <= 0)
		{
			dump_string("encrypt failed!");
			return 0;
		}
	}

	packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->pPacketData = (uint8_t *)malloc(head_size + body_size);

	packet->nSessionID = nSessionID;
	memcpy(packet->pPacketData, szRawData, head_size);
	memcpy(&packet->pPacketData[head_size], szBody, body_size);

	packet->nPacketSize = head_size + body_size;
	encode_uint16_t(packet->pPacketData, packet->nPacketSize, &offset, packet->nPacketSize);

    offset = 0;
    decode_event_head(packet->pPacketData, packet->nPacketSize, &offset, &head);
	event_dump_head(g_pNetContext->pLogName, "write", &head);

	lock(g_pNetContext->stSendLock);
	list_add_tail(&packet->list, g_pNetContext->pSendList);
	unlock(g_pNetContext->stSendLock);

	return (head_size + body_size);
}

int32_t func_net_writen(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return 0;
}

int32_t func_net_close(SessionID nSessionID)
{
	event_dump_closing(g_pNetContext->pLogName, nSessionID);
	return net_close_wrapper(g_pNetContext->pNetHandler, nSessionID);
}

int32_t func_net_closed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_closed closed;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	event_dump_closed(g_pNetContext->pLogName, nSessionID, pPeerAddress, nPeerPort);

	head.event_id = SYSEVT_CLOSED;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	closed.sessionid = nSessionID;
	strcpy(closed.address, pPeerAddress);
	closed.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket), &offset, &head);
	encode_event_closed(szPacket, sizeof(szPacket), &offset, &closed);

	head.total_size = offset;
	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, head.total_size);

	dump_string("stop health timer\n");
	net_destroy_timer_by_sessionid(nSessionID);

	return push_read_queue(nSessionID, szPacket, head.total_size);
}

int32_t func_net_error(SessionID nSessionID, int32_t nErrorID)
{
	struct event_head head;
	struct event_error error;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	head.event_id = SYSEVT_CONNECTTIMEOUT;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	error.sessionid = nSessionID;
	error.error_code = nErrorID;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_error(szPacket, sizeof(szPacket) - offset, &offset, &error);

	head.total_size = offset;
	offset = 0;
	encode_uint16_t(szPacket, sizeof(szPacket), &offset, head.total_size);

	return push_read_queue(nSessionID, szPacket, head.total_size);
}

int32_t func_net_connect(char *addr, uint16_t port)
{
	struct ServerList *pServerList = (struct ServerList *)malloc(sizeof(struct ServerList));
	strcpy(pServerList->szAddress, addr);
	pServerList->nPort = port;

	lock(g_pNetContext->stServerLock);
	list_add_tail(&pServerList->list, g_pNetContext->pServerList);
	unlock(g_pNetContext->stServerLock);

	event_dump_connecting(g_pNetContext->pLogName, addr, port);
	return 0;
}

int32_t push_read_queue(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	struct PacketList *packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = nSessionID;
	packet->pPacketData = (uint8_t *)malloc(nBytes);
	memcpy(packet->pPacketData, pData, nBytes);
	packet->nPacketSize = nBytes;

	lock(g_hRecvLock);
	list_add_tail(&packet->list, g_pNetContext->pRecvList);
	unlock(g_hRecvLock);

	return nBytes;
}
