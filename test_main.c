/*
 * test_main.c
 *
 *  Created on: 2014年12月31日
 *      Author: jimm
 */

#include "net_func_wrapper.h"
#include "net_interface.h"
#include "net_loop.h"
#include "logger_extern.h"
#include "net_timer.h"
#include "system_event.h"
#include "event_format.h"
#include <stdio.h>

struct LoginReq
{
	uint8_t		phone_type;
	char		phone_num[32];
	char		password[64];
	char		os_ver[32];
	char		phone_style[32];
};

void ThreadProc(void *pParam)
{
	uint32_t nSessionID = 0;
	uint8_t arrPacket[4096];
	int32_t nPacketSize = 0;
	struct event_head head;
	struct event_head send_head;
	struct LoginReq login_req;
	uint32_t offset = 0;

	while(1)
	{
		func_net_read(&nSessionID, arrPacket, &nPacketSize);
		if(nPacketSize > 0)
		{
			offset = 0;
			decode_event_head(arrPacket, sizeof(arrPacket), &offset, &head);

			printf("recv msg id %d\n", head.event_id);

			if(head.event_id == SYSEVT_CONNECTED)
			{
				send_head.event_id = 10;
				strcpy(login_req.os_ver, "1");
				strcpy(login_req.phone_num, "18616274138");
				strcpy(login_req.password, "0b4e7a0e5fe84ad35fb5f95b9ceeac79");
				strcpy(login_req.phone_style, "test");
				login_req.phone_type = 1;
				offset = 0;
				encode_event_head(arrPacket, sizeof(arrPacket), &offset, &send_head);
				encode_uint8_t(arrPacket, sizeof(arrPacket), &offset, login_req.phone_type);
				encode_char_mem(arrPacket, sizeof(arrPacket), &offset, login_req.phone_num, 32);
				encode_char_mem(arrPacket, sizeof(arrPacket), &offset, login_req.password, 64);
				encode_char_mem(arrPacket, sizeof(arrPacket), &offset, login_req.os_ver, 32);
				encode_char_mem(arrPacket, sizeof(arrPacket), &offset, login_req.phone_style, 32);

				func_net_write(nSessionID, arrPacket, offset);
			}
			else if(head.event_id == SYSEVT_CLOSED)
			{
				func_net_connect("182.92.240.240", 8000);
			}
		}

		Sleep(100);
		nPacketSize = 0;
	}
}

int32_t main(int argc, char** argv)
{
	NetTimer *timer1;
	NetTimer *timer2;
	//set_log_dir("D:/");
	//start_log_thread();
	//Sleep(1000);
	//write_debug_log("test", "%s%d\n", "haha", 1);
	net_init("D:/", "test");

	_beginthread(ThreadProc, 0, NULL);
	//func_net_connect("42.62.31.11", 8001);
	func_net_connect("182.92.240.240", 8000);

	net_loop();
	//struct CAcceptor *pAcceptor = NULL;
	//struct CConnector *pConnector = NULL;
	//struct CNetHandler *pNetHandler = NULL;
	//struct NetFuncEntry *pNetFuncEntry = NULL;

	////注册回调接口
	//pNetFuncEntry = regist_interface(func_net_parser, func_net_accepted, func_net_connected,
	//					func_net_connect_timeout, func_net_read, func_net_writen, func_net_closed,
	//					func_net_error);

	//g_pFuncEntry = pNetFuncEntry;

	////初始化网络环境
	//pNetHandler = init_context_wrapper();

	////创建接收器
	//pAcceptor = create_acceptor_wrapper(pNetHandler);
	//net_bind_wrapper(pAcceptor, "127.0.0.1", 10000);
	////net_bind_wrapper(pAcceptor, "127.0.0.1", 10001);

	////创建连接器
	//pConnector = create_connector_wrapper(pNetHandler);
	//net_connect_wrapper(pConnector, "42.62.31.11", 8001);

	//while(1)
	//{
	//	net_run_wrapper(pNetHandler);
	//}

	//destory_acceptor_wrapper(pAcceptor);
	//destory_connector_wrapper(pConnector);

	Sleep(10000000);
	return 0;
}
