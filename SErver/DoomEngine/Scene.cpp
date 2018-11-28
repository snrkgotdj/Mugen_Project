#include "Scene.h"

#include "Layer.h"
#include "Camera.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "Scene.h"
#include "TransForm.h"


CScene::CScene()
	:m_pMainCamera(NULL)
{
	m_vecLayer.resize(MAX_LAYER);
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		m_setIdx.insert(i);
	}
}


CScene::~CScene()
{
	Safe_Delete_Map(m_mapLayer);
}

void CScene::Awake()
{
	m_pMainCamera = FindLayer(L"Camera")->FindObjectByTag(L"Main_Camera");
	m_pUICamera = FindLayer(L"Camera")->FindObjectByTag(L"UI_Camera");

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			m_vecLayer[i]->Awake();
		}
	}
}

void CScene::Start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			m_vecLayer[i]->Start();
		}
	}
}

void CScene::Update()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			m_vecLayer[i]->Update();
		}
	}
}

void CScene::LateUpdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			m_vecLayer[i]->LateUpdate();
		}
	}
}

void CScene::FinalUpdate()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			m_vecLayer[i]->FinalUpdate();
		}
	}
}

void CScene::Render()
{
	CLayer* pCamLayer = FindLayer(L"Camera");
	list<CGameObject*>& listObj = pCamLayer->GetObjList();
	
	list<CGameObject*>::iterator iter = listObj.begin();
	for (; iter != listObj.end(); ++iter)
	{
		CCamera* pCamera = (*iter)->GetCamera();
		for (UINT i = 0; i < MAX_LAYER; ++i)
		{
			if (NULL != m_vecLayer[i])
			{
				pCamera->Render(m_vecLayer[i]);
			}
		}
	}
}

void CScene::Render(CGameObject * _pCamObj)
{
	CCamera* pCamera = _pCamObj->GetCamera();
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			pCamera->Render(m_vecLayer[i]);
		}
	}
}

int CScene::AddLayer(const wstring & _strLayerName)
{
	CLayer* pLayer = FindLayer(_strLayerName);

	if (pLayer != NULL)
	{
		return RET_FAIL;
	}

	pLayer = new CLayer;

	UINT iIdx = *(m_setIdx.begin());
	m_setIdx.erase(m_setIdx.begin());

	pLayer->SetLayerIdx(iIdx);
	pLayer->SetLayerName(_strLayerName);

	m_mapLayer.insert(make_pair(_strLayerName, pLayer));
	m_vecLayer[iIdx] = pLayer;

	CSceneManager::Manager().SetLayerChange(true);

	return 0;
}

int CScene::AddLayer(CLayer* _pLayer, UINT _iIdx)
{
	if (NULL != FindLayer(_pLayer->GetLayerName()))
		assert(NULL);

	UINT iIdx = _pLayer->GetLayerIdx();

	if (NULL != m_vecLayer[iIdx])
		assert(NULL);

	m_mapLayer.insert(make_pair(_pLayer->GetLayerName(), _pLayer));
	m_vecLayer[iIdx] = _pLayer;

	DeleteIdx(iIdx);
	CSceneManager::Manager().SetLayerChange(true);
	return 0;
}

CLayer * CScene::FindLayer(const wstring & _strLayerName)
{ 
	map<wstring, CLayer*>::iterator iter = m_mapLayer.find(_strLayerName);

	if (iter == m_mapLayer.end())
		return NULL;

	return iter->second;
}

CGameObject * CScene::FindObjec(Vec2 _vMousePos, Vec2 _vResolution)
{
	Vec2 vWorldPos = MtoW(_vMousePos, _vResolution);

	CGameObject* pTarget = NULL;
	for (UINT i = 0; i < m_vecLayer.size(); ++i)
	{
		if (NULL != m_vecLayer[i])
		{
			pTarget = m_vecLayer[i]->FindObject(vWorldPos);
			if (NULL != pTarget)
				return pTarget;
		}
	}
	return nullptr;
}

void CScene::GetAllParentObject(list<CGameObject*>& _listOut)
{
	for (UINT i = 0; i < m_vecLayer.size(); ++i)
	{
		if (NULL == m_vecLayer[i])
			continue;

		list<CGameObject*>& listLayer = m_vecLayer[i]->GetParentList();
		_listOut.insert(_listOut.end(), listLayer.begin(), listLayer.end());
	}
}

Vec2 CScene::MtoW(Vec2 _vMousePos, Vec2 _vResolution)
{
	Vec2 vRatio = Vec2( WindowSizeX / _vResolution.x , WindowSizeY/ _vResolution.y );

	CLayer* pLayer = FindLayer(L"Camera");
	CGameObject* pMainCam = pLayer->FindObjectByTag(L"Main_Camera");
	if (pMainCam == NULL)
		return Vec2{};

	Vec3 vCamPos = pMainCam->GetTransform()->GetWorldPos();

	Vec2 vMousePos = {};
	vMousePos.x = _vMousePos.x * vRatio.x;
	vMousePos.y = _vMousePos.y * vRatio.y;

	Vec2 vMouseWorld = {};
	vMouseWorld.x = vMousePos.x - (WindowSizeX / 2.f) + vCamPos.x;
	vMouseWorld.y = (WindowSizeY / 2.f) - vMousePos.y + vCamPos.y;

	float fScale = pMainCam->GetCamera()->GetScale();

	Vec2 vDiff = Vec2(vMouseWorld.x - vCamPos.x, vMouseWorld.y - vCamPos.y);
	vDiff.x *= fScale;
	vDiff.y *= fScale;

	vMouseWorld.x = vCamPos.x + vDiff.x;
	vMouseWorld.y = vCamPos.y + vDiff.y;

	return vMouseWorld;
}

void CScene::DeleteLayer(const wstring & _strLayerName)
{
	map<wstring, CLayer*>::iterator iter = m_mapLayer.find(_strLayerName);

	if (iter == m_mapLayer.end())
		return;

	CLayer* pLayer = iter->second;
	m_mapLayer.erase(iter);

	m_vecLayer[pLayer->GetLayerIdx()] = NULL;

	m_setIdx.insert(pLayer->GetLayerIdx());

	SAFE_DELETE(pLayer);

	CSceneManager::Manager().SetLayerChange(true);
}

void CScene::DeleteIdx(UINT _iIdx)
{
	set<UINT>::iterator iter = m_setIdx.find(_iIdx);
	if (iter != m_setIdx.end())
		m_setIdx.erase(iter);

}
