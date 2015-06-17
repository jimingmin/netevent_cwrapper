/*
 * net_loop.c
 *
 *  Created on: 2015年02月02日
 *      Author: jimm
 */

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "net_loop.h"
#include "net_func_wrapper.h"
#include "net_interface.h"
#include "net_timer.h"
#include "list.h"
#include "msg_hook.h"
#include "logger_extern.h"
#include "event_dump.h"

EXPORT struct NetContext *g_pNetContext;
EXPORT LOCK_HANDLE g_hRecvLock;
EXPORT uint32_t g_nMagicNum;

#define RANDOM_RANGE		1000000

int32_t net_init(const char *log_dir, const char *log_name)
{
	srand((unsigned int)time(0));

	g_pNetContext = (struct NetContext *)malloc(sizeof(struct NetContext));

	if(log_dir != NULL)
	{
		g_pNetContext->pLogDir = (char *)malloc(strlen(log_dir) + 1);
		strcpy(g_pNetContext->pLogDir, log_dir);
		g_pNetContext->pLogDir[strlen(log_dir)] = '\0';

		g_pNetContext->pLogName = (char *)malloc(strlen(log_name) + 1);
		strcpy(g_pNetContext->pLogName, log_name);
		g_pNetContext->pLogName[strlen(log_name)] = '\0';

		set_log_dir(log_dir);
		start_log_thread();
	}
	else
	{
		g_pNetContext->pLogDir = NULL;
		g_pNetContext->pLogName = NULL;
	}

	//注册回调接口
	g_pNetContext->pNetFuncEntry = regist_interface(func_net_parser, func_net_accepted, func_net_connected,
		func_net_connect_timeout, func_net_recved, func_net_writen, func_net_closed,
		func_net_error);

	//初始化网络环境
	g_pNetContext->pNetHandler = init_context_wrapper(log_dir, log_name);
	//初始化接收器
	g_pNetContext->pAcceptor = create_acceptor_wrapper(g_pNetContext->pNetHandler);
	//初始化连接器
	g_pNetContext->pConnector = create_connector_wrapper(g_pNetContext->pNetHandler);
	//初始化退出标志
	g_pNetContext->nIsStop = 0;

	g_pNetContext->stSendLock = create_lock();
	//初始化发送链表
	g_pNetContext->pSendList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pSendList);

	if(g_hRecvLock == NULL)
	{
		g_hRecvLock = create_lock();
	}

	//初始化接收链表
	g_pNetContext->pRecvList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pRecvList);

	g_pNetContext->stServerLock = create_lock();
	//初始化服务器列表
	g_pNetContext->pServerList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pServerList);

	net_timer_init();

	init_msg_hook();

	g_nMagicNum = rand() % RANDOM_RANGE + 1;

	dump_string("net init...");

	return 0;
}

int32_t net_connect_server()
{
	struct list_head *pos, *backup;
	struct ServerList *server;

	if(list_empty(g_pNetContext->pServerList))
	{
		return 0;
	}

	lock(g_pNetContext->stServerLock);

	if(list_empty(g_pNetContext->pServerList))
	{
		unlock(g_pNetContext->stServerLock);
		return 0;
	}

	list_for_each_safe(pos, backup, g_pNetContext->pServerList)
	{
		server = list_entry(pos, struct ServerList, list);
		net_connect_wrapper(g_pNetContext->pConnector, server->szAddress, server->nPort);

		list_del(pos);
		free(server);
	}

	unlock(g_pNetContext->stServerLock);

	return 0;
}

int32_t net_start_server()
{
	net_bind_wrapper(g_pNetContext->pAcceptor, NULL, 10000);
	return 0;
}

int32_t net_stop()
{
    if (g_pNetContext != NULL)
    {
        g_pNetContext->nIsStop = 1;
    }

    dump_string("net stop...");
	return 0;
}

int32_t net_send_data()
{
	struct list_head *pos, *backup;
	struct PacketList *packet;
	uint32_t offset;
	struct event_head head;

	if(list_empty(g_pNetContext->pSendList))
	{
		return 0;
	}

	lock(g_pNetContext->stSendLock);

	if(list_empty(g_pNetContext->pSendList))
	{
		unlock(g_pNetContext->stSendLock);
		return 0;
	}

	list_for_each_safe(pos, backup, g_pNetContext->pSendList)
	{
		packet = list_entry(pos, struct PacketList, list);

	    offset = 0;
	    decode_event_head(packet->pPacketData, packet->nPacketSize, &offset, &head);
		event_dump_head(g_pNetContext->pLogName, "send", &head);

		net_write(g_pNetContext->pNetHandler, packet->nSessionID, packet->pPacketData, packet->nPacketSize);

		list_del(pos);
		free(packet->pPacketData);
		free(packet);
	}

	unlock(g_pNetContext->stSendLock);

	return 0;
}

int32_t net_loop()
{
	net_start_server();

	while(!g_pNetContext->nIsStop)
	{
		net_connect_server();

		net_send_data();

		net_loop_timer();

		net_run_wrapper(g_pNetContext->pNetHandler);
	}
    
    net_uninit();

	return 0;
}

void net_uninit()
{
	dump_string("net uninit...");
	if(g_pNetContext != NULL)
	{
		destory_connector_wrapper(g_pNetContext->pConnector);
		destory_acceptor_wrapper(g_pNetContext->pAcceptor);
		uninit_context_wrapper(g_pNetContext->pNetHandler);
		unregist_interface(g_pNetContext->pNetFuncEntry);

		destory_lock(g_pNetContext->stServerLock);
		destory_lock(g_pNetContext->stSendLock);

		if(g_pNetContext->pLogDir != NULL)
		{
			free(g_pNetContext->pLogDir);
			g_pNetContext->pLogDir = NULL;

			stop_log_thread();
		}

		if(g_pNetContext->pLogName != NULL)
		{
			free(g_pNetContext->pLogName);
			g_pNetContext->pLogName = NULL;
		}

		free(g_pNetContext);
		g_pNetContext = NULL;
	}

	net_timer_uninit();

	uninit_msg_hook();
}
