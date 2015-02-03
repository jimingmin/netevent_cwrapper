/*
 * net_loop.c
 *
 *  Created on: 2015��02��02��
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

	//ע��ص��ӿ�
	g_pNetContext->pNetFuncEntry = regist_interface(func_net_parser, func_net_accepted, func_net_connected,
		func_net_connect_timeout, func_net_recved, func_net_writen, func_net_closed,
		func_net_error);

	//��ʼ�����绷��
	g_pNetContext->pNetHandler = init_context_wrapper();
	//��ʼ��������
	g_pNetContext->pAcceptor = create_acceptor_wrapper(g_pNetContext->pNetHandler);
	//��ʼ��������
	g_pNetContext->pConnector = create_connector_wrapper(g_pNetContext->pNetHandler);

	g_pNetContext->stSendLock = create_lock();
	//��ʼ����������
	g_pNetContext->pSendList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pSendList);

	g_pNetContext->stRecvLock = create_lock();
	//��ʼ����������
	g_pNetContext->pRecvList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pRecvList);

	g_pNetContext->stServerLock = create_lock();
	//��ʼ���������б�
	g_pNetContext->pServerList = (struct list_head *)malloc(sizeof(struct list_head));
	INIT_LIST_HEAD(g_pNetContext->pServerList);

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
	return 0;
}

int32_t net_send_data()
{
	struct list_head *pos, *backup;
	struct PacketList *packet;

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

	while(1)
	{
		net_connect_server();

		net_send_data();

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

	destory_lock(g_pNetContext->stServerLock);
	destory_lock(g_pNetContext->stSendLock);

	free(g_pNetContext);
	g_pNetContext = NULL;
}