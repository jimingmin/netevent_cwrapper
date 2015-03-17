/*
 * code_extern.h
 *
 *  Created on: 2015年02月03日
 *      Author: jimm
 */

#ifndef SYSTEM_EVENT_H_
#define SYSTEM_EVENT_H_

#include "../common/common_typedef.h"
#include "../netevent/net_typedef.h"

#ifdef WIN32
#define INLINE __inline
#else
#define INLINE inline
#endif

#define SYSEVT_BASE					60000
//连接接收事件
#define SYSEVT_ACCEPTED				SYSEVT_BASE + 1
//连接建立成功
#define SYSEVT_CONNECTED			SYSEVT_BASE + 2
//连接超时
#define SYSEVT_CONNECTTIMEOUT		SYSEVT_BASE + 3
//连接关闭
#define SYSEVT_CLOSED				SYSEVT_BASE + 4
//连接错误
#define SYSEVT_ERROR				SYSEVT_BASE + 5

#define NETEVT_BASE					60100
//ping包
#define NETEVT_PING					NETEVT_BASE + 1
//pong包
#define NETEVT_PONG					NETEVT_BASE + 2

struct event_head
{
	uint16_t		total_size;
	uint16_t		event_id;
	uint32_t		seq;
	uint32_t		src_uin;
	uint32_t		dst_uin;
};

#define event_head_size()	\
	(sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t) + sizeof(uint32_t))


struct event_accepted
{
	SessionID		sessionid;
	char			address[enmMaxIPAddressLength];
	uint16_t		port;
};

struct event_connected
{
	SessionID		sessionid;
	char			address[enmMaxIPAddressLength];
	uint16_t		port;
};

struct event_connecttimeout
{
	char			address[enmMaxIPAddressLength];
	uint16_t		port;
};

struct event_closed
{
	SessionID		sessionid;
	char			address[enmMaxIPAddressLength];
	uint16_t		port;
};

struct event_error
{
	SessionID		sessionid;
	uint8_t			error_code;
};

#endif
