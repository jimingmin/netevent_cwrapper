/*
 * event_format.c
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

#include "event_format.h"
#include "code_extern.h"

int32_t encode_event_head(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_head *target)
{
	encode_uint16_t(buf, buf_size, offset, target->total_size);
	encode_uint16_t(buf, buf_size, offset, target->event_id);
	encode_uint32_t(buf, buf_size, offset, target->seq);
	encode_uint32_t(buf, buf_size, offset, target->src_uin);
	encode_uint32_t(buf, buf_size, offset, target->dst_uin);

	return 0;
}

int32_t decode_event_head(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_head *target)
{
	decode_uint16_t(buf, buf_size, offset, &(target->total_size));
	decode_uint16_t(buf, buf_size, offset, &(target->event_id));
	decode_uint32_t(buf, buf_size, offset, &(target->seq));
	decode_uint32_t(buf, buf_size, offset, &(target->src_uin));
	decode_uint32_t(buf, buf_size, offset, &(target->dst_uin));

	return 0;
}

int32_t encode_event_accepted(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_accepted *target)
{
	encode_uint32_t(buf, buf_size, offset, target->sessionid);
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16_t(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_connected(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connected *target)
{
	encode_uint32_t(buf, buf_size, offset, target->sessionid);
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16_t(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_connecttimeout(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connecttimeout *target)
{
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16_t(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_closed(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_closed *target)
{
	encode_uint32_t(buf, buf_size, offset, target->sessionid);
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16_t(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_error(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_error *target)
{
	encode_uint32_t(buf, buf_size, offset, target->sessionid);
	encode_uint8_t(buf, buf_size, offset, target->error_code);

	return 0;
}

int32_t decode_statussync_noti(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct statussync_noti *target)
{
	return 0;
}

int32_t encode_msgsync_req(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgsync_req *target)
{
	encode_uint32_t(buf, buf_size, offset, target->sync_seq);
	return 0;
}

int32_t decode_msgpush_noti(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgpush_noti *target)
{
	decode_uint8_t(buf, buf_size, offset, &(target->m_nSyncFlag));
	decode_uint32_t(buf, buf_size, offset, &(target->m_nSyncSeq));
	decode_uint16_t(buf, buf_size, offset, &(target->m_nMsgSize));
	decode_uint8_mem(buf, buf_size, offset, target->m_arrMsg, target->m_nMsgSize, MAX_PACKET_SIZE);

	return 0;
}

int32_t encode_msgfinack_req(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgfinack_req *target)
{
	encode_int32_t(buf, buf_size, offset, target->sync_count);
	encode_uint32_t(buf, buf_size, offset, target->sync_seq);
	return 0;
}

int32_t decode_msgfinack_resp(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgfinack_resp *target)
{
	decode_uint32_t(buf, buf_size, offset, &(target->sync_seq));
	return 0;
}

