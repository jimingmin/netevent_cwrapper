/*
 * net_messagehandler.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "../netevent/net_impl.h"
#include "../netevent/net_ioadapter.h"
#include "../netevent/net_connector.h"

#include "net_namespace.h"

class CNetMessageHandler : public CIOHandlerAdapter
{
public:
	virtual int32_t OnOpened(IIOSession *pIoSession);

	virtual int32_t OnRecved(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnSent(IIOSession *pIoSession, uint8_t *pBuf, uint32_t nBufSize);

	virtual int32_t OnClosed(IIOSession *pIoSession);

	virtual int32_t OnError(IIOSession *pIoSession);

	virtual int32_t OnTimeout(IIOSession *pIoSession);

};

