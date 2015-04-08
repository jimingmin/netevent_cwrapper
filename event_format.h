/*
 * event_format.h
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

#ifndef EVENT_FORMAT_H_
#define EVENT_FORMAT_H_

#include "code_extern.h"
#include "system_event.h"
#include "msg_event.h"

int32_t encode_event_head(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_head *target);

int32_t decode_event_head(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_head *target);

int32_t encode_event_accepted(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_accepted *target);

int32_t encode_event_connected(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connected *target);

int32_t encode_event_connecttimeout(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connecttimeout *target);

int32_t encode_event_closed(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_closed *target);

int32_t encode_event_error(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_error *target);

int32_t decode_statussync_noti(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct statussync_noti *target);

int32_t encode_msgsync_req(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgsync_req *target);

int32_t decode_msgpush_noti(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgpush_noti *target);

int32_t encode_msgfinack_req(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgfinack_req *target);

int32_t decode_msgfinack_resp(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct msgfinack_resp *target);

#endif /* EVENT_FORMAT_H_ */
