#pragma once
#include "Thread.h"

SERVER_BEGIN

class CAcceptThread :
	public CThread
{
public:
	CAcceptThread();
	virtual ~CAcceptThread();

private:
	class CGSocket* m_pListen;

public:
	void SetListen(CGSocket* _pSocket);

public:
	virtual void run();
};

SERVER_END