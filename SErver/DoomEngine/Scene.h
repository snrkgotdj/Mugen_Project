#pragma once

#include "global.h"

class CLayer;
class CGameObject;

class CScene
{
private:
	map<wstring, CLayer*>	m_mapLayer;
	vector<CLayer*>			m_vecLayer;
	set<UINT>				m_setIdx;
	
	CGameObject* m_pMainCamera;
	CGameObject* m_pUICamera;

public:
	void Awake();
	void Start();
	void Update();
	void LateUpdate();
	void FinalUpdate();
	void Render();
	void Render(CGameObject* _pCamObj);

public:
	int AddLayer(const wstring& _strLayerName);
	int AddLayer(CLayer* _pLayer, UINT _iIdx = -1);
	CLayer* FindLayer(const wstring& _strLayerName);
	int GetLayerCount() const { return m_mapLayer.size(); }

public:
	CGameObject* FindObjec(Vec2 _vMousePos, Vec2 _vResolution);
	void GetAllParentObject(list<CGameObject*>& _listOut);

public:
	void SetMainCamera(CGameObject* _pMainCam) { m_pMainCamera = _pMainCam; }
	CGameObject* GetMainCamera() { return m_pMainCamera; }
	CGameObject* GetUICamera() { return m_pUICamera; }
	Vec2 MtoW(Vec2 _vMousePos, Vec2 _vResolution);

private:
	vector<CLayer*>& GetVecLayer() { return m_vecLayer; }
	void DeleteLayer(const wstring& _strLayerName);
	void DeleteIdx(UINT _iIdx);

public:
	CScene();
	~CScene();

	friend class CCollisionManager;
	friend class CSaveLoadManager;
	friend class CMainFrame;
	friend class CChangeLayerDlg;
	friend class CCheckColLayerDlg;
	friend class CLayerDialog;
	friend class CShootLayerDlg;
};

