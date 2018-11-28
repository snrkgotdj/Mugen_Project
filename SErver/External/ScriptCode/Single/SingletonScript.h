#pragma once
#include "Script.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"


template<typename T>
class CSingletonScript :
	public Script
{
private:
	static T* g_pThis;

public:
	static T* GetInst();
	static void DestroySingle();
	
public:
	CSingletonScript() {}
	virtual ~CSingletonScript()	{ g_pThis = NULL; }
};

template<typename T>
T* CSingletonScript<T>::g_pThis = NULL;

template<typename T>
inline T * CSingletonScript<T>::GetInst()
{
	if (NULL != g_pThis)
		return g_pThis;

	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	CLayer* pLayer = pCurScene->FindLayer(L"Default");
	CGameObject* pMgrObj = NULL;
	if(pLayer!=NULL)
		pMgrObj = pLayer->FindObjectByTag(L"ManagerObject");

	if (NULL == pMgrObj)
	{
		pMgrObj = new CGameObject;
		pMgrObj->AddComponent<CTransform>(new CTransform);
		pMgrObj->SetTagName(L"ManagerObject");
		pLayer->AddGameObject(pMgrObj);
	}

	g_pThis = new T;
	pMgrObj->AddComponent<Script>(g_pThis);

	return g_pThis;
}

template<typename T>
inline void CSingletonScript<T>::DestroySingle()
{
	//CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	//CLayer* pLayer = pCurScene->FindLayer(L"Default");
	//CGameObject* pMgrObj = pLayer->FindObjectByTag(L"ManagerObject");
	//
	//if (pMgrObj == NULL)
	//	return;

	//UINT iSize=pMgrObj->GetScriptList().size();
	//for(int i=0;i<iSize;++i)
	//	pMgrObj->DeleteScript(i);
	//pMgrObj->AddComponent<Script>(g_pThis);

	g_pThis = NULL;
}