#pragma once

#include "global.h"

class Script;

struct tScriptInfo
{
	wstring strScriptName;
	DWORD dwTypeID;
};

class CScriptMgr
{
public:
	static void GetAllScriptName(vector<tScriptInfo>& _vecScriptName);
	static Script* GetScript(DWORD _dwTypeID);
	static Script* GetScript(const wstring& _strScriptName);
	static wchar_t* GetScriptName(Script* _pScript);
};