#include "GameObject.h"

#include "MeshRender.h"
#include "Layer.h"
#include "Script.h"

CGameObject::CGameObject()
	:m_ArrComponent{}
	, m_bIsDead(false)
	, m_pParentObj(NULL)
{
	AddComponent(new CTransform);
}

CGameObject::CGameObject(const CGameObject & _other)
	:m_strTag(_other.m_strTag)
	, m_ArrComponent{}
	, m_pParentObj(NULL)
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL == _other.m_ArrComponent[i])
			continue;

		m_ArrComponent[i] = _other.m_ArrComponent[i]->Clone();
		m_ArrComponent[i]->SetGameObject(this);
	}

	list<Script*>::const_iterator iter = _other.m_listScript.begin();
	for (; iter != _other.m_listScript.end(); ++iter)
	{
		Script* pCloneScript = (*iter)->Clone();
		pCloneScript->SetGameObject(this);
		m_listScript.push_back(pCloneScript);
	}

	list<CGameObject*>::const_iterator iterObj = _other.m_listChildObj.begin();
	for (; iterObj != _other.m_listChildObj.end(); ++iterObj)
	{
		AddChild((*iterObj)->Clone());
	}
}

CGameObject::~CGameObject()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		SAFE_DELETE(m_ArrComponent[i]);
	}

	Safe_Delete_List(m_listScript);
}

CGameObject * CGameObject::GetChildByName(const wstring & _strTag)
{
	list<CGameObject*>::iterator iter = m_listChildObj.begin();
	list<CGameObject*>::iterator iterEnd = m_listChildObj.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTagName() == _strTag)
		{
			return (*iter);
		}
	}

	return NULL;
}

void CGameObject::Awake()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_ArrComponent[i] != NULL)
		{
			m_ArrComponent[i]->Awake();
		}
	}
	list<Script*>::iterator iter = m_listScript.begin();
	for (; iter != m_listScript.end(); ++iter)
	{
		if((*iter) != NULL)
			(*iter)->Awake();
	}

	list<CGameObject*>::iterator iterChild = m_listChildObj.begin();
	for (; iterChild != m_listChildObj.end(); ++iterChild)
	{
		(*iterChild)->Awake();
	}
}

void CGameObject::Start()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_ArrComponent[i] != NULL)
		{
			m_ArrComponent[i]->Start();
		}
	}

	list<Script*>::iterator iter = m_listScript.begin();
	for (; iter != m_listScript.end(); ++iter)
	{
		if ((*iter) != NULL)
			(*iter)->Start();
	}

	list<CGameObject*>::iterator iterChild = m_listChildObj.begin();
	for (; iterChild != m_listChildObj.end(); ++iterChild)
	{
		(*iterChild)->Start();
	}
}

void CGameObject::Update()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_ArrComponent[i] != NULL)
		{
			m_ArrComponent[i]->Update();
		}
	}
	list<Script*>::iterator iter = m_listScript.begin();
	for (; iter != m_listScript.end(); ++iter)
	{
		if ((*iter) != NULL)
			(*iter)->Update();
	}

	list<CGameObject*>::iterator iterChild = m_listChildObj.begin();
	for (; iterChild != m_listChildObj.end(); ++iterChild)
	{
		(*iterChild)->Update();
	}
}

void CGameObject::LateUpdate()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_ArrComponent[i] != NULL)
		{
			m_ArrComponent[i]->LateUpdate();
		}
	}
	list<Script*>::iterator iter = m_listScript.begin();
	for (; iter != m_listScript.end(); ++iter)
	{
		if ((*iter) != NULL)
			(*iter)->LateUpdate();
	}

	list<CGameObject*>::iterator iterChild = m_listChildObj.begin();
	for (; iterChild != m_listChildObj.end(); ++iterChild)
	{
		(*iterChild)->LateUpdate();
	}
}

void CGameObject::FinalUpdate()
{
	for (int i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_ArrComponent[i] != NULL)
		{
			m_ArrComponent[i]->FinalUpdate();
		}
	}

	list<Script*>::iterator iter = m_listScript.begin();
	for (; iter != m_listScript.end(); ++iter)
	{
		if ((*iter) != NULL)
			(*iter)->FinalUpdate();
	}

	list<CGameObject*>::iterator iterChild = m_listChildObj.begin();
	for (; iterChild != m_listChildObj.end(); ++iterChild)
	{
		(*iterChild)->FinalUpdate();
	}
}

void CGameObject::Render()
{
	if (GetMeshRender() != NULL)
	{
		//float fZ = GetTransform()->GetLocalPos().z;
		//
		//if(fZ>=-1.f&&fZ<=1000.f)
			GetMeshRender()->Render();
	}
}

CTransform * CGameObject::GetTransform()
{
	if ((CTransform*)m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM] != NULL)
	{
		return (CTransform*)m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM];
	}
	else if ((CTransform*)m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM] == NULL)
	{
		return NULL;
	}
	return NULL;
}

CMeshRender * CGameObject::GetMeshRender()
{
	if (m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER] != NULL)
	{
		return (CMeshRender*)m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER];
	}
	
	else if (m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER] == NULL)
	{
		return NULL;
	}
	return NULL;
}

CCamera * CGameObject::GetCamera()
{
	if (m_ArrComponent[(UINT)COMPONENT_TYPE::CAMERA] != NULL)
	{
		return (CCamera*)m_ArrComponent[(UINT)COMPONENT_TYPE::CAMERA];
	}
		
	else  if (m_ArrComponent[(UINT)COMPONENT_TYPE::CAMERA] == NULL)
	{
		return NULL;
	}
	return NULL;
}

Script * CGameObject::GetFirstScript()
{
	if (m_listScript.size() == 0)
		return NULL;

	else
		return (Script*)(*m_listScript.begin());
}

void CGameObject::AddChild(CGameObject * _pChildObj)
{
	if (IsSeniority(_pChildObj))
		return;

	_pChildObj->EndRelationship();
	_pChildObj->SetParentObj(this);
	m_listChildObj.push_back(_pChildObj);
}

void CGameObject::EndRelationship()
{
	if (NULL != m_pParentObj)
	{
		list<CGameObject*>& listChild = m_pParentObj->GetChildList();

		list<CGameObject*>::iterator iter = listChild.begin();

		for (iter; iter != listChild.end(); ++iter)
		{
			if (this == (*iter))
			{
				listChild.erase(iter);
				break;
			}
		}
		m_pParentObj = NULL;
	}

	else
	{
		if (m_strLayerName.empty())
			return;

		CLayer* pCurLayer = CSceneManager::Manager().GetCurScene()->FindLayer(m_strLayerName);
		if (NULL != pCurLayer)
			pCurLayer->RemoveParentObj(this);
	}
}

bool CGameObject::IsSeniority(CGameObject * _pObj)
{
	if (this == _pObj)
		return true;

	if (NULL == m_pParentObj)
		return false;

	return m_pParentObj->IsSeniority(_pObj);

}


void CGameObject::AddComponent(Script * _pCom, ID<Script>)
{
	m_listScript.push_back(_pCom);
	_pCom->SetGameObject(this);
}

bool CGameObject::IsMouseOn(Vec2 _vWorldPos)
{
	Vec3 vWorldPos = GetTransform()->GetWorldPos();
	Vec3 vWorldScale = GetTransform()->GetWorldScale();

	if (vWorldPos.x - vWorldScale.x / 2.f <= _vWorldPos.x && _vWorldPos.x <= vWorldPos.x + vWorldScale.x / 2.f
		&& vWorldPos.y - vWorldScale.y / 2.f <= _vWorldPos.y && _vWorldPos.y <= vWorldPos.y + vWorldScale.y / 2.f)
		return true;

	return false;
}

void CGameObject::DeleteScript(int _iIdx)
{
	list<Script*>::iterator iter = m_listScript.begin();

	for (int i = 0; i < _iIdx; ++i)
	{
		++iter;
	}

	delete(*iter);
	m_listScript.erase(iter);
}

void CGameObject::DeleteCollider()
{
	SAFE_DELETE(m_ArrComponent[(UINT)COMPONENT_TYPE::COLLIDER]);

	m_ArrComponent[(UINT)COMPONENT_TYPE::COLLIDER] = NULL;
}

void CGameObject::Delete()
{
	if (m_pParentObj != NULL)
	{
		list<CGameObject*>& ChildList = m_pParentObj->GetChildList();

		list<CGameObject*>::iterator iter = ChildList.begin();

		for (; iter != ChildList.end(); ++iter)
		{
			if ((*iter) == m_pParentObj->GetChildByName(m_strTag))
			{
				ChildList.erase(iter);
				break;
			}
		}
	}

	Die();
}