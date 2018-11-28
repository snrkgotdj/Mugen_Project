#include "PacketSync.h"

SERVER_USING

CRITICAL_SECTION CPacketSync::m_tCrt;

CPacketSync::CPacketSync()
{
}


CPacketSync::~CPacketSync()
{
}

void CPacketSync::init()
{
	InitializeCriticalSection(&m_tCrt);
}

void CPacketSync::Cleanup()
{
	DeleteCriticalSection(&m_tCrt);
}

void CPacketSync::Lock()
{
	EnterCriticalSection(&m_tCrt);
}

void CPacketSync::Unlock()
{
	LeaveCriticalSection(&m_tCrt);
}
