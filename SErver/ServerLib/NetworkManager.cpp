#include "NetworkManager.h"
#include "PacketSync.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "IocpThread.h"
#include "IocpThread_Server.h"
#include "AcceptThread.h"

SERVER_USING

DEFINITION_SINGLE(CNetworkManager);

CNetworkManager::CNetworkManager()
	: m_pListen(NULL)
	, m_1Player(NULL)
	, m_2Player(NULL)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	CPacketSync::init();
}


CNetworkManager::~CNetworkManager()
{
	CPacketSync::Cleanup();

	if (m_pListen != NULL)
		delete m_pListen;

	for (int i = 0; i < m_vecIocpThread.size(); ++i)
	{
		PostQueuedCompletionStatus(m_hComPort, 0, 0, NULL);
	}

	for (int i = 0; i < m_vecIocpThread.size(); ++i)
	{
		SAFE_DELETE(m_vecIocpThread[i]);
	}

	unordered_map<string, class CThread*>::iterator iter = m_mapThread.begin();
	for (; iter != m_mapThread.end(); ++iter)
	{
		if (iter->second != NULL)
		{
			delete iter->second;
			iter->second = NULL;
		}
	}

	DESTROY_SINGLE(CPacketQueue);

	WSACleanup();
}

bool CNetworkManager::init(short _sPort)
{
	GET_SINGLE(CPacketQueue)->init();

	m_hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	SYSTEM_INFO tSysInfo = {};
	GetSystemInfo(&tSysInfo);

	for (size_t i = 0; i < tSysInfo.dwNumberOfProcessors * 2; ++i)
	{
		char cText[64] = {};
		sprintf_s(cText, "Iocp%d", i + 1);
		CIocpThread* pIocp = CreateIocpThread();

		pIocp->SetCompletionPort(m_hComPort);
		pIocp->Start();
	}

	m_pListen = new CPacket;
	m_pListen->CreateListen(_sPort);

	CAcceptThread* pAccThread = CreateThread<CAcceptThread>("AcceptThread");
	pAccThread->SetListen(m_pListen);
	pAccThread->Start();

	return true;
}


CThread * CNetworkManager::FindThread(const string & _strKey)
{
	unordered_map<string, class CThread*>::iterator iter = m_mapThread.find(_strKey);

	if (iter == m_mapThread.end())
		return NULL;

	return iter->second;
}

CPacket * CNetworkManager::CreateClient(const SOCKETINFO & tSockInfo)
{
	CPacket* pClient = new CPacket;

	pClient->SetSocketInfo(tSockInfo);

	m_SocketList.push_back(pClient);

	return (CPacket*)pClient;
}

bool CNetworkManager::CloseClient(CPacket * _pPacket)
{
	list<CPacket*> ::iterator iter = m_SocketList.begin();

	for (; iter != m_SocketList.end(); ++iter)
	{
		if ((*iter) == _pPacket)
		{
			SAFE_DELETE((*iter));
			m_SocketList.erase(iter);
			return true;
		}
	}
	return false;
}

void CNetworkManager::SetPlayer(CPacket * _Player)
{
	if (m_1Player == NULL)
		m_1Player = _Player;

	else if (m_2Player == NULL)
		m_2Player = _Player;
}

CIocpThread * CNetworkManager::CreateIocpThread()
{
	CIocpThread* pThread = new CIocpThread_Server;

	pThread->Create();

	m_vecIocpThread.push_back(pThread);

	return pThread;
}

void CNetworkManager::SendMessageAll(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize)
{
	list<class CPacket*>::iterator iter = m_SocketList.begin();

	for (; iter != m_SocketList.end(); ++iter)
	{
		(*iter)->Write(_iHeader, _pBuffer, _iSize);
	}
}

void CNetworkManager::SendPacket(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize, bool _is1P)
{
	if (_is1P)
		m_1Player->Write(_iHeader, _pBuffer, _iSize);

	else
		m_2Player->Write(_iHeader, _pBuffer, _iSize);

}
