/*
 * net_interface.h
 *
 *  Created on: 2014Äê12ÔÂ31ÈÕ
 *      Author: jimm
 */

#include "../netevent/net_typedef.h"

extern void init_context(callback_net_accepted func_net_accepted, callback_net_connected func_net_connected,
						  callback_net_read func_net_read, callback_net_writen func_net_writen,
						  callback_net_closed func_net_closed);

extern struct CAcceptor *create_acceptor();

extern void destory_acceptor(struct CAcceptor *pAcceptor);

extern struct CConnector *create_connector();

extern void destory_connector(struct CConnector *pConnector);

extern int32_t net_bind(struct CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

extern int32_t net_accepted(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

extern int32_t net_connect(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

extern int32_t net_connect_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

extern int32_t net_read_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

extern int32_t net_write(SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

extern int32_t net_write_completed(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

extern int32_t net_close(SessionID nSessionID);

extern int32_t net_close_completed(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);
