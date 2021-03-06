/*
 * net_ctype.h
 *
 *  Created on: 2015��02��02��
 *      Author: jimm
 */

#ifndef NET_CTYPE_H_
#define NET_CTYPE_H_

#include "../netevent/net_typedef.h"
#include "list.h"
#include "lock.h"

#define MAX_PACKET_SIZE		(64 * 1024 - 1)

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
	int32_t				nIsStop;
	LOCK_HANDLE			stSendLock;
	struct list_head	*pSendList;
//	LOCK_HANDLE			stRecvLock;
	struct list_head	*pRecvList;
	LOCK_HANDLE			stServerLock;
	struct list_head	*pServerList;
	char				*pLogDir;
	char				*pLogName;
};

struct HeartbeatTimerData
{
	uint32_t			nUin;
	SessionID			nSessionID;
	int32_t				nMissCount;
};

enum enmSyncFlag
{
	enmSyncFlag_Sync		= 0x00,
	enmSyncFlag_Fin		= 0x01,
} SyncFlag;

typedef int32_t (*sync_msg_proc)(uint32_t sessionid, uint8_t *buf, int32_t buf_size);

#endif /* NET_CTYPE_H_ */
