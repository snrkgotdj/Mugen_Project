#pragma once

#include "global.h"
#include "CCore.h"

#define MM_LOG 5000

class CDebugManager
{
public:
	void init();
	void Log(wchar_t* _pLog)
	{
		SendMessage(CCore::GetInst().GetMainWindowHwnd(), MM_LOG, (WPARAM)_pLog, 0);
	}


	static CDebugManager& Manager()
	{
		static CDebugManager inst;
		return inst;
	}
private:
	CDebugManager();
	~CDebugManager();
};

