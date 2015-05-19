/*
 * msg_event.h
 *
 *  Created on: 2015��4��8��
 *      Author: jimm
 */

#ifndef MSG_EVENT_H_
#define MSG_EVENT_H_

#include "../common/common_typedef.h"
#include "../netevent/net_typedef.h"
#include "net_ctype.h"

//״̬ͬ��֪ͨ
#define MSGID_STATUSSYNC_NOTI		60200
struct statussync_noti
{
	uint8_t				placeholder[0];
};

//��Ϣͬ������
#define MSGID_MSGSYNC_REQ			60201
struct msgsync_req
{
	uint32_t				sync_seq;
};

//��Ϣ����֪ͨ
#define MSGID_MSGPUSH_NOTI			60202
struct msgpush_noti
{
	uint8_t				m_nSyncFlag;
	uint32_t				m_nSyncSeq;
	uint16_t				m_nMsgSize;
	uint8_t				m_arrMsg[MAX_PACKET_SIZE];
};

//��Ϣ��������
#define MSGID_MSGFINACK_REQ		60203
struct msgfinack_req
{
	int32_t				sync_count;
	uint32_t				sync_seq;
};

//��Ϣ������Ӧ
#define MSGID_MSGFINACK_RESP		60204
struct msgfinack_resp
{
	uint32_t				sync_seq;
};


#endif /* MSG_EVENT_H_ */
