#include "ClientManager.h"
#include "PacketSync.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "IocpThread.h"


SERVER_USING

CClientManager::CClientManager()
	: m_pClient(nullptr)
	, m_pIocpThread(nullptr)
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	CPacketSync::init();
}


CClientManager::~CClientManager()
{
	if (m_pClient != NULL)
		delete m_pClient;

	PostQueuedCompletionStatus(m_hComport, 0, 0, NULL);
	
	SAFE_DELETE(m_pIocpThread);

	WSACleanup();

	CPacketSync::Cleanup();

	CloseHandle(m_hComport);

	DESTROY_SINGLE(CPacketQueue);
}


bool CClientManager::init(char* _IP, short _port)
{
	GET_SINGLE(CPacketQueue)->init();

	m_hComport = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
	
	m_pIocpThread = new CIocpThread;
	m_pIocpThread->Create();
	m_pIocpThread->SetCompletionPort(m_hComport);
	m_pIocpThread->Start();

	m_pClient = new CPacket;
	m_pClient->Connect(_IP, _port);
	m_pClient->ConnectIocp(m_hComport);

	m_pClient->Read();
	return false;
}

bool CClientManager::Write(unsigned int _iHeader, char * _pBuffer, unsigned int _iSize)
{
	m_pClient->Write(_iHeader, _pBuffer, _iSize);
	return true;
}
