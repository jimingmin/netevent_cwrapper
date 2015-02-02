/*
 * net_interface.c
 *
 *  Created on: 2015Äê02ÔÂ01ÈÕ
 *      Author: jimm
 */

#include <stdlib.h>
#include <string.h>

#include "net_interface.h"
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
	*pPacketSize = nBufSize;
	memcpy(arrPacket, arrBuf, *pPacketSize);

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

int32_t func_net_read(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	func_net_write(nSessionID, pData, nBytes);
	return 0;
}

int32_t func_net_writen(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return 0;
}

int32_t func_net_closed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t func_net_error(SessionID nSessionID, int32_t nErrorID)
{
	return 0;
}

int32_t func_net_write(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	struct PacketList *packet = (struct PacketList *)malloc(sizeof(struct PacketList));
	packet->nSessionID = nSessionID;
	packet->pPacketData = (uint8_t *)malloc(nBytes);
	memcpy(packet->pPacketData, pData, nBytes);
	packet->nPacketSize = nBytes;

	list_add_tail(&packet->list, g_pNetContext->pSendList);
	return 0;
}

