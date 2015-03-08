/*
 * net_func_wrapper.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "net_extern.h"

struct CNetHandler* init_context_wrapper();

void uninit_context_wrapper(struct CNetHandler *pNetHandler);

int32_t net_run_wrapper(struct CNetHandler* pNetHandler);

int32_t net_parser_wrapper(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrOutputBuf[], int32_t nOutputBufSize);

struct CAcceptor *create_acceptor_wrapper(struct CNetHandler* pNetHandler);

void destory_acceptor_wrapper(struct CAcceptor *pAcceptor);

struct CConnector *create_connector_wrapper(struct CNetHandler* pNetHandler);

void destory_connector_wrapper(struct CConnector *pConnector);

int32_t net_bind_wrapper(struct CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort);

int32_t net_accept_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_wrapper(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_connect_timeout_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_read_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t net_write_wrapper(struct CNetHandler *pNetHandler, SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes);

int32_t net_write_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes);

int32_t net_close_wrapper(struct CNetHandler *pNetHandler, SessionID nSessionID);

int32_t net_close_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort);

int32_t net_error_wrapper(SessionID nSessionID, int32_t nErrorID);


