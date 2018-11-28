#pragma once
#include "Component.h"

#include "MeshRender.h"
#include "TransForm.h"
#include "Camera.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "ResManager.h"
#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"

class CPrefab;

class Script :
	public CComponent
{
public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() = 0;
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

protected:
	float DelTaTime() { return CTimeManager::Manager().GetDeltaTime(); }
	CGameObject* Instantiate(CPrefab* _Prefab, Vec3 _vPos, const wstring& _strLayerName = L"Default");

public:
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

public:
	virtual Script* Clone() = 0;

public:
	Script();
	virtual ~Script();
};

