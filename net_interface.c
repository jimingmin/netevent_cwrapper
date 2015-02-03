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

extern struct NetContext *g_pNetContext;

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
	return nBufSize;
}

int32_t func_net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t func_net_connected(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t func_net_connect_timeout(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
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
	struct PacketList *packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = nSessionID;
	packet->pPacketData = (uint8_t *)malloc(nBytes);
	memcpy(packet->pPacketData, pData, nBytes);
	packet->nPacketSize = nBytes;

	lock(g_pNetContext->stRecvLock);
	list_add_tail(&packet->list, g_pNetContext->pRecvList);
	unlock(g_pNetContext->stRecvLock);

	return 0;
}

int32_t func_net_write(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	struct PacketList *packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = nSessionID;
	packet->pPacketData = (uint8_t *)malloc(nBytes);
	memcpy(packet->pPacketData, pData, nBytes);
	packet->nPacketSize = nBytes;

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
	return 0;
}

int32_t func_net_error(SessionID nSessionID, int32_t nErrorID)
{
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