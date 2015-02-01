/*
 * net_interface.h
 *
 *  Created on: 2014年12月31日
 *      Author: jimm
 */

#include "net_func_wrapper.h"
#include "net_interface.h"

int32_t main(int argc, char** argv)
{
	struct CAcceptor *pAcceptor = NULL;
	struct CConnector *pConnector = NULL;
	struct CNetHandler *pNetHandler = NULL;
	struct NetFuncEntry *pNetFuncEntry = NULL;

	//注册回调接口
	pNetFuncEntry = regist_interface(func_net_parser, func_net_accepted, func_net_connected,
						func_net_connect_timeout, func_net_read, func_net_writen, func_net_closed,
						func_net_error);

	g_pFuncEntry = pNetFuncEntry;

	//初始化网络环境
	pNetHandler = init_context_wrapper();

	//创建接收器
	pAcceptor = create_acceptor_wrapper(pNetHandler);
	net_bind_wrapper(pAcceptor, "127.0.0.1", 10000);
	//net_bind_wrapper(pAcceptor, "127.0.0.1", 10001);

	//创建连接器
	pConnector = create_connector_wrapper(pNetHandler);
	net_connect_wrapper(pConnector, "42.62.31.11", 8001);

	while(1)
	{
		net_run_wrapper(pNetHandler);
	}

	destory_acceptor_wrapper(pAcceptor);
	destory_connector_wrapper(pConnector);

	return 0;
}
