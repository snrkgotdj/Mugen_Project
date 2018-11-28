#pragma once
#include "Script.h"
class CKyoLoad :
	public Script
{
private:
	int m_iRandAct;

public:
	CKyoLoad();
	virtual ~CKyoLoad();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

	CLONE(CKyoLoad);
};

