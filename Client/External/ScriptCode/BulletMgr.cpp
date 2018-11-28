#include "stdafx.h"
#include "BulletMgr.h"
#include "SaveLoadManager.h"

CBulletMgr::CBulletMgr()
{
}


CBulletMgr::~CBulletMgr()
{
	//CPrefab* pBullet = CResManager::Manager().FindPrefab(L"IoriBullet_108식");
	//SAFE_DELETE(pBullet);

}

void CBulletMgr::Awake()
{
	//CBulletMgr::GetInst()->LoadBullet(L"00Common");
}

void CBulletMgr::Update()
{
}

void CBulletMgr::LoadBullet(const wstring & _strFolder)
{
	if (NULL == CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect"))
		return;

	wstring strFolderPath = {};
	strFolderPath = CPathManager::GetResPathEX();
	strFolderPath += L"Save\\Bullet\\" + _strFolder;

	WIN32_FIND_DATA tData = {};
	HANDLE hHandle = FindFirstFile(wstring(strFolderPath + L"\\*.*").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == hHandle)
	{
		MessageBox(NULL, L"존재하지 않는 폴더입니다", L"실패", MB_OK);
		assert(NULL);
	}

	CGameObject* pBullet;
	wstring strExt;
	while (true)
	{
		strExt = CPathManager::GetExt(tData.cFileName);
		if (L".goc" == strExt)
		{
			//pBullet = CSaveLoadManager::Manager().LoadGameObjectOnly(wstring(strFolderPath + L"\\" + tData.cFileName));
			//CResManager::Manager().AddPrefab(pBullet->GetTagName(), pBullet);
			//CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect")->AddGameObject(pBullet);
		}

		if (0 == FindNextFile(hHandle, &tData))
			break;
	}
	FindClose(hHandle);
}

void CBulletMgr::ActiveBullet(const wstring & _strBulletTag, int _iDir)
{
}
