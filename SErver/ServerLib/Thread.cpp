#include "Thread.h"

SERVER_USING

CThread::CThread() :
	m_hThread(0),
	m_hStart(0)
{
}

CThread::~CThread()
{
	if (m_hStart != NULL)
	{
		CloseHandle(m_hStart);
	}

	if (m_hThread)
	{
		WaitForSingleObject(m_hThread, INFINITE);
		CloseHandle(m_hThread);
	}
}

bool CThread::Create()
{
	m_hStart = CreateEvent(NULL, FALSE, FALSE, NULL);
	m_hThread = (HANDLE)_beginthreadex(NULL, 0, CThread::ThreadFun, this, 0, 0);

	if (m_hThread == NULL)
		return false;

	//CloseHandle(m_hThread);
	return true;
}

void CThread::WaitStart()
{
	WaitForSingleObject(m_hStart, INFINITE);
}

void CThread::Start()
{
	SetEvent(m_hStart);
}

UINT CThread::ThreadFun(void * pArg)
{
	CThread* pThread = (CThread*)pArg;

	pThread->WaitStart();
	pThread->run();

	return 0;
}
