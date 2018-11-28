#include "SceneManager.h"

#include "Script.h"
#include "Prefab.h"
#include "Scene.h"


Script::Script()
{
}

Script::~Script()
{
}


CGameObject * Script::Instantiate(CPrefab * _pPrefab, Vec3 _vPos, const wstring & _strLayerName)
{
	if (_pPrefab == NULL)
		return NULL;

	CGameObject* pCloneObj = _pPrefab->Instantiate();

	pCloneObj->GetTransform()->SetLocalPos(_vPos);

	CSceneManager::Manager().GetCurScene()->FindLayer(_strLayerName)->AddGameObject(pCloneObj);

	return pCloneObj;
}

