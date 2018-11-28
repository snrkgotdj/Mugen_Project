#pragma once
#include "Script.h"
class CUICombo :
	public Script
{
private:
	int m_iVal;
	float m_fTime;

public:
	CUICombo();
	virtual ~CUICombo();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUICombo);

public:
	void SetValue(int _iVal) { m_iVal = _iVal; m_fTime = 0.f; }
	void AddValue();
};

