#pragma once

#include "global.h"
class CPathManager
{
private:
	static wchar_t m_szResPath[255];
	static wchar_t m_szExternalPath[255];
	static wchar_t m_szFullPath[255];
	static wchar_t m_szExt[20];

public:
	static void init();

public:
	static wchar_t* GetResPath() { return m_szResPath; }
	static wchar_t* GetExternalPath() { return m_szExternalPath; }
	static wchar_t* GetResPathEX() { return m_szFullPath; }
	static wchar_t* GetExt(const wchar_t* _pPath);

private:
	CPathManager();
	~CPathManager();
};

