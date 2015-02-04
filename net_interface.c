/*
 * net_interface.c
 *
 *  Created on: 2015Äê02ÔÂ01ÈÕ
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

extern struct NetContext *g_pNetContext;
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

int32_t func_net_parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t *pPacketSize)
{
	uint16_t nTotalSize = 0;
	uint32_t nOffset = 0;
	int32_t nRet = 0;
	nRet = decode_uint16((unsigned char *)arrBuf, nBufSize, &nOffset, &nTotalSize);
	if(nRet < 0)
	{
		return 0;
	}

	if(nTotalSize > nBufSize)
	{
		return 0;
	}

	*pPacketSize = nTotalSize;
	memcpy(arrPacket, arrBuf, nTotalSize);

	return nTotalSize;
}

int32_t func_net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t func_net_connected(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_connected connected;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	head.event_id = SYSEVT_CONNECTED;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	strcpy(connected.address, pPeerAddress);
	connected.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_connected(szPacket, sizeof(szPacket) - offset, &offset, &connected);

	head.total_size = offset;
	offset = 0;
	encode_uint16(szPacket, sizeof(szPacket), &offset, head.total_size);

	push_read_queue(nSessionID, szPacket, offset);
	return 0;
}

int32_t func_net_connect_timeout(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_connecttimeout timeout;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

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
	encode_uint16(szPacket, sizeof(szPacket), &offset, head.total_size);

	push_read_queue(nSessionID, szPacket, offset);
	return 0;
}

int32_t func_net_read(SessionID *pSessionID, uint8_t *pData, int32_t *pBytes)
{
	struct list_head *pos, *backup;
	struct PacketList *packet;

	if(list_empty(g_pNetContext->pRecvList))
	{
		return 0;
	}

	lock(g_pNetContext->stRecvLock);

	if(list_empty(g_pNetContext->pRecvList))
	{
		unlock(g_pNetContext->stRecvLock);
		return 0;
	}

	list_for_each_safe(pos, backup, g_pNetContext->pRecvList)
	{
		packet = list_entry(pos, struct PacketList, list);
		break;
	}

	*pSessionID = packet->nSessionID;
	memcpy(pData, packet->pPacketData, packet->nPacketSize);
	*pBytes = packet->nPacketSize;

	free(packet->pPacketData);
	free(packet);

	list_del(&packet->list);

	unlock(g_pNetContext->stRecvLock);

	return 0;
}

int32_t func_net_recved(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	uint8_t szPacket[MAX_PACKET_SIZE];
	uint16_t body_size;
	uint8_t head_size = event_head_size();

	body_size = decrypt((char *)&pData[head_size], nBytes - head_size,
			(char *)&szPacket[head_size], sizeof(szPacket) - head_size, g_arrSSKey);
	if(body_size <= 0)
	{
		return 0;
	}

	memcpy(szPacket, pData, head_size);
	push_read_queue(nSessionID, szPacket, head_size + body_size);
	return 0;
}

int32_t func_net_write(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	uint8_t head_size = 0;
	struct PacketList *packet = NULL;
	uint8_t szPacket[MAX_PACKET_SIZE];
	uint16_t body_size = 0;
	uint32_t offset = 0;
	head_size = event_head_size();
	body_size = encrypt((char *)&pData[head_size], nBytes - head_size,
			(char *)&szPacket[head_size], sizeof(szPacket) - head_size, g_arrSSKey);
	if(body_size <= 0)
	{
		return 0;
	}

	packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->pPacketData = (uint8_t *)malloc(head_size + body_size);

	packet->nSessionID = nSessionID;
	memcpy(packet->pPacketData, pData, head_size);
	memcpy(&packet->pPacketData[head_size], szPacket, body_size);

	packet->nPacketSize = head_size + body_size;
	encode_uint16(packet->pPacketData, packet->nPacketSize, &offset, packet->nPacketSize);

	lock(g_pNetContext->stSendLock);
	list_add_tail(&packet->list, g_pNetContext->pSendList);
	unlock(g_pNetContext->stSendLock);

	return 0;
}

int32_t func_net_writen(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return 0;
}

int32_t func_net_close(SessionID nSessionID)
{
	return net_close_wrapper(g_pNetContext->pNetHandler, nSessionID);
}

int32_t func_net_closed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	struct event_head head;
	struct event_closed closed;
	uint32_t offset = 0;
	uint8_t szPacket[MAX_PACKET_SIZE];

	head.event_id = SYSEVT_CLOSED;
	head.seq = 0;
	head.src_uin = 0;
	head.dst_uin = 0;

	strcpy(closed.address, pPeerAddress);
	closed.port = nPeerPort;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_closed(szPacket, sizeof(szPacket) - offset, &offset, &closed);

	head.total_size = offset;
	offset = 0;
	encode_uint16(szPacket, sizeof(szPacket), &offset, head.total_size);

	push_read_queue(nSessionID, szPacket, offset);
	return 0;
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

	error.error_code = nErrorID;

	encode_event_head(szPacket, sizeof(szPacket) - offset, &offset, &head);
	encode_event_error(szPacket, sizeof(szPacket) - offset, &offset, &error);

	head.total_size = offset;
	offset = 0;
	encode_uint16(szPacket, sizeof(szPacket), &offset, head.total_size);

	push_read_queue(nSessionID, szPacket, offset);
	return 0;
}

int32_t func_net_connect(char *addr, uint16_t port)
{
	struct ServerList *pServerList = (struct ServerList *)malloc(sizeof(struct ServerList));
	strcpy(pServerList->szAddress, addr);
	pServerList->nPort = port;

	lock(g_pNetContext->stServerLock);
	list_add_tail(&pServerList->list, g_pNetContext->pServerList);
	unlock(g_pNetContext->stServerLock);

	return 0;
}

void push_read_queue(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	struct PacketList *packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = nSessionID;
	packet->pPacketData = (uint8_t *)malloc(nBytes);
	memcpy(packet->pPacketData, pData, nBytes);
	packet->nPacketSize = nBytes;

	lock(g_pNetContext->stRecvLock);
	list_add_tail(&packet->list, g_pNetContext->pRecvList);
	unlock(g_pNetContext->stRecvLock);
}
