#pragma once

#include "global.h"

class CScene;
class CGameObject;

class CSceneManager
{
private:
	CScene* m_pCurScene;
	CScene* m_pPrevScene;

	bool m_bObjChange;
	bool m_bLayerChnage;
	bool m_bIsPlayMode;

	CGameObject* m_pToolCam;

public:
	void SetPlayMode(bool _bTrue);
	void InitScene();

public:
	CScene* GetCurScene() { return m_pCurScene; }
	bool CheckObjectChange() { return m_bObjChange; }
	bool CheckLayerChange() { return m_bLayerChnage; }
	void SetObjectChange(bool _bTrue) { m_bObjChange = _bTrue; }
	void SetLayerChange(bool _bTrue) { m_bLayerChnage = _bTrue; }
	void ChangeScene(CScene* _pNewScene);
	void SetToolCam(CGameObject* _pCamObj) { m_pToolCam = _pCamObj; }

private:
	void CreateCamera();

private:
	void DeleteScene(CScene* _pScene);
	 
public:
	void init();
	void progress();
	void Render();

private:
	static CSceneManager* m_pInst;

public:
	static CSceneManager& Manager()
	{
		if (m_pInst == NULL)
		{
			m_pInst = new CSceneManager;
		}
		return (*m_pInst);
	}
	static void DestoryInst()
	{
		SAFE_DELETE(m_pInst);
	}

private:
	CSceneManager();
	~CSceneManager();
};

