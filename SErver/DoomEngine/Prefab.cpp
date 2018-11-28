#include "stdafx.h"
#include "Prefab.h"


CPrefab::CPrefab(CGameObject* _pObj)
	:m_pObj(_pObj)
{
}


CPrefab::~CPrefab()
{
	SAFE_DELETE(m_pObj);
}
