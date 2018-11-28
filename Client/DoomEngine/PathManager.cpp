#include "PathManager.h"

wchar_t CPathManager::m_szResPath[255] = {};
wchar_t CPathManager::m_szFullPath[255] = {};
wchar_t CPathManager::m_szExt[20] = {};
wchar_t CPathManager::m_szExternalPath[255] = {};

CPathManager::CPathManager()
{
}


CPathManager::~CPathManager()
{
}

void CPathManager::init()
{
	GetCurrentDirectory(255, m_szResPath);
	
	int iLen = (int)wcslen(m_szResPath);
	for (int i = iLen - 1; i >= 0; --i)
	{
		if (m_szResPath[i] == L'\\')
		{
			wmemset(m_szResPath + i, 0, iLen - 1);
			break;
		}
	}
	
	wcscpy_s(m_szExternalPath, 255, m_szResPath);
	wmemcpy_s(m_szFullPath, 255, m_szResPath, 255);

	wcscat_s(m_szResPath, 255, L"\\Bin\\Resource");
	wcscat_s(m_szFullPath, 255, L"\\Bin\\Resource\\");
	wcscat_s(m_szExternalPath, 255, L"\\External\\");
}

wchar_t * CPathManager::GetExt(const wchar_t * _pPath)
{
	_wsplitpath_s(_pPath, NULL, NULL, NULL, NULL, NULL, NULL, m_szExt, 20);
	return m_szExt;
}
