#include "stdafx.h"
#include "UICmdList.h"

#include "FontMgr.h"
#include "PathManager.h"

#include "GameManager.h"
#include "PlayerManager.h"
#include "UIManager.h"

#include "UIBtnOutput.h"

CUICmdList::CUICmdList()
	: m_iMode(1)
	, m_bCalled(false)
{
	
}


CUICmdList::~CUICmdList()
{
}

void CUICmdList::Awake()
{
}

void CUICmdList::Start()
{
	//m_strName = L"Kyo";
}

void CUICmdList::Update()
{
	Vec3 vPos = GetTransform()->GetLocalPos();

	if (KEYPUSH(KEY_TAB))
	{
		if (KEYDOWN(KEY_LEFT))
		{
			m_iMode--;
			GetMeshRender()->TextInfoClear();
			m_bCalled = false;
		}
		else if (KEYDOWN(KEY_RIGHT))
		{
			m_iMode++;
			GetMeshRender()->TextInfoClear();
			m_bCalled = false;
		}

		if (!m_bCalled)
		{
			
		}
		vPos.z = 0.f;
	}
	else
	{
		vPos.z = 3000.f;
		m_bCalled = false;
	}

	if (m_iMode < 1)m_iMode = 4;
	else if (m_iMode > 4)m_iMode = 1;

	GetTransform()->SetLocalPos(vPos);
}

void CUICmdList::LateUpdate()
{
	//if (false == CGameManager::GetInst()->GetActive2P())
	//{
	if (KEYPUSH(KEY_TAB))
	{
		((CUIBtnOutput*)(CUIManager::GetInst()->GetCmdOutput()->GetFirstScript()))->SetActive(false);
		((CUIBtnOutput*)(CUIManager::GetInst()->GetCmdOutput()->GetFirstScript()))->Clear();

		if (!m_bCalled)
		{
			wchar_t szMode[16];
			_itow_s(m_iMode, szMode, 10);
			wstring strPath;
			strPath = CPathManager::GetResPathEX();

			wstring strFileName;

			if (m_strName != L"")
			{
				strFileName = strPath + L"CmdListTxt\\" + m_strName + szMode + L".txt";
			}
			else
				return;

			wchar_t str[255] = {};

			FILE* pFile;
			errno_t err = _wfopen_s(&pFile, strFileName.c_str(), L"rb");

			fseek(pFile, 0, SEEK_CUR);
			fgetws(str, 255, pFile);
			for (int j = 0; j < 255; ++j)
			{
				if (str[j] == L'|')
				{
					str[j] = L'\0';
				}
			}

			tTextOutput tText = {};
			tText.strText = str;
			tText.fPosX = 150.f;
			tText.fPosY = 100.f;
			tText.fSize = 30.f;
			tText.iColor = ABGR(255, 255, 255, 255);
			GetMeshRender()->TextInfoPushBack(tText);

			for (int i = 0; i < 15; ++i)
			{
				fseek(pFile, 0, SEEK_CUR);
				fgetws(str, 255, pFile);
				for (int j = 0; j < 255; ++j)
				{
					if (str[j] == L'|')
					{
						str[j] = L'\0';
					}
				}
				tTextOutput tText = {};
				tText.strText = str;
				tText.fPosX = 150.f;
				tText.fPosY = 100.f + 30.f*(i + 1);
				tText.fSize = 20.f;
				tText.iColor = ABGR(255, 255, 255, 255);
				GetMeshRender()->TextInfoPushBack(tText);
			}

			fclose(pFile);

			m_bCalled = true;
		}
	}

	if (KEYUP(KEY_TAB))
	{
		GetMeshRender()->TextInfoClear();
		((CUIBtnOutput*)(CUIManager::GetInst()->GetCmdOutput()->GetFirstScript()))->SetActive(true);
	}
	//}

	//wchar_t str[128] = {};
	//wsprintf(str, L"");
	//
	//CFontMgr::Manager().DrawFont(str, 100.f, 100.f, 30.f, ABGR(255, 255, 255, 255));
}
