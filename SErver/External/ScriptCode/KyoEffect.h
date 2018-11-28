#pragma once
#include "Script.h"
class CKyoEffect :
	public Script
{
public:
	CKyoEffect();
	virtual ~CKyoEffect();

private:
	bool m_b1Player;
	float m_fCharge;

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

	CLONE(CKyoEffect);
};

