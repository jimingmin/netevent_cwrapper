#ifndef EVENT_DUMP_H_
#define EVENT_DUMP_H_

#include "../common/common_typedef.h"
#include "../netevent/net_typedef.h"
#include "net_ctype.h"
#include "system_event.h"

void event_dump_head(const char *log_name, const char *action, struct event_head *head);

void event_dump_connecting(const char *log_name, char *pPeerAddress, uint16_t nPeerPort);

void event_dump_connect_timeout(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort);

void event_dump_connected(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort);

void event_dump_closing(const char *log_name, SessionID sesson_id);

void event_dump_closed(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort);

#endif /* EVENT_DUMP_H_ */
