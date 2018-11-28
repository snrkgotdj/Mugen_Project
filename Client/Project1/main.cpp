#include <iostream>

#include "PathManager.h"

#include <vector>

using namespace std;

vector<wstring> g_vecScriptName;

int main()
{
	CPathManager::init();

	wstring strExternal = CPathManager::GetExternalPath();
	wstring strScriptPath = strExternal + L"\\ScriptCode\\";

	WIN32_FIND_DATA tData = {};
	HANDLE hHandle = FindFirstFile(wstring(strScriptPath + L"*.h").c_str(), &tData);

	while (true)
	{
		g_vecScriptName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
		if (!FindNextFile(hHandle, &tData))
			break;
	}

	FindClose(hHandle);

	strExternal += L"ScriptManager.cpp";

	FILE* pFile = NULL;
	_wfopen_s(&pFile, strExternal.c_str(), L"w");

	// 헤더
	fwprintf_s(pFile, L"#include \"stdafx.h\"\n");
	fwprintf_s(pFile, L"#include \"ScriptManager.h\"\n\n");

	for (UINT i = 0; i <g_vecScriptName.size(); ++i)
	{
		fwprintf_s(pFile, L"#include \"ScriptCode\\");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L".h\"\n");
	}
	//첫번째

	fwprintf_s(pFile, L"\nvoid CScriptMgr::GetAllScriptName(vector<tScriptInfo>& _vecScriptName)\n{\n");
	fwprintf_s(pFile, L"\ttScriptInfo info;\n\n");
	for (UINT i = 0; i <g_vecScriptName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tinfo.strScriptName = L\"");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L"\";\n");

		fwprintf_s(pFile, L"\tinfo.dwTypeID = typeid(C");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L").hash_code();\n");

		fwprintf_s(pFile, L"\t_vecScriptName.push_back(info);\n\n");
	}

	fwprintf_s(pFile, L"}\n\n");

	//두번째
	fwprintf_s(pFile, L"Script * CScriptMgr::GetScript(DWORD _dwTypeID)\n{\n");
	
	fwprintf_s(pFile, L"\tif (_dwTypeID == typeid(C");
	fwprintf_s(pFile, g_vecScriptName[0].c_str());
	fwprintf_s(pFile, L").hash_code())\n\t\treturn new C");
	fwprintf_s(pFile, g_vecScriptName[0].c_str());
	fwprintf_s(pFile, L";\n");

	for (UINT i = 1; i < g_vecScriptName.size(); ++i)
	{
		fwprintf_s(pFile, L"\telse if (_dwTypeID == typeid(C");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L").hash_code())\n\t\treturn new C");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L";\n");
	}

	fwprintf_s(pFile, L"\n\treturn nullptr;\n}\n\n");

	//세번째

	fwprintf_s(pFile, L"Script * CScriptMgr::GetScript(const wstring& _strScriptName)\n{\n");
	for (UINT i = 0; i < g_vecScriptName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tif (_strScriptName == L\"");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L"\")\n");
		fwprintf_s(pFile, L"\t\treturn new C");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L";\n\n");
	}
	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");



	//네번째
	fwprintf_s(pFile, L"wchar_t * CScriptMgr::GetScriptName(Script * _pScript)\n{\n");

	fwprintf_s(pFile, L"\tif (NULL != dynamic_cast<C");
	fwprintf_s(pFile, g_vecScriptName[0].c_str());
	fwprintf_s(pFile, L"*>(_pScript))\n\t\treturn L\"");
	fwprintf_s(pFile, g_vecScriptName[0].c_str());
	fwprintf_s(pFile, L"\";\n\n");

	for (UINT i = 1; i < g_vecScriptName.size(); ++i)
	{
		fwprintf_s(pFile, L"\telse if (NULL != dynamic_cast<C");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L"*>(_pScript))\n\t\treturn L\"");
		fwprintf_s(pFile, g_vecScriptName[i].c_str());
		fwprintf_s(pFile, L"\";\n\n");
	}

	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");

	fclose(pFile);
	return 0;
}