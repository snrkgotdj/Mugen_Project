#pragma once
#include "GSocket.h"
#include "Stream.h"

SERVER_BEGIN

class CPacket :
	public CGSocket, public CStream
{
public:
	CPacket();
	virtual ~CPacket();

private:
	int m_iRemainlength;
	int m_iCurrentPacketNumber;
	int m_iLastReadPacketNumber;
	
protected:
	PACKET m_tReadPacket;
	PACKET m_tWritePacket;

public:
	PACKET& GetReadPacket() { return m_tReadPacket; }
	PACKET& GetWritePacket() { return m_tWritePacket; }

	char* GetWriteBuffer();
public:
	virtual bool Read();
	bool Read_Ex(int _iDataLength);
	virtual bool Write(char* _pBuffer, unsigned int _iSize);

public:
	bool Write(unsigned int _iHeader, char* _pBuffer, unsigned int _iSize);
};

SERVER_END