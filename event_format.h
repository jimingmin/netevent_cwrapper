/*
 * event_format.h
 *
 *  Created on: 2015Äê02ÔÂ03ÈÕ
 *      Author: jimm
 */

#include "code_extern.h"
#include "system_event.h"

int32_t encode_event_head(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_head *target);

int32_t encode_event_connected(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connected *target);

int32_t encode_event_connecttimeout(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_connecttimeout *target);

int32_t encode_event_closed(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_closed *target);

int32_t encode_event_error(uint8_t *buf, int32_t buf_size, uint32_t *offset, struct event_error *target);
