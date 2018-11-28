#pragma once
#include "Script.h"

//¾²Áö¸¶

class CTexture;

class CUIPanel :
	public Script
{
private:
	CTexture* m_pTex;
	bool m_bCall;

public:
	CUIPanel();
	virtual ~CUIPanel();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUIPanel);
};

