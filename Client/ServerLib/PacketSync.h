#pragma once
#include "ServerEngine.h"

SERVER_BEGIN

class CPacketSync
{
public:
	CPacketSync();
	virtual ~CPacketSync();

private:
	static CRITICAL_SECTION m_tCrt;

public:
	static void init();
	static void Cleanup();

protected:
	void Lock();
	void Unlock();
};

SERVER_END