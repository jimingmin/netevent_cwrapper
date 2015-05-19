#include "event_dump.h"
#include "logger_extern.h"
#include "system_event.h"

void event_dump_head(const char *log_name, const char *action, struct event_head *head)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "%s event_head={totalsize=%d, msgid=%d, seq=%u, src_uin=%u, dst_uin=%u}\n",
		action, head->total_size, head->event_id, head->seq, head->src_uin, head->dst_uin);
}

void event_dump_connecting(const char *log_name, char *pPeerAddress, uint16_t nPeerPort)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "connecting %s : %d\n", pPeerAddress, nPeerPort);
}

void event_dump_connect_timeout(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "connecting %s : %d timeout, session_id=%u\n", pPeerAddress, nPeerPort, sesson_id);
}

void event_dump_connected(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "connected %s : %d, session_id=%u\n", pPeerAddress, nPeerPort, sesson_id);
}

void event_dump_closing(const char *log_name, SessionID sesson_id)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "closing session_id %u\n", sesson_id);
}

void event_dump_closed(const char *log_name, SessionID sesson_id, char *pPeerAddress, uint16_t nPeerPort)
{
	if(log_name == NULL)
	{
		return;
	}

	write_debug_log(log_name, "closed %s : %d, session_id=%u\n", pPeerAddress, nPeerPort, sesson_id);
}
