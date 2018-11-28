#include "Packet.h"
#include "PacketQueue.h"

SERVER_USING

CPacket::CPacket()
{
	m_tReadPacket.pPacket = this;
	m_tWritePacket.pPacket = this;

	m_iRemainlength = 0;
	m_iCurrentPacketNumber = 0;
	m_iLastReadPacketNumber = 0;

}

CPacket::~CPacket()
{
	cout << m_tSocketInfo.hSocket << " Client DisConnect" << endl;
}

char * CPacket::GetWriteBuffer()
{
	memset(m_tWritePacket.cPacket, 0, sizeof(m_tWritePacket.cPacket));
	return m_tWritePacket.cPacket;
}

bool CPacket::Read()
{
	CGSocket::Read();
	return true;
}

bool CPacket::Read_Ex(int _iDataLength)
{
	GET_SINGLE(CPacketQueue)->AddData(m_tReadBuffer.cBuffer, _iDataLength);
	GET_SINGLE(CPacketQueue)->GetData();

	return true;
}

bool CPacket::Write(char * _pBuffer, unsigned int _iSize)
{
	CGSocket::Write(_pBuffer, _iSize);
	return true;
}

bool CPacket::Write(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize)
{
	memset(&m_tWritePacket, 0, sizeof(PACKET));

	m_tWritePacket.iSize = _iSize + sizeof(m_tWritePacket.iHeader) + sizeof(m_tWritePacket.iSize);
	m_tWritePacket.iHeader = _iHeader;
	memcpy(m_tWritePacket.cPacket, _pBuffer, _iSize);

	Write((char*)&m_tWritePacket, _iSize + 8);

	return true;
}
