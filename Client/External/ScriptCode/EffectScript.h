#pragma once
#include "Script.h"
class CEffectScript :
	public Script
{
private:
	bool m_bActive;

private:
	bool m_bManager;
	bool m_bContinue;
	bool m_bMove;
	float m_fSpeed;

public:
	bool isActive() { return m_bActive; }
	void SetContinue(bool _true) { m_bContinue = _true; }
	void SetActive(bool _true) { m_bActive = _true; }
	void SetManaged(bool _false) { m_bManager = _false; }
	void SetMove(bool _bTrue) { m_bMove = _bTrue; }
	void SetSpeed(float _fSpeed) { m_fSpeed = _fSpeed; }

private:
	void ContinueEffect();
	void UnContinueEffect();

public:
	void SetManagerEffect(bool _true) { m_bManager = _true; }

public:
	virtual void Update();

public:
	CLONE(CEffectScript);

public:
	CEffectScript();
	virtual ~CEffectScript();
};

