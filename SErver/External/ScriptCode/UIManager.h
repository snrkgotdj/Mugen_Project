#pragma once
#include "Single\SingletonScript.h"
class CUIManager :
	public CSingletonScript<CUIManager>
{

private:
	CGameObject* m_pUICamera;
	CGameObject* m_pUI_HP_1P;
	CGameObject* m_pUI_HP_2P;

	CGameObject* m_pUI_1P_ChildHP;
	CGameObject* m_pUI_2P_ChildHP;
	CGameObject* m_pUI_1P_ChildGuard;
	CGameObject* m_pUI_2P_ChildGuard;

	CGameObject* m_pComboOutput_1P;
	CGameObject* m_pComboOutput_2P;
	CGameObject* m_pGameMsg;
	CGameObject* m_pCmdOutput;

	CGameObject* m_pCmdList;

private:
	wstring m_strUI_HP_1P;
	wstring m_strUI_HP_2P;
	wstring m_strCombo;
	wstring m_strGameMsg;

	bool m_bEnd;
	bool m_bTraining;
	char m_cPlayer;

public:
	CGameObject* Get1PHP() { return m_pUI_HP_1P; }
	CGameObject* Get2PHP() { return m_pUI_HP_2P; }
	CGameObject* GetCombo1P() { return m_pComboOutput_1P; }
	CGameObject* GetCombo2P() { return m_pComboOutput_2P; }
	CGameObject* GetGameMsg() { return m_pGameMsg; }
	CGameObject* GetCmdOutput() { return m_pCmdOutput; }

public:
	void SetHP_1P(float _fHP);
	void SetHP_2P(float _fHP);
	void SetPlayer(char _cPlayer) { m_cPlayer = _cPlayer; }

private:
	void CreateUICam();

public:
	void SetTraining(bool _bTrue) { m_bTraining = _bTrue; }
	bool GetTraining() { return m_bTraining; }
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();

	CLONE(CUIManager);

public:
	CUIManager();
	virtual ~CUIManager();
};

