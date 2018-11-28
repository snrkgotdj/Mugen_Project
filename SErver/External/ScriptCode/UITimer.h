#pragma once
#include "Script.h"

class CUITimer :
	public Script
{
private:
	CGameObject* m_pOneObj;
	CGameObject* m_pTenObj;
	bool m_bChanged;
	bool m_bFirstCall;
	float m_fTime;

	CMaterial* m_pMtrl;

public:
	CUITimer();
	virtual ~CUITimer();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUITimer);
};

