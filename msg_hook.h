/*
 * msg_hook.h
 *
 *  Created on: 2015Äê4ÔÂ8ÈÕ
 *      Author: jimm
 */

#ifndef MSG_HOOK_H_
#define MSG_HOOK_H_

#include "../netevent/net_typedef.h"
#include "net_ctype.h"

int32_t init_msg_hook();

void uninit_msg_hook();

int32_t msg_hook(int32_t msgid, SessionID nSessionID, uint8_t *buf, int32_t buf_size);

sync_msg_proc get_msg_hook(int32_t msgid);

void clear_msg_list();

int32_t handle_statussync_noti(uint32_t sessionid, uint8_t *buf, int32_t buf_size);

int32_t handle_msgpush_noti(uint32_t sessionid, uint8_t *buf, int32_t buf_size);

int32_t handle_msgfinack_resp(uint32_t sessionid, uint8_t *buf, int32_t buf_size);

#endif /* MSG_HOOK_H_ */
