#pragma once
#include "Script.h"
class CUIMatchWait :
	public Script
{
private:
	float m_fTerm;

public:
	CUIMatchWait();
	virtual ~CUIMatchWait();

public:
	virtual void Update();

	CLONE(CUIMatchWait);
};

