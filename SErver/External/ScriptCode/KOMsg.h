#pragma once
#include "Script.h"
class CKOMsg :
	public Script
{
private:
	float m_fTime;

public:
	CKOMsg();
	virtual ~CKOMsg();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CKOMsg);
};

