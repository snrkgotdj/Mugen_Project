#pragma once
#include "Script.h"
class CUICmdList :
	public Script
{
private:
	wstring m_strName;
	int m_iMode;//1=특수 2,3=필살 4=초필
	bool m_bCalled;

public:
	CUICmdList();
	virtual ~CUICmdList();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUICmdList);

public:
	void SetName(const wstring& _str) { m_strName = _str; }
};

