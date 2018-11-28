#pragma once

#include "global.h"

#include "Component.h"

template<typename T>
struct ID
{
	typedef T type;
};

class CTransform;
class CMeshRender;
class CCamera;
class Script;
class CCollider;
class CAnimatorEX;

class CGameObject
{
private:
	CComponent* m_ArrComponent[(UINT)COMPONENT_TYPE::END];
	
private:
	wstring	m_strTag;
	wstring	m_strLayerName;
	CGameObject* m_pParentObj;

private:
	list<Script*> m_listScript;
	list<CGameObject*> m_listChildObj;

private:
	bool m_bIsDead;

public:
	void Die() { m_bIsDead = true; }
	bool isDead() { return m_bIsDead; }
	bool HasParent() { if (m_pParentObj != NULL) return true; else return false; }
	bool IsMouseOn(Vec2 _vWorldPos);

public:
	void SetTagName(const wstring& _strTag) { m_strTag = _strTag; }
	const wstring& GetTagName() { return m_strTag; }

public:
	void SetLayerName(const wstring& _strLayerName) { m_strLayerName = _strLayerName; }
	const wstring& GetLayerName() { return m_strLayerName; }

public:
	void AddChild(CGameObject* _pChildObj);
	CGameObject* GetParent() { return m_pParentObj; }
	list<CGameObject*>& GetChildList() { return m_listChildObj; }
	CGameObject* GetChildByName(const wstring& _strName);
	void EndRelationship();

private:
	void SetParentObj(CGameObject* _pParentObj) { m_pParentObj = _pParentObj; }
	bool IsSeniority(CGameObject* _pObj);

public:
	list<Script*>& GetScriptList() { return m_listScript; }
	void DeleteScript(int _iIdx);
	void DeleteCollider();
	void Delete();

public:
	CGameObject* Clone() { return new CGameObject(*this); }

public:
	template<typename T>
	void AddComponent(T* _pCom) { AddComponent(_pCom, ID<T>()); }

	template<typename T>
	void AddComponent(T* _pCom, ID<T>);
	void AddComponent(Script* _pCom, ID<Script>);

	template<typename T>
	CComponent* GetComponent();
	CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_ArrComponent[(UINT)_eType]; }

	CTransform* GetTransform();
	CMeshRender* GetMeshRender();
	CCamera* GetCamera();
	CCollider* GetCollider() { return (CCollider*)m_ArrComponent[(UINT)COMPONENT_TYPE::COLLIDER]; }
	CAnimatorEX* GetAnimatorEX() { return (CAnimatorEX*)m_ArrComponent[(UINT)COMPONENT_TYPE::ANIMATOR_EX]; }
	Script* GetFirstScript(); 
	

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

public:
	CGameObject();
	CGameObject(const CGameObject& _other);
	~CGameObject();

	friend class CLayer;
	friend class CSaveLoadManager;
};


////////////////////////////////////////

template<typename T>
inline void CGameObject::AddComponent(T* _pCom, ID<T>_id)
{
	const type_info& info = typeid(T);

	if (info.hash_code() == typeid(CTransform).hash_code())
	{
		if (m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM] != NULL)
			SAFE_DELETE(m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM]);

		m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM] = _pCom;
	}

	else if (info.hash_code() == typeid(CMeshRender).hash_code())
	{
		if (m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER] != NULL)
			SAFE_DELETE(m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER]);

		m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER] = _pCom;
	}

	else if (info.hash_code() == typeid(CCamera).hash_code())
	{
		m_ArrComponent[(UINT)COMPONENT_TYPE::CAMERA] = _pCom;
	}

	else if (info.hash_code() == typeid(CCollider).hash_code())
	{
		m_ArrComponent[(UINT)COMPONENT_TYPE::COLLIDER] = _pCom;
	}

	else if (info.hash_code() == typeid(CAnimatorEX).hash_code())
	{
		m_ArrComponent[(UINT)COMPONENT_TYPE::ANIMATOR_EX] = _pCom;
	}

	_pCom->SetGameObject(this);
}

template<typename T>
inline CComponent * CGameObject::GetComponent()
{
	const type_info& info = typeid(T);

	if (info.hash_code() == typeid(CTransform).hash_code())
	{
		return m_ArrComponent[(UINT)COMPONENT_TYPE::TRANSFORM];
	}
	else if (info.hash_code() == typeid(CMeshRender).hash_code())
	{
		return m_ArrComponent[(UINT)COMPONENT_TYPE::MESHRENDER];
	}
	else if (info.hash_code() == typeid(CCamera).hash_code())
	{
		return m_ArrComponent[(UINT)COMPONENT_TYPE::CAMERA];
	}
	else if (info.hash_code() == typeid(CCollider).hash_code())
	{
		return m_ArrComponent[(UINT)COMPONENT_TYPE::COLLIDER];
	}
	else if (info.hash_code() == typeid(CAnimatorEX).hash_code())
	{
		return m_ArrComponent[(UINT)COMPONENT_TYPE::ANIMATOR_EX];
	}

	else
	{
		list<Script*>::iterator iter = m_listScript.begin();
		for (; iter != m_listScript.end(); ++iter)
		{
			if (dynamic_cast<T*>(*iter))
				return(*iter);
		}
	}

	return NULL;
}
