#pragma once
#include "global.h"

class CCore
{
private:
	HWND m_hWnd;
	HDC m_hDC;

	float m_fAccTime;

public:
	int init(HWND _hWnd, bool _bWindowMode);
	int Run();

	HWND GetMainWindowHwnd() { return m_hWnd; }
public:
	void Update();
	void Render();


public:
	static CCore& GetInst()
	{
		static CCore inst;
		return inst;
	}

private:
	CCore();
	~CCore();
};

