#include "Layer.h"

#include "GameObject.h"
#include "TransForm.h"
#include "SceneManager.h"
#include "Scene.h"

CLayer::CLayer()
	: m_iIdx(-1)
	, m_bZOrder(true)
	, m_fCamNear(-1.f)
	, m_fCamFar(1000.f)
	, m_fCamZoom(1.f)
{
}


CLayer::~CLayer()
{
	Safe_Delete_List(m_listObj);
}

void CLayer::Awake()
{
	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		(*iter)->Awake();
	}
}

void CLayer::Start()
{
	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		(*iter)->Start();
	}
}

void CLayer::Update()
{
	CheckDie();

	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		(*iter)->Update();
	}
}

void CLayer::LateUpdate()
{
	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		(*iter)->LateUpdate();
	}
}

void CLayer::FinalUpdate()
{
	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		(*iter)->FinalUpdate();
	}
}

void CLayer::Render()
{
	if (m_bZOrder == true)
	{
		m_listObj.sort([](CGameObject* _pLeft, CGameObject* _pRight)
		{
			return _pLeft->GetTransform()->GetWorldPos().z > _pRight->GetTransform()->GetWorldPos().z;
		});
	}

	list<CGameObject*>::iterator iter = m_listObj.begin();
	for (; iter != m_listObj.end(); ++iter)
	{
		Vec3 vIterPos = (*iter)->GetTransform()->GetWorldPos();
		//Vec3 vIterScale = (*iter)->GetTransform()->GetWorldScale();

		//float fLimitX = (WindowSizeX + abs(vIterScale.x)) / (2.f*m_fCamZoom);
		//float fLimitY = (WindowSizeY + abs(vIterScale.y)) / (2.f*m_fCamZoom);
		float fZ = vIterPos.z;

		if (fZ >= m_fCamNear&&fZ <= m_fCamFar)
		{
			(*iter)->Render();
		}
	}
}

void CLayer::AddGameObject(CGameObject * _pObj, bool _bAllChild)
{
	if (NULL != _pObj)
	{
		CSceneManager::Manager().SetObjectChange(true);
		CSceneManager::Manager().SetLayerChange(true);
	}

	list<CGameObject*> listSource;

	if (false == _pObj->HasParent() && m_strLayerName != _pObj->GetLayerName())
	{
		m_listParentObj.push_back(_pObj);
	}

	listSource.push_back(_pObj);

	if (true == _bAllChild)
	{
		list<CGameObject*> listChild;
		listChild.insert(listChild.end(), _pObj->GetChildList().begin(), _pObj->GetChildList().end());

		list<CGameObject*>::iterator iter = listChild.begin();
		for (; iter != listChild.end();)
		{
			if ((*iter)->GetLayerName() != m_strLayerName)
				listSource.push_back(*iter);

			listChild.insert(listChild.end(), (*iter)->GetChildList().begin(), (*iter)->GetChildList().end());
			iter = listChild.erase(iter);
		}
	}

	m_listObj.insert(m_listObj.end(), listSource.begin(), listSource.end());

	list<CGameObject*>::iterator iter = listSource.begin();
	for (; iter != listSource.end(); ++iter)
	{
		CLayer* pPrevLayer = CSceneManager::Manager().GetCurScene()->FindLayer((*iter)->GetLayerName());
		if (NULL != pPrevLayer)
			pPrevLayer->RemoveObj((*iter));

		(*iter)->SetLayerName(m_strLayerName);
	}
}

void CLayer::AddParent(CGameObject * _pObj)
{
	if (_pObj != NULL)
	{
		m_listParentObj.push_back(_pObj);
		CSceneManager::Manager().SetLayerChange(true);
	}
}

void CLayer::AddListOnly(CGameObject * _pObj)
{
	if (_pObj != NULL)
	{
		m_listObj.push_back(_pObj);
		CSceneManager::Manager().SetLayerChange(true);
	}
}

void CLayer::RemoveObj(CGameObject * _pObj)
{
	if (false == _pObj->HasParent())
	{
		list<CGameObject*>::iterator iter = m_listParentObj.begin();
		for (; iter != m_listParentObj.end(); ++iter)
		{
			if (*iter == _pObj)
			{
				m_listParentObj.erase(iter);
				break;
			}
		}
	}

	list<CGameObject*>::iterator iter = m_listObj.begin();
	for (; iter != m_listObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			m_listObj.erase(iter);
			break;
		}
	}
	CSceneManager::Manager().SetLayerChange(true);
}

void CLayer::RemoveParentObj(CGameObject * _pObj)
{
	list<CGameObject*>::iterator iter = m_listParentObj.begin();
	for (; iter != m_listParentObj.end(); ++iter)
	{
		if ((*iter) == _pObj)
		{
			m_listParentObj.erase(iter);
			return;
		}
	}
	CSceneManager::Manager().SetLayerChange(true);
}

void CLayer::RemoveListOnly(CGameObject * _pObj)
{
	list<CGameObject*>::iterator iter = m_listObj.begin();
	for (; iter != m_listObj.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			m_listObj.erase(iter);
			break;
		}
	}
	CSceneManager::Manager().SetLayerChange(true);
}

void CLayer::CheckDie()
{
	list<CGameObject*>::iterator iter = m_listObj.begin();

	for (; iter != m_listObj.end();)
	{
		if (true == (*iter)->isDead())
		{
			list<CGameObject*> listChildObj = (*iter)->GetChildList();
			list<CGameObject*>::iterator iterChild = listChildObj.begin();
			list<CGameObject*>::iterator iterChildEnd = listChildObj.end();

			for (iterChild; iterChild != iterChildEnd; ++iterChild)
			{
				(*iterChild)->Die();
				(*iterChild)->EndRelationship();
			}

			if ((*iter)->HasParent() == false)
			{
				list<CGameObject*>::iterator ParentIter = m_listParentObj.begin();
				for (; ParentIter != m_listParentObj.end(); ++ParentIter)
				{
					if ((*ParentIter) == (*iter))
					{
						m_listParentObj.erase(ParentIter);
						break;
					}
				}
			}
			SAFE_DELETE(*iter);
			iter = m_listObj.erase(iter);
		}

		else
		{
			++iter;
		}
	}
}

void CLayer::CheckOnCamRange(float _fNear, float _fFar, float _fZoom)
{
	m_fCamFar = _fFar;
	m_fCamNear = _fNear;
	m_fCamZoom = _fZoom;
}

CGameObject * CLayer::FindObjectByTag(const wstring & _strTag)
{
	list<CGameObject*>::iterator iter = m_listObj.begin();
	for (; iter != m_listObj.end(); ++iter)
	{
		if ((*iter)->GetTagName() == _strTag)
			return (*iter);
	}

	return NULL;
}

CGameObject * CLayer::FindObject(Vec2 _vWorldPos)
{
	list<CGameObject*> ::iterator iter = m_listObj.begin();
	for (; iter != m_listObj.end(); ++iter)
	{
		if ((*iter)->IsMouseOn(_vWorldPos))
			return (*iter);
	}

	return NULL;
}
