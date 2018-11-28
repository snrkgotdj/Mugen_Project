#include "AcceptThread.h"
#include "GSocket.h"
#include "NetworkManager.h"
#include "Packet.h"

SERVER_USING

CAcceptThread::CAcceptThread()
{
}

CAcceptThread::~CAcceptThread()
{
}

void CAcceptThread::SetListen(CGSocket * _pSocket)
{
	m_pListen = _pSocket;
}

void CAcceptThread::run()
{
	while (true)
	{
		SOCKETINFO tInfo = m_pListen->Accept();
		if (tInfo.hSocket == SOCKET_ERROR)
		{
			break;
		}

		CPacket* pPacket = GET_SINGLE(CNetworkManager)->CreateClient(tInfo);
		pPacket->ConnectIocp(GET_SINGLE(CNetworkManager)->GetComPort());
		pPacket->Read();
		GET_SINGLE(CNetworkManager)->SetPlayer(pPacket);
	}
}
