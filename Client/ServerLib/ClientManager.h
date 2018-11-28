#pragma once

#include "ServerEngine.h"

SERVER_BEGIN

class CPacket;
class CIocpThread;

class CClientManager
{
private:
	HANDLE m_hComport;
	CPacket* m_pClient;
	CIocpThread* m_pIocpThread;

public:
	bool init(char* _IP, short _port);
	bool Write(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize);
	CPacket* GetClient() { return m_pClient; }


public:
	static CClientManager& Manager()
	{
		static CClientManager inst;
		return inst;
	}

private:
	CClientManager();
	~CClientManager();
};

SERVER_END