#pragma once

#include "global.h"

class CGameObject;
class CTransform;
class CMeshRender;
class CCamera;
class CCollider;
class CAnimator;
class CAnimatorEX;
class Script;

class CComponent
{
private:
	CGameObject* m_pGameObj;


public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() = 0;
	virtual void LateUpdate() {}
	virtual void FinalUpdate() {}

private:
	void SetGameObject(CGameObject* _pObj) { m_pGameObj = _pObj; }

public:
	CGameObject* GetGameObject() { return m_pGameObj; }
	CGameObject* GetParentObject();
	CTransform* GetTransform();
	CMeshRender* GetMeshRender();
	CCamera* GetCamera();
	CCollider* GetCollider();
	CAnimatorEX* GetAnimatorEX();
	Script* GetFirstScript();

public:
	virtual CComponent* Clone() = 0;
	virtual void Save(FILE* _pFile) {};
	virtual void Load(FILE* _pFile) {};

public:
	CComponent();
	virtual ~CComponent();

	friend class CGameObject;
	friend class CTransform;
	friend class CCamera;
};

