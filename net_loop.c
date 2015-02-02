/*
 * net_loop.c
 *
 *  Created on: 2015年02月02日
 *      Author: jimm
 */

#include <stdlib.h>
#include <string.h>
#include "net_loop.h"
#include "net_func_wrapper.h"
#include "net_interface.h"
#include "list.h"

int32_t net_init()
{
	g_pNetContext = (struct NetContext *)malloc(sizeof(struct NetContext));

	//注册回调接口
	g_pNetContext->pNetFuncEntry = regist_interface(func_net_parser, func_net_accepted, func_net_connected,
		func_net_connect_timeout, func_net_read, func_net_writen, func_net_closed,
		func_net_error);

	//初始化网络环境
	g_pNetContext->pNetHandler = init_context_wrapper();
	//初始化接收器
	g_pNetContext->pAcceptor = create_acceptor_wrapper(g_pNetContext->pNetHandler);
	//初始化连接器
	g_pNetContext->pConnector = create_connector_wrapper(g_pNetContext->pNetHandler);
	//初始化发送链表
	g_pNetContext->pSendList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pSendList);
	//初始化服务器列表
	g_pNetContext->pServerList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pServerList);

	return 0;
}

void net_add_server(char *addr, uint16_t port)
{
	struct ServerList *pServerList = (struct ServerList *)malloc(sizeof(struct ServerList));
	strcpy(pServerList->szAddress, addr);
	pServerList->nPort = port;

	list_add_tail(&pServerList->list, g_pNetContext->pServerList);
}

int32_t net_connect_server()
{
	struct list_head *pos;
	struct ServerList *server;

	list_for_each(pos, g_pNetContext->pServerList)
	{
		server = list_entry(pos, struct ServerList, list);
		net_connect_wrapper(g_pNetContext->pConnector, server->szAddress, server->nPort);
	}

	return 0;
}

int32_t net_start_server()
{
	return 0;
}

int32_t net_loop()
{
	struct list_head *pos, *backup;
	struct PacketList *packet;

	net_start_server();

	net_connect_server();

	while(1)
	{
		list_for_each_safe(pos, backup, g_pNetContext->pSendList)
		{
			packet = list_entry(pos, struct PacketList, list);
			net_write(g_pNetContext->pNetHandler, packet->nSessionID, packet->pPacketData, packet->nPacketSize);
			
			list_del(pos);
			free(packet->pPacketData);
			free(packet);
		}

		net_run_wrapper(g_pNetContext->pNetHandler);
	}

	return 0;
}

void net_uninit()
{
	if(g_pNetContext == NULL)
	{
		return;
	}

	destory_connector_wrapper(g_pNetContext->pConnector);
	destory_acceptor_wrapper(g_pNetContext->pAcceptor);
	uninit_context_wrapper(g_pNetContext->pNetHandler);
	unregist_interface(g_pNetContext->pNetFuncEntry);

	free(g_pNetContext);
	g_pNetContext = NULL;
}
