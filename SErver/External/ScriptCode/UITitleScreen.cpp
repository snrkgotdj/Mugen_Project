#include "stdafx.h"
#include "UITitleScreen.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "SaveLoadManager.h"
#include "GameManager.h"
#include "UIManager.h"

#include "UICharacterSel.h"
#include "PlayerManager.h"

//서버관련 헤더
#include "..\Doom\define_server.h"
#include "..\ServerLib\ClientManager.h"
#include "..\Doom\ClientMgr.h"
#include "PlayerManager.h"
#include "ModeMgr.h"


void CUITitleScreen::BattleMode()
{
	wstring strFullPath = CPathManager::GetResPathEX();
	strFullPath += L"Save\\UI\\UICharacterSel.goc";
	//CPlayerManager::GetInst()->
	CGameObject* pCharacterSel = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath);
	((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetPlayMode(1);
	((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetTrainMode(m_iMode);
	GetTransform()->SetLocalScale(Vec3Zero);
}

void CUITitleScreen::OnlineMode()
{
	ClientMgr::Manager().SetServer(true);
	ServerEngine::CClientManager::Manager().init(SERVER_IP, PORT);
	ServerEngine::CClientManager::Manager().Write(PH_JOIN, "Join", 4);

	GetTransform()->SetLocalScale(Vec3Zero);

	wstring strObj;
	strObj = CPathManager::GetResPathEX();
	strObj += L"Save\\UI\\Waiting.goc";

	CGameObject* pObj= CSaveLoadManager::Manager().LoadGameObjectOnly(strObj);
	//CSceneManager::Manager().GetCurScene()->FindLayer(L"Default")->AddGameObject(pObj);
}

CUITitleScreen::CUITitleScreen()
	: m_bEnterPushed(false)
{
}


CUITitleScreen::~CUITitleScreen()
{
}

void CUITitleScreen::Awake()
{
}

void CUITitleScreen::Start()
{
}

void CUITitleScreen::Update()
{
	wstring strChildName;
	list<CGameObject*> listChild = GetGameObject()->GetChildList();
	list<CGameObject*>::iterator iter = listChild.begin();
	list<CGameObject*>::iterator iterEnd = listChild.end();

	if (!m_bEnterPushed)
	{
		for (iter; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTagName() != L"EnterKey"&& (*iter)->GetTagName() != L"TitleBG")
			{
				(*iter)->GetTransform()->SetLocalScale(Vec3Zero);
			}
		}
		if (CKeyManager::Manager().GetKeyState(KEY_ENTER, STATE_DOWN))
		{
			m_bEnterPushed = true;
		}
	}

	else
	{
		if (CKeyManager::Manager().GetKeyState(KEY_ENTER, STATE_DOWN)&&GetTransform()->GetLocalScale()!=Vec3Zero)
		{
			wstring strFullPath = CPathManager::GetResPathEX();
			strFullPath += L"Save\\UI\\UICharacterSel.goc";
			switch (m_iMode)
			{
			case 0:
			{
				CUIManager::GetInst()->SetTraining(true);

				CGameObject* pCharacterSel = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath);
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetPlayMode(1);
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetTrainMode(m_iMode);
				GetTransform()->SetLocalScale(Vec3Zero);
			}
				break;
			case 1:
			{
				CGameObject* pCharacterSel = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath);
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetPlayMode(1);
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetTrainMode(m_iMode);
				GetTransform()->SetLocalScale(Vec3Zero);
				CUIManager::GetInst()->SetTraining(false);
			}
				break;

			case 2:
				BattleMode();
				CUIManager::GetInst()->SetTraining(false);
				break;
			case 3:
				OnlineMode();
				CUIManager::GetInst()->SetTraining(false);
				break;
			}

			
			//GetGameObject()->Die();
		}

		if (m_iMode < 0)m_iMode = 3;
		if (m_iMode > 3)m_iMode = 0;

		if (GetTransform()->GetLocalScale() != Vec3Zero)
		{
			if (CKeyManager::Manager().GetKeyState(KEY_DOWN, STATE_DOWN))
				m_iMode++;
			else if (CKeyManager::Manager().GetKeyState(KEY_UP, STATE_DOWN))
				m_iMode--;

			CModeMgr::GetInst()->SetMode(m_iMode);

			switch (m_iMode)
			{
			case 0:
				strChildName = L"mode_training";
				CGameManager::GetInst()->SetActive2P(false);
				break;
			case 1:
				strChildName = L"mode_1p";
				CGameManager::GetInst()->SetActive2P(true);
				break;
			case 2:
				strChildName = L"mode_2p";
				CGameManager::GetInst()->SetActive2P(true);
				break;
			case 3:
				strChildName = L"mode_online";
				CGameManager::GetInst()->SetActive2P(true);
				break;
			}
		}

		for (iter; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTagName() == strChildName)
			{
				CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Red");
				(*iter)->GetTransform()->SetLocalScale(Vec3(400.f, 100.f, 1.f));
				(*iter)->GetMeshRender()->SetMaterial(pMtrl);
			}
			else if ((*iter)->GetTagName() == L"EnterKey")
			{
				(*iter)->GetAnimatorEX()->SetCurAnim(NULL);
			}
			else if ((*iter)->GetTagName() == L"TitleBG")
			{
				(*iter)->GetTransform()->SetLocalScale(Vec3(1280.f, 720.f, 1.f));
			}
			else
			{
				CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
				(*iter)->GetTransform()->SetLocalScale(Vec3(400.f, 100.f, 1.f));
				(*iter)->GetMeshRender()->SetMaterial(pMtrl);
			}
		}
	}
}

void CUITitleScreen::LateUpdate()
{
	if (m_bEnterPushed)
	{
		/*if (KEYDOWN(KEY_ENTER))
			;*/
	}
}
