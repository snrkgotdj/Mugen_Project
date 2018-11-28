#pragma once

#include "global.h"

class CGameObject;

class CLayer
{
	friend class CScene;

private:
	wstring m_strLayerName;
	list<CGameObject*> m_listObj;
	list<CGameObject*> m_listParentObj;

private:
	int	m_iIdx;
	bool m_bZOrder;
	float m_fCamNear;
	float m_fCamFar;
	float m_fCamZoom;

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();

public:
	void AddGameObject(CGameObject* _pObj, bool _bAllChild = true);
	void AddParent(CGameObject* _pObj);
	void AddListOnly(CGameObject* _pObj);

private:
	void SetZOrder(bool _bZOrder) { m_bZOrder = _bZOrder; }
	void SetLayerIdx(UINT _iIdx) { m_iIdx = _iIdx; }
	void SetLayerName(const wstring& _strLayerName) { m_strLayerName = _strLayerName; }

public:
	list<CGameObject*>& GetObjList() { return m_listObj; }
	list<CGameObject*>& GetParentList() { return m_listParentObj; }
	UINT GetLayerIdx() { return m_iIdx; }
	const wstring& GetLayerName() { return m_strLayerName; }
	CGameObject* FindObjectByTag(const wstring& _strTag);
	CGameObject* FindObject(Vec2 _vWorldPos);
	void CheckOnCamRange(float _fNear, float _fFar, float _fZoom);

private:
	void RemoveObj(CGameObject* _pObj);
	void RemoveParentObj(CGameObject* _pObj);
	void RemoveListOnly(CGameObject* _pObj);
	void CheckDie();
	
public:
	CLayer();
	~CLayer();

	friend class CScene;
	friend class CGameObject;
	friend class CSaveLoadManager;
	friend class CChangeLayerDlg;
	friend class CUpColScript;
	friend class CDownColScript;
	friend class CHitColScript;
};

