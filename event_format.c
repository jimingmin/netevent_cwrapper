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
	encode_uint16(buf, buf_size, offset, target->total_size);
	encode_uint16(buf, buf_size, offset, target->event_id);
	encode_uint32(buf, buf_size, offset, target->seq);
	encode_uint32(buf, buf_size, offset, target->src_uin);
	encode_uint32(buf, buf_size, offset, target->dst_uin);

	return 0;
}


int32_t encode_event_accepted(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_accepted *target)
{
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_connected(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connected *target)
{
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_connecttimeout(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connecttimeout *target)
{
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_closed(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_closed *target)
{
	encode_char_mem(buf, buf_size, offset, target->address, enmMaxIPAddressLength);
	encode_uint16(buf, buf_size, offset, target->port);

	return 0;
}

int32_t encode_event_error(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_error *target)
{
	encode_uint8(buf, buf_size, offset, target->error_code);

	return 0;
}

