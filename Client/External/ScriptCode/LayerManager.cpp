#include "stdafx.h"
#include "LayerManager.h"
#include "SceneManager.h"
#include "CollisionManager.h"

CLayerManager::CLayerManager()
{
}

CLayerManager::~CLayerManager()
{
}

void CLayerManager::Awake()
{
	//���̾� �߰�
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Map");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_1P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Hit_2P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_1P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Collider_2P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"1P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"2P");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Shadow");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Effect");
	CSceneManager::Manager().GetCurScene()->AddLayer(L"Font");	
}

void CLayerManager::Start()
{
	//�浹 ����
	CCollisionManager::Manager().CheckLayerCollision(L"Hit_1P", L"Collider_2P");
	CCollisionManager::Manager().CheckLayerCollision(L"Hit_2P", L"Collider_1P");

	//ī�޶� �Կ�
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Map");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"1P");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"2P");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Effect");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Hit_1P");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Hit_2P");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Shadow");
#ifdef _DEBUG
	//CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayerAll();
	
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Collider_1P");
	CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetCamera()->CheckRenderLayer(L"Collider_2P");
#endif 
}

void CLayerManager::Update()
{
}

