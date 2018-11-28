#pragma once
#include "Ref.h"

SERVER_BEGIN

class CGSocket :
	public CRef
{
public:
	CGSocket();
	virtual ~CGSocket();

protected:
	SOCKETINFO m_tSocketInfo;
	BUFFER m_tReadBuffer;
	BUFFER m_tWriteBuffer;
	DWORD m_dwReadTransfer;
	DWORD m_dwWriteTransfer;

public:
	bool CreateListen(short _sPort);
	bool Connect(char* _pIP, short _sPort);
	SOCKETINFO Accept();
	void SetSocketInfo(const SOCKETINFO& _tSocketInfo);
	void ConnectIocp(HANDLE _hComport);

public:
	const SOCKETINFO& GetSocketInfo() { return m_tSocketInfo; }

public:
	virtual bool Read();
	virtual bool Write(char* _pBuffer, unsigned int _iSize);
};

SERVER_END