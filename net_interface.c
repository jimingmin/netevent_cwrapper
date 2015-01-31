/*
 * net_interface.cpp
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "net_interface.h"

void init_context_wrapper()
{
	init_context(net_accept_completed_wrapper, net_connect_completed_wrapper, 
		net_read_completed_wrapper, net_write_completed_wrapper, net_connect_completed_wrapper);
}

int32_t net_run_wrapper()
{
	net_run();
	return 0;
}

struct CAcceptor *create_acceptor_wrapper()
{
	return create_acceptor();
}

void destory_acceptor_wrapper(struct CAcceptor *pAcceptor)
{
	destory_acceptor(pAcceptor);
}

struct CConnector *create_connector_wrapper()
{
	return create_connector();
}

void destory_connector_wrapper(struct CConnector *pConnector)
{
	destory_connector(pConnector);
}

int32_t net_bind_wrapper(struct CAcceptor *pAcceptor, char *pLocalAddress, uint16_t nLocalPort)
{
	return net_bind(pAcceptor, pLocalAddress, nLocalPort);
}

int32_t net_accept_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t net_connect_wrapper(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort)
{
	return net_connect(pConnector, pPeerAddress, nPeerPort);
}

int32_t net_connect_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t net_connect_timeout_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t net_read_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return 0;
}

int32_t net_write_wrapper(SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes)
{
	return net_write(nSessionID, arrBuf, nBytes);
}

int32_t net_write_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	return 0;
}

int32_t net_close_wrapper(SessionID nSessionID)
{
	return net_close(nSessionID);
}

int32_t net_close_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	return 0;
}

int32_t net_error_wrapper(SessionID nSessionID, int32_t nErrorID)
{
	return 0;
}

int32_t net_event_loop()
{
	return 0;
}
