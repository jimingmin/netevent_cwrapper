/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

#include "../net_impl.h"

class CNetPacketParserFactory : public IPacketParserFactory
{
public:
	CNetPacketParserFactory();

	virtual IPacketParser *Create();

	virtual void Destory(IPacketParser *pParser);
};
