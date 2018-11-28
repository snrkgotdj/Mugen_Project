#include "stdafx.h"
#include "PlayerManager.h"

#include "SceneManager.h"
#include "SaveLoadManager.h"
#include "CollisionManager.h"
#include "PathManager.h"
#include "EffectManager.h"
#include "UIManager.h"

#include "CameraScript.h"
#include "CharactorScript.h"
#include "ShadowScript.h"
#include "MapScript.h"

#include "GameObject.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "ColScript.h"


CPlayerManager::CPlayerManager()
	: m_pGameObject_1P(NULL)
	, m_pGameObject_2P(NULL)
	, m_b1P(false)
	, m_bNetwork(false)
	, m_eMapName(MAP_END)
{
	m_str1P = L"Nanaya.goc";
	m_str2P = L"Nanaya.goc";
	m_strMap = L"Map.goc";
}

CPlayerManager::~CPlayerManager()
{
}

void CPlayerManager::NetworkOn_1P()
{
	CCharactorScript * Script_1P = ((CCharactorScript*)m_pGameObject_1P->GetFirstScript());

	Script_1P->Online(true);
	Script_1P->SetOnlineKey();
}

void CPlayerManager::NetworkOn_2P()
{
	CCharactorScript * Script_2P = ((CCharactorScript*)m_pGameObject_2P->GetFirstScript());

	Script_2P->Online(true);
	Script_2P->SetOnlineKey();
}

void CPlayerManager::SetName(const wstring & _strName)
{
	if (m_b1P == true)
	{
		m_str2P = _strName;
	}

	else
	{
		m_str1P = _strName;
	}
}

void CPlayerManager::SetNetwork()
{
	if (m_b1P == true)
		NetworkOn_1P();

	else
	{
		CKeyManager::Manager().Active1P(false);
		NetworkOn_2P();
	}
	m_bNetwork = true;
}

void CPlayerManager::SetState(UINT _iState, bool _is1P)
{
	CCharactorScript* pScript = NULL;

	if (_is1P == true)
	{
		pScript = (CCharactorScript*)m_pGameObject_1P->GetFirstScript();
	}
	else if (_is1P == false)
	{
		pScript = (CCharactorScript*)m_pGameObject_2P->GetFirstScript();
	}

	pScript->SetState(_iState);
}

void CPlayerManager::SetPos(const Vec3 & _vecPos, bool _is1P)
{
	CCharactorScript* pScript = NULL;

	if (_is1P == true)
	{
		pScript = (CCharactorScript*)m_pGameObject_1P->GetFirstScript();
	}
	else if (_is1P == false)
	{
		pScript = (CCharactorScript*)m_pGameObject_2P->GetFirstScript();
	}

	pScript->SetPos(_vecPos);
}

void CPlayerManager::Awake()
{
	wstring strFullPath1P;
	wstring strFullPath2P;
	wstring strFullPathMap;

	//1P
	strFullPath1P = CPathManager::GetResPathEX();
	strFullPath1P += L"Save\\GameObject\\" + m_str1P;
	m_pGameObject_1P = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath1P);

	//2P
	strFullPath2P = CPathManager::GetResPathEX();
	strFullPath2P += L"Save\\GameObject\\" + m_str2P;
	m_pGameObject_2P = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath2P);

	////Map
	strFullPathMap = CPathManager::GetResPathEX();
	strFullPathMap += L"Save\\GameObject\\" + m_strMap;
	m_pMap = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPathMap);

	if (NULL == m_pGameObject_1P || NULL == m_pGameObject_2P)
	{
		MessageBox(NULL, L"플레이어 로드 실패", L"로드 실패", MB_OK);
	}

	if (m_pGameObject_1P == NULL || m_pGameObject_2P == NULL)
		return;

	m_pGameObject_1P->Awake();
	m_pGameObject_2P->Awake();
	m_pMap->Awake();

	CCameraScript* pCamScript = new CCameraScript;
	pCamScript->Set1P(m_pGameObject_1P);
	pCamScript->Set2P(m_pGameObject_2P);

	CSceneManager::Manager().GetCurScene()->GetMainCamera()->AddComponent<Script>(pCamScript);
}

void CPlayerManager::Start()
{
	if (m_pGameObject_1P == NULL || m_pGameObject_2P == NULL)
		return;

	m_pGameObject_1P->Start();
	m_pGameObject_2P->Start();
	m_pMap->Start();

	CCharactorScript * Script_1P = ((CCharactorScript*)m_pGameObject_1P->GetFirstScript());
	CCharactorScript * Script_2P = ((CCharactorScript*)m_pGameObject_2P->GetFirstScript());

	// 1P 설정
	Script_1P->Set1P();
	float fFloorY = Script_1P->GetFloorY();
	m_pGameObject_1P->GetTransform()->SetLocalPos(Vec3(-150.f, fFloorY, 0.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"1P")->AddGameObject(m_pGameObject_1P);

	// 2P 설정
	Script_2P->Set2P();
	fFloorY = Script_2P->GetFloorY();
	m_pGameObject_2P->GetTransform()->SetLocalPos(Vec3(150.f, fFloorY, 0.f));
	CSceneManager::Manager().GetCurScene()->FindLayer(L"2P")->AddGameObject(m_pGameObject_2P);

	// 충돌박스 레이어 설정
	list<CGameObject*> listChild_1P = m_pGameObject_1P->GetChildList();
	list<CGameObject*> listChild_2P = m_pGameObject_2P->GetChildList();

	for (auto& pair : listChild_1P)
	{
		if (NULL != dynamic_cast<CColScript*>(*pair->GetScriptList().begin()))
		{
			((CColScript*)(*pair->GetScriptList().begin()))->MoveLayer(true);
			((CColScript*)(*pair->GetScriptList().begin()))->Set1P(true);
		}
	}

	for (auto& pair : listChild_2P)
	{
		if (NULL != dynamic_cast<CColScript*>(*pair->GetScriptList().begin()))
		{
			((CColScript*)(*pair->GetScriptList().begin()))->MoveLayer(false);
			((CColScript*)(*pair->GetScriptList().begin()))->Set1P(false);
		}
	}

	CSceneManager::Manager().GetCurScene()->FindLayer(L"Map")->AddGameObject(m_pMap);

	if (m_eMapName == MAP_END)
		m_eMapName = (MAP_STATE)getRandomNumber_int((int)MAP_KOREA, (int)(MAP_END - 1));

	((CMapScript*)m_pMap->GetFirstScript())->init(m_eMapName);
}

void CPlayerManager::Update()
{
	if (m_pGameObject_1P == NULL || m_pGameObject_2P == NULL)
		return;

	if (KEYDOWN(KEY_F3)&&CUIManager::GetInst()->GetTraining())
	{
		CCharactorScript * Script_1P = ((CCharactorScript*)m_pGameObject_1P->GetFirstScript());
		CCharactorScript * Script_2P = ((CCharactorScript*)m_pGameObject_2P->GetFirstScript());

		// 1P 설정
		Script_1P->Set1P();
		float fFloorY = Script_1P->GetFloorY();
		m_pGameObject_1P->GetTransform()->SetLocalPos(Vec3(-150.f, fFloorY, 0.f));

		// 2P 설정
		Script_2P->Set2P();
		fFloorY = Script_2P->GetFloorY();
		m_pGameObject_2P->GetTransform()->SetLocalPos(Vec3(150.f, fFloorY, 0.f));
	}

	wstring str1PClip = m_pGameObject_1P->GetAnimatorEX()->GetCurAnim()->GetKey();
	wstring str2PClip = m_pGameObject_2P->GetAnimatorEX()->GetCurAnim()->GetKey();

	if (m_pGameObject_1P->GetTransform()->GetLocalPos().x
		< m_pGameObject_2P->GetTransform()->GetLocalPos().x)
	{
		if (str1PClip == L"Idle")
			((CCharactorScript*)m_pGameObject_1P->GetFirstScript())->SetDir(DIR_RIGHT);
		if (str2PClip == L"Idle" )
			((CCharactorScript*)m_pGameObject_2P->GetFirstScript())->SetDir(DIR_LEFT);
	}

	else
	{
		if (str1PClip == L"Idle")
			((CCharactorScript*)m_pGameObject_1P->GetFirstScript())->SetDir(DIR_LEFT);
		if (str2PClip == L"Idle")
			((CCharactorScript*)m_pGameObject_2P->GetFirstScript())->SetDir(DIR_RIGHT);
	}

	float xLength = abs(m_pGameObject_1P->GetTransform()->GetLocalPos().x - m_pGameObject_2P->GetTransform()->GetLocalPos().x);
	float yLength = abs(m_pGameObject_1P->GetTransform()->GetLocalPos().y - m_pGameObject_2P->GetTransform()->GetLocalPos().y);

	if (xLength <= 50 && yLength <= 50)
	{
		((CCharactorScript*)m_pGameObject_1P->GetFirstScript())->PushOtherPlayer(m_pGameObject_2P);
		((CCharactorScript*)m_pGameObject_2P->GetFirstScript())->PushOtherPlayer(m_pGameObject_1P);
	}
}

