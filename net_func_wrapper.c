/*
 * net_func_wrapper.c
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "net_func_wrapper.h"
#include "net_interface.h"

struct CNetHandler* init_context_wrapper()
{
	return init_context(net_parser_wrapper, net_accept_completed_wrapper, net_connect_completed_wrapper,
			net_connect_timeout_wrapper, net_read_completed_wrapper, net_write_completed_wrapper,
			net_close_completed_wrapper, net_error_wrapper);
}

void uninit_context_wrapper(struct CNetHandler *pNetHandler)
{
	uninit_context(pNetHandler);
}

int32_t net_run_wrapper(struct CNetHandler* pNetHandler)
{
	net_run(pNetHandler);
	return 0;
}

int32_t net_parser_wrapper(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t *pPacketSize)
{
	if(GetNetFuncEntry()->func_net_parser == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_parser(arrBuf, nBufSize, arrPacket, pPacketSize);
}

struct CAcceptor *create_acceptor_wrapper(struct CNetHandler* pNetHandler)
{
	return create_acceptor(pNetHandler);
}

void destory_acceptor_wrapper(struct CAcceptor *pAcceptor)
{
	destory_acceptor(pAcceptor);
}

struct CConnector *create_connector_wrapper(struct CNetHandler* pNetHandler)
{
	return create_connector(pNetHandler);
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
	if(GetNetFuncEntry()->func_net_accepted == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_accepted(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_connect_wrapper(struct CConnector *pConnector, char *pPeerAddress, uint16_t nPeerPort)
{
	return net_connect(pConnector, pPeerAddress, nPeerPort);
}

int32_t net_connect_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	if(GetNetFuncEntry()->func_net_connected == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_connected(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_connect_timeout_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	if(GetNetFuncEntry()->func_net_connect_timeout == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_connect_timeout(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_read_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	if(GetNetFuncEntry()->func_net_read == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_read(nSessionID, pData, nBytes);
}

int32_t net_write_wrapper(struct CNetHandler *pNetHandler, SessionID nSessionID, uint8_t arrBuf[], int32_t nBytes)
{
	return net_write(pNetHandler, nSessionID, arrBuf, nBytes);
}

int32_t net_write_completed_wrapper(SessionID nSessionID, uint8_t *pData, int32_t nBytes)
{
	if(GetNetFuncEntry()->func_net_writen == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_writen(nSessionID, pData, nBytes);
}

int32_t net_close_wrapper(struct CNetHandler *pNetHandler, SessionID nSessionID)
{
	return net_close(pNetHandler, nSessionID);
}

int32_t net_close_completed_wrapper(SessionID nSessionID, char *pPeerAddress, uint16_t nPeerPort)
{
	if(GetNetFuncEntry()->func_net_closed == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_closed(nSessionID, pPeerAddress, nPeerPort);
}

int32_t net_error_wrapper(SessionID nSessionID, int32_t nErrorID)
{
	if(GetNetFuncEntry()->func_net_error == NULL)
	{
		return 0;
	}
	return GetNetFuncEntry()->func_net_error(nSessionID, nErrorID);
}

