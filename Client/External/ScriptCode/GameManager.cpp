#include "stdafx.h"
#include "GameManager.h"
#include "EffectManager.h"
#include "PlayerManager.h"
#include "LayerManager.h"
#include "UIManager.h"
#include "SaveLoadManager.h"
#include "SoundLoadMgr.h"
#include "BulletMgr.h"

CGameManager::CGameManager()
	: m_bActive2P(false)
	, m_pSound(NULL)
{
}

CGameManager::~CGameManager()
{
	//CLayerManager::GetInst()->DestroySingle();
	//CEffectManager::GetInst()->DestroySingle();
	//CPlayerManager::GetInst()->DestroySingle();
	//CUIManager::GetInst()->DestroySingle();
}

void CGameManager::init()
{
	Awake();

	Start();
}

void CGameManager::Awake()
{
	CLayerManager::GetInst()->Awake();
	CEffectManager::GetInst()->Awake();
	CPlayerManager::GetInst()->Awake();
	CUIManager::GetInst()->Awake();
	CBulletMgr::GetInst()->Awake();
}

void CGameManager::Start()
{
	CLayerManager::GetInst()->Start();
	CEffectManager::GetInst()->Start();
	CPlayerManager::GetInst()->Start();
	CUIManager::GetInst()->Start();
	CBulletMgr::GetInst()->Start();
	
	m_pSound = (CSound*)CResManager::Manager().FindSound(L"KyoBGM1.wav");
	m_pSound->Play(1);
}

void CGameManager::Update()
{
	if (m_pSound != NULL)
	{
		m_pSound->SetVolume(0.3f);
	
		if (!m_pSound->GetIsPlaying())
		{
			m_pSound= (CSound*)CResManager::Manager().FindSound(L"KyoBGM2.wav");
			m_pSound->Play(1);
			m_pSound->SetVolume(0.3f);
		}
	}

	if (CKeyManager::Manager().GetKeyState(KEY_ESC, STATE_DOWN))
	{
		if(m_pSound!=NULL)
		m_pSound->Stop(true);

		wstring strFullPath = CPathManager::GetResPathEX();
		strFullPath += L"Save\\Scene\\Title.scene";
		CSaveLoadManager::Manager().LoadScene(strFullPath);

		CSceneManager::Manager().GetCurScene()->AddLayer(L"Map");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"1P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"2P");
		CSceneManager::Manager().GetCurScene()->AddLayer(L"Effect");

		CLayerManager::GetInst()->DestroySingle();
		CEffectManager::GetInst()->DestroySingle();
		CUIManager::GetInst()->DestroySingle();
		CPlayerManager::GetInst()->DestroySingle();
		CBulletMgr::GetInst()->DestroySingle();
		CSoundLoadMgr::GetInst()->DestroySingle();
		CGameManager::GetInst()->DestroySingle();

		CScene* pCurScene = CSceneManager::Manager().GetCurScene();
		CLayer* pLayer = pCurScene->FindLayer(L"Default");
		CGameObject* pMgrObj = pLayer->FindObjectByTag(L"ManagerObject");
		CGameObject* pTitle = pLayer->FindObjectByTag(L"TitleScreen");

		if(pMgrObj!=NULL)
			pMgrObj->Die();

		if (pTitle != NULL)
			pTitle->GetTransform()->SetLocalPos(Vec3Zero);
	}
}

