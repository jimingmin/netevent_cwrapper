/*
 * net_ctype.h
 *
 *  Created on: 2015��02��02��
 *      Author: jimm
 */

#include "../netevent/net_typedef.h"
#include "list.h"
#include "lock.h"

struct PacketList
{
	SessionID			nSessionID;
	uint16_t			nPacketSize;
	uint8_t				*pPacketData;
	struct list_head	list;
};

#define MAX_SERVER_ADDRESS_LEN		256
struct ServerList
{
	char				szAddress[MAX_SERVER_ADDRESS_LEN];
	uint16_t			nPort;
	struct list_head	list;
};

struct NetContext
{
	struct NetFuncEntry *pNetFuncEntry;
	struct CAcceptor	*pAcceptor;
	struct CConnector	*pConnector;
	struct CNetHandler	*pNetHandler;
	LOCK_HANDLE			stSendLock;
	struct list_head	*pSendList;
	LOCK_HANDLE			stRecvLock;
	struct list_head	*pRecvList;
	LOCK_HANDLE			stServerLock;
	struct list_head	*pServerList;
};

