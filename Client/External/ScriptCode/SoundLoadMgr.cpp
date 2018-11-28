#include "stdafx.h"
#include "SoundLoadMgr.h"


CSoundLoadMgr::CSoundLoadMgr()
{
}


CSoundLoadMgr::~CSoundLoadMgr()
{
}

void CSoundLoadMgr::Awake()
{
	
}

void CSoundLoadMgr::Start()
{
}

void CSoundLoadMgr::Update()
{
}

void CSoundLoadMgr::LoadSoundFolder(const wstring & _strFolder)
{
	wstring strFolderPath = {};
	strFolderPath = CPathManager::GetResPathEX();
	strFolderPath += L"Sound\\" + _strFolder;

	WIN32_FIND_DATA tData = {};
	HANDLE hHandle = FindFirstFile(wstring(strFolderPath + L"\\*.*").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == hHandle)
	{
		MessageBox(NULL, L"폴더 ㅇㄷ?", L"사운드 로드 실패", MB_OK);
		assert(NULL);
	}

	wstring strExt;
	while (true)
	{
		strExt = CPathManager::GetExt(tData.cFileName);
		if (L".wav" == strExt||L".OGG"==strExt)
		{
			CResManager::Manager().Load<CSound>(tData.cFileName, _strFolder+L"\\"+tData.cFileName);
		}

		if (0 == FindNextFile(hHandle, &tData))
			break;
	}
	FindClose(hHandle);
}
