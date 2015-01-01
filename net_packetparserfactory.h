/*
 * net_packetparserfactory.h
 *
 *  Created on: 2014��12��24��
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
