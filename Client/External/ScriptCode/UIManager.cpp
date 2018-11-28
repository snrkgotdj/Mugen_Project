#include "stdafx.h"
#include "UIManager.h"
#include "GameObject.h"
#include "SaveLoadManager.h"
#include "SceneManager.h"
#include "FontMgr.h"
#include "PlayerManager.h"
#include "LayerManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "ModeMgr.h"
#include "BulletMgr.h"
#include "GameObject.h"
#include "AnimationEX.h"
#include "AnimatorEX.h"

#include "UIBar.h"
#include "GameMsg.h"
#include "CharactorScript.h"
#include "UICharacterSel.h"
#include "UICmdList.h"

CUIManager::CUIManager()
	: m_pUICamera(NULL)
	, m_pUI_HP_1P(NULL)
	, m_pUI_HP_2P(NULL)
	, m_pComboOutput_1P(NULL)
	, m_pComboOutput_2P(NULL)
	, m_bEnd(false)
	, m_cPlayer(0)
{
	m_strUI_HP_1P = L"UIBar1.goc";
	m_strUI_HP_2P = L"UIBar1.goc";
	m_strCombo = L"UICombo.goc";
	m_strGameMsg = L"UIGameMsg.goc";
}

CUIManager::~CUIManager()
{
}

void CUIManager::Awake()
{
	CGameObject* pObj = CSceneManager::Manager().GetCurScene()->FindLayer(L"Default")
		->FindObjectByTag(L"Waiting");
	if (pObj != NULL)
		pObj->Die();

	CSceneManager::Manager().GetCurScene()->AddLayer(L"UI");
	CreateUICam();

	wstring strUI1P;
	wstring strUI2P;

	if (!m_bTraining)
	{
		strUI1P = CPathManager::GetResPathEX();
		strUI1P += L"Save\\UI\\" + m_strUI_HP_1P;
		m_pUI_HP_1P = CSaveLoadManager::Manager().LoadGameObjectOnly(strUI1P);

		m_pUI_HP_1P->GetTransform()->SetLocalPos(Vec3(-330.f, 250.f, 0.f));
		CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pUI_HP_1P);

		m_pUI_HP_1P->Awake();
		m_pUI_HP_1P->Start();

		list<CGameObject*> listObj = m_pUI_HP_1P->GetChildList();
		list<CGameObject*>::iterator iter = listObj.begin();
		list<CGameObject*>::iterator iterEnd = listObj.end();

		CGameObject* pBar1P = NULL;
		CGameObject* pBar2P = NULL;

		for (iter; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTagName() == L"HPBar2P")
			{
				m_pUI_1P_ChildHP = (*iter);
			}
			else if ((*iter)->GetTagName() == L"GuardBar2P")
			{
				m_pUI_1P_ChildGuard = (*iter);
			}
		}

		((CUIBar*)m_pUI_1P_ChildHP->GetFirstScript())->SetPlayer(true);
	}

	strUI2P = CPathManager::GetResPathEX();
	strUI2P += L"Save\\UI\\" + m_strUI_HP_2P;
	m_pUI_HP_2P = CSaveLoadManager::Manager().LoadGameObjectOnly(strUI2P);

	wstring strUICombo;
	strUICombo = CPathManager::GetResPathEX();
	strUICombo += L"Save\\UI\\" + m_strCombo;
	m_pComboOutput_1P = CSaveLoadManager::Manager().LoadGameObjectOnly(strUICombo);
	m_pComboOutput_2P = CSaveLoadManager::Manager().LoadGameObjectOnly(strUICombo);

	wstring strGameMsg;
	strGameMsg = CPathManager::GetResPathEX();
	strGameMsg += L"Save\\UI\\" + m_strGameMsg;
	m_pGameMsg = CSaveLoadManager::Manager().LoadGameObjectOnly(strGameMsg);

	wstring strBtnOutput;
	strBtnOutput = CPathManager::GetResPathEX();
	strBtnOutput += L"Save\\UI\\UIBtnOutput.goc";

	wstring strCmdList;
	strCmdList = CPathManager::GetResPathEX();
	strCmdList += L"Save\\UI\\CmdList.goc";

	m_pUI_HP_2P->GetTransform()->SetLocalPos(Vec3(330.f, 250.f, 0.f));
	m_pUI_HP_2P->GetTransform()->SetLocalScale(Vec3(-1.f, 1.f, 1.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pUI_HP_2P);

	m_pComboOutput_1P->GetTransform()->SetLocalPos(Vec3(-330.f, 0.f, 0.f));
	m_pComboOutput_1P->GetTransform()->SetLocalScale(Vec3(150.f,50.f,1.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pComboOutput_1P);

	m_pComboOutput_2P->GetTransform()->SetLocalPos(Vec3(430.f, 0.f, 0.f));
	m_pComboOutput_2P->GetTransform()->SetLocalScale(Vec3(150.f,50.f, 1.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pComboOutput_2P);

	m_pGameMsg->GetTransform()->SetLocalPos(Vec3Zero);
	m_pGameMsg->GetTransform()->SetLocalScale(Vec3(400.f,100.f, 1.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pGameMsg);

	if (m_bTraining)
	{
		m_pCmdOutput = CSaveLoadManager::Manager().LoadGameObjectOnly(strBtnOutput);
		m_pCmdOutput->GetTransform()->SetLocalPos(Vec3(-530.f, 250.f, 0.f));
		m_pCmdOutput->GetTransform()->SetLocalScale(Vec3(2.f, 2.f, 1.f));
		CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(m_pCmdOutput);
		m_pCmdOutput->Awake();
		m_pCmdOutput->Start();

		m_pCmdList = CSaveLoadManager::Manager().LoadGameObjectOnly(strCmdList);
		m_pCmdList->GetTransform()->SetLocalPos(Vec3Zero);
		CSceneManager::Manager().GetCurScene()->FindLayer(L"Font")->AddGameObject(m_pCmdList);
		wstring strName = CPlayerManager::GetInst()->Get1P()->GetTagName();
		((CUICmdList*)(m_pCmdList->GetFirstScript()))->SetName(strName);
		m_pCmdList->Awake();
		m_pCmdList->Start();
	}

	m_pUI_HP_2P->Awake();
	m_pUI_HP_2P->Start();

	m_pComboOutput_1P->Awake();
	m_pComboOutput_1P->Start();	
	
	m_pComboOutput_2P->Awake();
	m_pComboOutput_2P->Start();

	list<CGameObject*> listObj = m_pUI_HP_2P->GetChildList();
	list<CGameObject*>::iterator iter2 = listObj.begin();
	list<CGameObject*>::iterator iter2End = listObj.end();

	for (iter2; iter2 != iter2End; ++iter2)
	{
		if ((*iter2)->GetTagName() == L"HPBar2P")
		{
			m_pUI_2P_ChildHP = (*iter2);
		}
		else if ((*iter2)->GetTagName() == L"GuardBar2P")
		{
			m_pUI_2P_ChildGuard = (*iter2);
		}
	}

	((CUIBar*)m_pUI_2P_ChildHP->GetFirstScript())->SetPlayer(false);
}

void CUIManager::Start()
{
	
}

void CUIManager::Update()
{
	if (m_bEnd)
	{
		int iMode = CModeMgr::GetInst()->GetMode();
		list<CGameObject*> listObj;
		CSceneManager::Manager().GetCurScene()->GetAllParentObject(listObj);
	
		list<CGameObject*>::iterator iter = listObj.begin();
		list<CGameObject*>::iterator iterEnd = listObj.end();
	
		CScene* pCurScene = CSceneManager::Manager().GetCurScene();
		CLayer* pLayer = pCurScene->FindLayer(L"Default");
		CGameObject* pMgrObj = pLayer->FindObjectByTag(L"ManagerObject");

		if (pMgrObj == NULL)
			return;

		for (iter; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetTagName() != L"ManagerObject")
			{
				CSceneManager::Manager().SetObjectChange(true);
				
				(*iter)->Die();
			}
		}
		
		CSound* pSound = CGameManager::GetInst()->GetSound();
		if (pSound != NULL)
			pSound->Stop(true);

		if(iMode!=0)
			CModeMgr::GetInst()->SetMode(iMode);
		else
			CModeMgr::GetInst()->SetMode(1);

		wstring strFullPath = CPathManager::GetResPathEX();
		strFullPath += L"Save\\Scene\\CharSel.scene";
		CSaveLoadManager::Manager().LoadScene(strFullPath);
		
		CGameObject* pObj = CSceneManager::Manager().GetCurScene()->FindLayer(L"Default")->FindObjectByTag(L"CharacterSel");
		CUICharacterSel* pScript = ((CUICharacterSel*)pObj->GetFirstScript());
		pScript->SetPlayer(m_cPlayer);
		////////////////////////////////////

		CSceneManager::Manager().GetCurScene()->AddLayer(L"Map");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Shadow");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Effect");

		CKeyManager::Manager().Active1P(true);
		CKeyManager::Manager().Active2P(true);

		m_bEnd = false;

		CPlayerManager::GetInst()->DestroySingle();
		CLayerManager::GetInst()->DestroySingle();
		CEffectManager::GetInst()->DestroySingle();
		CGameManager::GetInst()->DestroySingle();
		CUIManager::GetInst()->DestroySingle();
		CBulletMgr::GetInst()->DestroySingle();

		return;
	}

	if (/*m_pUI_HP_1P == NULL ||*/ m_pUI_HP_2P == NULL)
		return;

	CGameObject* pObj1P = CPlayerManager::GetInst()->Get1P();
	CGameObject* pObj2P = CPlayerManager::GetInst()->Get2P();

	float f1P = 1.f;
	if(m_pUI_1P_ChildHP!=NULL)
		f1P = ((CUIBar*)m_pUI_1P_ChildHP->GetFirstScript())->GetValue();
	float f2P = ((CUIBar*)m_pUI_2P_ChildHP->GetFirstScript())->GetValue();
	
	if (m_bTraining&&KEYDOWN(KEY_F3))
	{
		f2P = 1.f;
		((CUIBar*)m_pUI_2P_ChildHP->GetFirstScript())->SetValue(f2P);
	}

	if (m_pGameMsg == NULL)
		return;

	int iGameMsg=((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetState();

	if (iGameMsg != -1)
	{
		CKeyManager::Manager().Active1P(false);
		CKeyManager::Manager().Active2P(false);
	}
	else
	{
		if (true == CPlayerManager::GetInst()->isNetWork() && false == CPlayerManager::GetInst()->is1P())
		{
			CKeyManager::Manager().Active1P(false);
		}
		else
			CKeyManager::Manager().Active1P(true);

		if(CGameManager::GetInst()->GetActive2P())
			CKeyManager::Manager().Active2P(true);
	}

	if (f2P <= 0.f&&f1P > 0.f)//win1p
	{
		if (m_bTraining)
		{
			f2P = 1.f;
			((CUIBar*)m_pUI_2P_ChildHP->GetFirstScript())->SetValue(f2P);
			return;
		}

		int iState = ((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetState();
		if (iState != 2&& iState != 3&& iState != 4)
		{
			wstring strKO;
			strKO = CPathManager::GetResPathEX();
			strKO += L"Save\\UI\\KOMsg.goc";

			CGameObject* pObj = CSaveLoadManager::Manager().LoadGameObjectOnly(strKO);
			pObj->GetTransform()->SetLocalPos(Vec3Zero);
			CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(pObj);

			pObj2P->GetAnimatorEX()->ChangeAnimation(L"KO");
		}

		((CGameMsg*)(m_pGameMsg->GetFirstScript()))->SetState(2);
		((CCharactorScript*)(pObj2P->GetFirstScript()))->SetDefeated(true);
		
		if (((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetKOTime() > 2.f)		
			((CGameMsg*)(m_pGameMsg->GetFirstScript()))->SetState(3);
		
	}
	else if (f1P <= 0.f&&f2P > 0.f)//win2p
	{
		int iState = ((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetState();
		if (iState != 2&& iState != 3 && iState != 4)
		{
			wstring strKO;
			strKO = CPathManager::GetResPathEX();
			strKO += L"Save\\UI\\KOMsg.goc";

			CGameObject* pObj = CSaveLoadManager::Manager().LoadGameObjectOnly(strKO);
			pObj->GetTransform()->SetLocalPos(Vec3Zero);
			CSceneManager::Manager().GetCurScene()->FindLayer(L"UI")->AddGameObject(pObj);

			pObj1P->GetAnimatorEX()->ChangeAnimation(L"KO");
		}

		((CGameMsg*)(m_pGameMsg->GetFirstScript()))->SetState(2);
		((CCharactorScript*)(pObj1P->GetFirstScript()))->SetDefeated(true);

		if (((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetKOTime() > 2.f)		
			((CGameMsg*)(m_pGameMsg->GetFirstScript()))->SetState(4);	
	}

	if (((CGameMsg*)(m_pGameMsg->GetFirstScript()))->GetKOTime() > 4.f)
		m_bEnd = true;
}


void CUIManager::SetHP_1P(float _fHP)
{
	CUIBar* pHP_1P = (CUIBar*)m_pUI_HP_1P->GetFirstScript();
	pHP_1P->SetValue(_fHP);
}

void CUIManager::SetHP_2P(float _fHP)
{
	CUIBar* pHP_2P = (CUIBar*)m_pUI_HP_2P->GetFirstScript();
	pHP_2P->SetValue(_fHP);
}

void CUIManager::CreateUICam()
{
	m_pUICamera = new CGameObject;
	m_pUICamera->SetTagName(L"UI_Camera");
	m_pUICamera->AddComponent<CTransform>(new CTransform);
	m_pUICamera->AddComponent<CCamera>(new CCamera);
	m_pUICamera->GetTransform()->SetLocalPos(Vec3(0.f, 0.f, -1.f));
	m_pUICamera->GetCamera()->CheckRenderLayer(L"UI");
	m_pUICamera->GetCamera()->CheckRenderLayer(L"Font");

	CSceneManager::Manager().GetCurScene()->FindLayer(L"Camera")->AddGameObject(m_pUICamera);
}
