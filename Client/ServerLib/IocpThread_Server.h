#pragma once
#include "IocpThread.h"

SERVER_BEGIN

class CIocpThread_Server :
	public CIocpThread
{
public:
	virtual void run();


public:
	CIocpThread_Server();
	virtual ~CIocpThread_Server();
};

SERVER_END