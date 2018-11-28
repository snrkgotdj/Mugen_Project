#include "SceneManager.h"
#include "ResManager.h"
#include "ShaderManager.h"
#include "CollisionManager.h"

#include "Scene.h"
#include "GameObject.h"

#include "TransForm.h"
#include "Camera.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Animator.h"
#include "Animation.h"
#include "Script.h"

#include "Layer.h"
#include "Texture.h"

#include "Material.h"
#include "Collider.h"

CSceneManager* CSceneManager::m_pInst = NULL;


CSceneManager::CSceneManager()
	: m_pCurScene(NULL)
	, m_pPrevScene(NULL)
	, m_bIsPlayMode(false)
	, m_bObjChange(false)
	, m_bLayerChnage(false)
	, m_pToolCam(NULL)
{
}

CSceneManager::~CSceneManager()
{
	SAFE_DELETE(m_pCurScene);
	SAFE_DELETE(m_pPrevScene);
}

void CSceneManager::SetPlayMode(bool _bTrue)
{
	m_bIsPlayMode = _bTrue;
}

void CSceneManager::InitScene()
{
	if (NULL != m_pCurScene)
	{
		m_pCurScene->Awake();
		m_pCurScene->Start();
	}
}

void CSceneManager::ChangeScene(CScene* _pNewScene)
{
	if (_pNewScene == NULL)
		MessageBox(NULL, L"오류", L"바꾸려는 씬이 없습니다.", MB_OK);

	DeleteScene(m_pPrevScene);
	m_pPrevScene = m_pCurScene;

	m_pCurScene = _pNewScene;
	
	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneManager::init()
{
	//CSound* pSound = (CSound*)CResManager::Manager().Load<CSound>(L"test", L"Sound\\Nope.wma");
	//pSound->Play(2);

	m_pCurScene = new CScene;

	m_pCurScene->AddLayer(L"Default");
	m_pCurScene->AddLayer(L"Transparent");
	m_pCurScene->AddLayer(L"Camera");
	m_pCurScene->AddLayer(L"UI");

	CreateCamera();

	m_pCurScene->Awake();
	m_pCurScene->Start();
}

void CSceneManager::progress()
{
	if (m_bIsPlayMode)
	{
		m_pCurScene->Update();
		m_pCurScene->LateUpdate();
	}

	m_pCurScene->FinalUpdate();

	CCollisionManager::Manager().Update();
}

void CSceneManager::CreateCamera()
{
	CGameObject* pObjCamera = new CGameObject;

	pObjCamera->SetTagName(L"Main_Camera");
	pObjCamera->AddComponent<CTransform>(new CTransform);
	pObjCamera->AddComponent<CCamera>(new CCamera);
	pObjCamera->GetTransform()->SetLocalPos(Vec3(0.f, 0.f, -1.f));
	pObjCamera->GetCamera()->CheckRenderLayer(L"Default");

	CLayer* pLayer = m_pCurScene->FindLayer(L"Camera");
	pLayer->AddGameObject(pObjCamera);
}

void CSceneManager::Render()
{
	if (m_pToolCam == NULL)
		m_pCurScene->Render();

	else
	{
		if (m_bIsPlayMode)
		{
			m_pCurScene->Render();
		}
		else
		{
			if (m_pToolCam != NULL)
				m_pCurScene->Render(m_pToolCam);
		}
	}
}

void CSceneManager::DeleteScene(CScene* _pScene)
{
	SAFE_DELETE(_pScene);
}