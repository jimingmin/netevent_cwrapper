/*
 * net_interface.h
 *
 *  Created on: 2014Äê12ÔÂ31ÈÕ
 *      Author: jimm
 */

#include "../netevent/net_typedef.h"

extern struct CNetHandler* init_context(callback_net_parser func_net_parser, callback_net_accepted func_net_accepted,
							callback_net_connected func_net_connected, callback_net_connect_timeout func_net_connect_timeout,
							callback_net_read func_net_read, callback_net_writen func_net_writen,
							callback_net_closed func_net_closed, callback_net_error func_net_error);

extern void uninit_context(struct CNetHandler *pNetHandler);

extern void net_run(struct CNetHandler* pNetHandler);

extern struct CAcceptor *create_acceptor(struct CNetHandler* pNetHandler);

extern void destory_acceptor(struct CAcceptor *pAcceptor);

extern struct CConnector *create_connector(struct CNetHandler* pNetHandler);

extern void destory_connector(struct CConnector *pConnector);

extern int32_t net_bind(struct CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

extern int32_t net_accepted(struct IIOSession *pIoSession, callback_net_accepted func_net_accepted);

extern int32_t net_connect(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

extern int32_t net_connect_completed(struct IIOSession *pIoSession, callback_net_connected func_net_connected);

extern int32_t net_read_completed(struct IIOSession *pIoSession, callback_net_read func_net_read);

extern int32_t net_write(struct CNetHandler *pNetHandler, SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

extern int32_t net_write_completed(struct IIOSession *pIoSession, callback_net_writen func_net_writen, uint8_t *pData, int32_t nBytes);

extern int32_t net_close(struct CNetHandler *pNetHandler, SessionID nSessionID);

extern int32_t net_close_completed(struct IIOSession *pIoSession, callback_net_closed func_net_closed);
