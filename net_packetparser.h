/*
 * net_packetparser.h
 *
 *  Created on: 2014Äê12ÔÂ24ÈÕ
 *      Author: jimm
 */

class CNetPacketParser : public IPacketParser
{
public:

	virtual ~CNetPacketParser(){};

	virtual int32_t Init();

	virtual int32_t Uninit();

	virtual int32_t GetSize();

	virtual int32_t Parser(const uint8_t arrBuf[], const uint32_t nBufSize, uint8_t arrPacket[], int32_t &nPacketSize);

};

