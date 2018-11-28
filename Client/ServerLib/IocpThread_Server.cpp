#include "IocpThread_Server.h"
#include "Packet.h"
#include "NetworkManager.h"
#include "PacketQueue.h"


SERVER_USING

CIocpThread_Server::CIocpThread_Server()
{
}

CIocpThread_Server::~CIocpThread_Server()
{
}

void CIocpThread_Server::run()
{
	CPacket* pPacket = NULL;
	BUFFER* pBuffer = NULL;
	int retval;

	while (true)
	{
		DWORD dwTransfer = 0;
		retval = GetQueuedCompletionStatus(m_hComPort, &dwTransfer, (PULONG_PTR)&pPacket, (LPOVERLAPPED*)&pBuffer, INFINITE);

		if (pPacket == NULL)
		{
			break;
		}

		else if (retval == 0 || dwTransfer == 0)
		{
			CNetworkManager::GetInst()->CloseClient(pPacket);
			continue;
		}

		switch (pBuffer->iType)
		{
		case READ_COMPLETE:
			pPacket->Read_Ex(dwTransfer);
			pPacket->Read();
			break;

		case WRITE_COMPLETE:
		{
			int a = 0;
		}
		break;
		}
	}
}
