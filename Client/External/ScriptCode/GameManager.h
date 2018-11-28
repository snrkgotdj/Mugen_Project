#pragma once
#include "Single\SingletonScript.h"
class CGameManager :
	public CSingletonScript<CGameManager>
{
private:
	bool m_bActive2P;

private:
	CSound* m_pSound;

public:
	void init();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();

	CLONE(CGameManager);

public:
	bool GetActive2P() { return m_bActive2P; }
	void SetActive2P(bool _bTrue) { m_bActive2P = _bTrue; }
	CSound* GetSound() { return m_pSound; }
	
public:
	CGameManager();
	virtual ~CGameManager();
};

