#pragma once
#include "Script.h"

class CGameObject;

class CShadowScript :
	public Script
{
private:
	CGameObject* m_pPlayer;
	float X;
	float Y;

public:
	void init(CGameObject* _pPlayer, float _X, float _Y);

public:
	virtual void Update();
	CLONE(CShadowScript)

public:
	CShadowScript();
	virtual ~CShadowScript();
};

