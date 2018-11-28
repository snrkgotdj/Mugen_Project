#pragma once
#include "Thread.h"

SERVER_BEGIN

class CIocpThread :
	public CThread
{
public:
	CIocpThread();
	virtual ~CIocpThread();

protected:
	HANDLE m_hComPort;

public:
	void SetCompletionPort(HANDLE _hComPort);

public:
	virtual void run();
};

SERVER_END