/*
 * net_interface.h
 *
 *  Created on: 2015Äê02ÔÂ01ÈÕ
 *      Author: jimm
 */

#include "../common/common_export.h"
#include "../netevent/net_typedef.h"

struct NetFuncEntry *regist_interface(callback_net_parser func_net_parser, callback_net_accepted func_net_accepted,
	callback_net_connected func_net_connected, callback_net_connect_timeout func_net_connect_timeout,
	callback_net_read func_net_read, callback_net_writen func_net_writen,
	callback_net_closed func_net_closed, callback_net_error func_net_error);

void unregist_interface(struct NetFuncEntry *pFuncEntry);

struct NetFuncEntry *GetNetFuncEntry();

int32_t func_net_parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t *pPacketSize);

int32_t func_net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t func_net_connected(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t func_net_connect_timeout(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t func_net_read(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t func_net_writen(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t func_net_closed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t func_net_error(SessionID nSessionID, int32_t nErrorID);

int32_t func_net_write(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

