#pragma once
#include "Res.h"

#include "GameObject.h"

class CPrefab :
	public CRes
{
private:
	CGameObject* m_pObj;

public:
	CGameObject* Instantiate() { return m_pObj->Clone(); }
	CGameObject* gameobject() { return m_pObj; }

public:
	CPrefab(CGameObject* _pObj);
	~CPrefab();
};

