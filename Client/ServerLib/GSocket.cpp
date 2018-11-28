#include "GSocket.h"

SERVER_USING

CGSocket::CGSocket()
{
	memset(&m_tReadBuffer, 0, sizeof(m_tReadBuffer));
	memset(&m_tWriteBuffer, 0, sizeof(m_tWriteBuffer));

	m_tReadBuffer.tBuf.buf = m_tReadBuffer.cBuffer;
	m_tWriteBuffer.tBuf.buf = m_tWriteBuffer.cBuffer;

	m_tReadBuffer.iType = READ_COMPLETE;
	m_tWriteBuffer.iType = WRITE_COMPLETE;
}


CGSocket::~CGSocket()
{
	closesocket(m_tSocketInfo.hSocket);
}

bool CGSocket::CreateListen(short _sPort)
{
	m_tSocketInfo.hSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0,
		WSA_FLAG_OVERLAPPED);

	memset(&m_tSocketInfo.tAddr, 0, sizeof(m_tSocketInfo.tAddr));
	m_tSocketInfo.tAddr.sin_family = AF_INET;
	m_tSocketInfo.tAddr.sin_port = htons(_sPort);
	m_tSocketInfo.tAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(m_tSocketInfo.hSocket, (SOCKADDR*)&m_tSocketInfo.tAddr, sizeof(m_tSocketInfo.tAddr));

	listen(m_tSocketInfo.hSocket, SOMAXCONN);

	return true;
}

bool CGSocket::Connect(char * _pIP, short _sPort)
{
	m_tSocketInfo.hSocket = WSASocket(PF_INET, SOCK_STREAM, IPPROTO_TCP, 0, 0,
		WSA_FLAG_OVERLAPPED);

	memset(&m_tSocketInfo.tAddr, 0, sizeof(m_tSocketInfo.tAddr));
	m_tSocketInfo.tAddr.sin_family = AF_INET;
	m_tSocketInfo.tAddr.sin_port = htons(_sPort);
	m_tSocketInfo.tAddr.sin_addr.s_addr = inet_addr(_pIP);

	connect(m_tSocketInfo.hSocket, (SOCKADDR*)&m_tSocketInfo.tAddr, sizeof(m_tSocketInfo.tAddr));

	return true;
}

SOCKETINFO CGSocket::Accept()
{
	SOCKETINFO tSocketInfo;

	int iSize = sizeof(tSocketInfo.tAddr);
	tSocketInfo.hSocket = WSAAccept(m_tSocketInfo.hSocket, (SOCKADDR*)&tSocketInfo.tAddr, &iSize, NULL, NULL);

	cout << tSocketInfo.hSocket << " Client Connect" << endl;

	return tSocketInfo;
}

void CGSocket::SetSocketInfo(const SOCKETINFO & _tSocketInfo)
{
	m_tSocketInfo = _tSocketInfo;
}

void CGSocket::ConnectIocp(HANDLE _hComport)
{
	CreateIoCompletionPort((HANDLE)m_tSocketInfo.hSocket, _hComport,
		(ULONG_PTR)this, 0);
}

bool CGSocket::Read()
{
	DWORD dwFlag = 0;
	m_tReadBuffer.tBuf.len = PACKET_SIZE + 8;
	WSARecv(m_tSocketInfo.hSocket, &m_tReadBuffer.tBuf, 1, &m_dwReadTransfer, &dwFlag, &m_tReadBuffer.tOverlapped, NULL);

	return true;
}

bool CGSocket::Write(char * _pBuffer, unsigned int _iSize)
{
	DWORD dwFlag = 0;
	m_tWriteBuffer.tBuf.len = _iSize;
	memcpy(m_tWriteBuffer.cBuffer, _pBuffer, _iSize);
	WSASend(m_tSocketInfo.hSocket, &m_tWriteBuffer.tBuf, 1, &m_dwWriteTransfer,
		dwFlag, &m_tWriteBuffer.tOverlapped, NULL);

	return true;
}
