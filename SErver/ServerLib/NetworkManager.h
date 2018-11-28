#pragma once

#include "ServerEngine.h"

SERVER_BEGIN

class CIocpThread;

class CNetworkManager
{
private:
	list<class CPacket*> m_SocketList;
	unordered_map<string, class CThread*> m_mapThread;
	vector<CIocpThread*> m_vecIocpThread;
	CPacket* m_pListen;
	CPacket* m_1Player;
	CPacket* m_2Player;
	HANDLE m_hComPort;

public:
	void SendMessageAll(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize);
	void SendPacket(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize, bool _is1P);

public:
	CPacket* GetListen() const { return m_pListen; }
	HANDLE GetComPort() const { return m_hComPort; }

public:
	bool init(short _sPort);

	CThread* FindThread(const string& _strKey);
	class CPacket* CreateClient(const SOCKETINFO& tSockInfo);
	bool CloseClient(class CPacket* _pPacket);

	void SetPlayer(CPacket* _Player);
public:
	CIocpThread* CreateIocpThread();

public:
	template <typename T>
	T* CreateThread(const string& _strKey)
	{
		T* pThread = (T*)FindThread(_strKey);

		if (pThread)
			return NULL;

		pThread = new T;

		pThread->Create();

		m_mapThread.insert(make_pair(_strKey, (CThread*)pThread));

		return pThread;
	}

	DECLARE_SINGLE(CNetworkManager)
};

SERVER_END