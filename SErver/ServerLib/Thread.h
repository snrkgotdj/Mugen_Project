#pragma once
#include "ServerEngine.h"

SERVER_BEGIN

class CThread
{
public:
	CThread();
	virtual ~CThread();

private:
	HANDLE m_hThread;
	HANDLE m_hStart;

public:
	bool Create();
	void WaitStart();
	void Start();

public:
	virtual void run() = 0;

public:
	static UINT __stdcall ThreadFun(void* pArg);
};

SERVER_END