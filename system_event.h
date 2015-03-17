/*
 * code_extern.h
 *
 *  Created on: 2015��02��03��
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
//���ӽ����¼�
#define SYSEVT_ACCEPTED				SYSEVT_BASE + 1
//���ӽ����ɹ�
#define SYSEVT_CONNECTED			SYSEVT_BASE + 2
//���ӳ�ʱ
#define SYSEVT_CONNECTTIMEOUT		SYSEVT_BASE + 3
//���ӹر�
#define SYSEVT_CLOSED				SYSEVT_BASE + 4
//���Ӵ���
#define SYSEVT_ERROR				SYSEVT_BASE + 5

#define NETEVT_BASE					60100
//ping��
#define NETEVT_PING					NETEVT_BASE + 1
//pong��
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
