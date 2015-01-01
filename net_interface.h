/*
 * net_interface.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "net_extern.h"

void init_context_wrapper();

struct CAcceptor *create_acceptor_wrapper();

void destory_acceptor_wrapper(struct CAcceptor *pAcceptor);

struct CConnector *create_connector_wrapper();

void destory_connector_wrapper(struct CConnector *pConnector);

int32_t net_bind_wrapper(struct CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

int32_t net_accept_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_wrapper(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_timeout_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_read_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t net_write_wrapper(SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

int32_t net_write_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t net_close_wrapper(SessionID nSessionID);

int32_t net_close_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_error_wrapper(SessionID nSessionID, int32_t nErrorID);

int32_t net_event_loop();

