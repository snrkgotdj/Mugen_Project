#include "stdafx.h"
#include "SaveLoadManager.h"

#include "SceneManager.h"
#include "Scene.h"

#include "ScriptManager.h"
#include "ResManager.h"

#include "Script.h"

#include "TransForm.h"
#include "MeshRender.h"
#include "Collider2D.h"
#include "Animator.h"
#include "AnimatorEX.h"
#include "Camera.h"
#include "func.h"

#include "Prefab.h"

CSaveLoadManager::CSaveLoadManager()
{
}


CSaveLoadManager::~CSaveLoadManager()
{
}

int CSaveLoadManager::SaveScene(const wstring & _strFilePath)
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	if (NULL == pCurScene)
		return RET_FAIL;

	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	if (NULL == pFile)
	{
		MessageBox(NULL, L"세이브 실패", L"세이브 경로 오류", MB_OK);
		assert(NULL);
	}

	// Save Resource
	CResManager::Manager().Save(pFile);

	// Save Prefab
	map<wstring, CResPtr<CPrefab>>& mapPrefab = CResManager::Manager().GetPrefabList();
	UINT iCount = mapPrefab.size();
	fwrite(&iCount, sizeof(UINT), 1, pFile);

	for (auto& pair : mapPrefab)
	{
		SaveResKey((CRes*)pair.second, pFile);
		SaveGameObject(pair.second->gameobject(), pFile);
	}

	//Scene이 보유한 레이어 개수
	vector<CLayer*>& vecLayer = pCurScene->GetVecLayer();

	iCount = pCurScene->GetLayerCount();
	fwrite(&iCount, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < vecLayer.size(); ++i)
	{
		if (NULL == vecLayer[i])
			continue;

		SaveLayer(vecLayer[i], pFile);
	}

	fclose(pFile);
	return RET_SUCCESS;
}

int CSaveLoadManager::SaveLayer(CLayer * _pLayer, FILE * _pFile)
{
	int iIdx = _pLayer->GetLayerIdx();
	fwrite(&iIdx, sizeof(UINT), 1, _pFile);

	const wstring& strLayerName = _pLayer->GetLayerName();
	SaveWString(strLayerName, _pFile);

	list<CGameObject*>& listParentObj = _pLayer->GetParentList();
	UINT iCount = listParentObj.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);

	list<CGameObject*>::iterator iter = listParentObj.begin();
	for (; iter != listParentObj.end(); ++iter)
	{
		SaveGameObject((*iter), _pFile);
	}

	return RET_SUCCESS;
}

int CSaveLoadManager::SaveGameObject(CGameObject * _pObj, FILE * _pFile)
{
	// Tag 저장
	const wstring& strTag = _pObj->GetTagName();
	SaveWString(strTag, _pFile);

	// Component 정보
	UINT i = (UINT)COMPONENT_TYPE::TRANSFORM;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _pObj->GetComponent((COMPONENT_TYPE)i);
		if (NULL == pComponent)
			continue;
		pComponent->Save(_pFile);
	}

	// Component 마감!!
	fwrite(&i, sizeof(UINT), 1, _pFile);

	// Script 정보
	list<Script*>& listScript = _pObj->GetScriptList();
	UINT iCount = listScript.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);

	list<Script*>::iterator iter = listScript.begin();
	wstring strScriptName;
	for (; iter != listScript.end(); ++iter)
	{
		strScriptName = CScriptMgr::GetScriptName((*iter));
		SaveWString(strScriptName, _pFile);
		(*iter)->Save(_pFile);
	}

	list<CGameObject*>& listChild = _pObj->GetChildList();
	iCount = listChild.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);

	list<CGameObject*>::iterator childIter = listChild.begin();
	for (; childIter != listChild.end(); ++childIter)
	{
		SaveGameObject(*childIter, _pFile);
	}

	return RET_SUCCESS;
}

int CSaveLoadManager::LoadScene(const wstring & _strFilePath)
{
	CScene* pCurScene = new CScene;

	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (NULL == pFile)
	{
		MessageBox(NULL, L"로드 실패", L"로드 경로 오류", MB_OK);
		return RET_FAIL;
	}

	CResManager::Manager().Load(pFile);

	// Load Prefab
	map<wstring, CResPtr<CPrefab>>& mapPrefab = CResManager::Manager().GetPrefabList();
	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		wstring strKey = LoadResKey(pFile);
		CGameObject* pObj = LoadGameObject(pFile);
		CResManager::Manager().AddPrefab(strKey, pObj);
	}

	iCount = 0;
	fread(&iCount, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		CLayer* pLayer = LoadLayer(pFile);
		if (NULL == pLayer)
			return RET_FAIL;

		pCurScene->AddLayer(pLayer);
	}

	fclose(pFile);

	CSceneManager::Manager().ChangeScene(pCurScene);

	return 0;
}

CLayer * CSaveLoadManager::LoadLayer(FILE * _pFile)
{
	CLayer* pLayer = new CLayer;

	// Layer Indx
	int iIdx = 0;
	fread(&iIdx, sizeof(UINT), 1, _pFile);
	pLayer->SetLayerIdx(iIdx);

	// Layer Name
	wstring strLayerName = LoadWString(_pFile);
	pLayer->SetLayerName(strLayerName);

	// GameObject 읽기
	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		CGameObject* pObj = LoadGameObject(_pFile);
		pLayer->AddGameObject(pObj);
	}

	return pLayer;
}

CGameObject * CSaveLoadManager::LoadGameObject(FILE * _pFile)
{
	CGameObject* pObj = new CGameObject;

	// Tag 저장
	pObj->m_strTag = LoadWString(_pFile);

	// Component 정보
	COMPONENT_TYPE eType = (COMPONENT_TYPE)0;
	CComponent* pComponent = NULL;
	while (eType != COMPONENT_TYPE::END)
	{
		fread(&eType, sizeof(UINT), 1, _pFile);

		switch (eType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			pComponent = new CTransform;
			pComponent->Load(_pFile);
			pObj->AddComponent<CTransform>((CTransform*)pComponent);
			break;

		case COMPONENT_TYPE::MESHRENDER:
			pComponent = new CMeshRender;
			pComponent->Load(_pFile);
			pObj->AddComponent<CMeshRender>((CMeshRender*)pComponent);
			break;

		case COMPONENT_TYPE::COLLIDER_2D:
			pComponent = new Collider2D;
			pComponent->Load(_pFile);
			pObj->AddComponent<CCollider>((Collider2D*)pComponent);
			break;

		case COMPONENT_TYPE::COLLIDER_3D:

			break;

		case COMPONENT_TYPE::ANIMATOR_EX:
			pComponent = new CAnimatorEX;
			pComponent->Load(_pFile);
			pObj->AddComponent<CAnimatorEX>((CAnimatorEX*)pComponent);
			break;

		case COMPONENT_TYPE::CAMERA:
			pComponent = new CCamera;
			pComponent->Load(_pFile);
			pObj->AddComponent<CCamera>((CCamera*)pComponent);
			break;
		}
	}

	// Script 정보
	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, _pFile);
	wstring strScriptName;

	for (UINT i = 0; i < iCount; ++i)
	{
		strScriptName = LoadWString(_pFile);
		Script* pScript = CScriptMgr::GetScript(strScriptName);
		pScript->Load(_pFile);
		pObj->AddComponent<Script>(pScript);
	}

	// Child Object 정보
	fread(&iCount, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		CGameObject* pChildObj = LoadGameObject(_pFile);
		pObj->AddChild(pChildObj);
	}

	return pObj;
}

int CSaveLoadManager::SaveGameObjectOnly(const wstring & _strFilePath, CGameObject * _pObj)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"wb");

	if (NULL == pFile)
	{
		MessageBox(NULL, L"세이브 실패", L"세이브 경로 오류", MB_OK);
		return false;
	}

	CResManager::Manager().Save(pFile);

	SaveGameObject(_pObj, pFile);

	fclose(pFile);

	return 0;
}

CGameObject * CSaveLoadManager::LoadGameObjectOnly(const wstring & _strFilePath)
{
	FILE* pFile = NULL;
	_wfopen_s(&pFile, _strFilePath.c_str(), L"rb");

	if (NULL == pFile)
	{
		MessageBox(NULL, L"로드 실패", L"로드 경로 오류", MB_OK);
		return NULL;
	}

	CResManager::Manager().Load(pFile);

	CGameObject* pObj = LoadGameObject(pFile);

	CSceneManager::Manager().GetCurScene()->FindLayer(L"Default")->AddGameObject(pObj);

	fclose(pFile);

	return pObj;
}

