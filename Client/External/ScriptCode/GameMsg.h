#pragma once
#include "Script.h"
class CGameMsg :
	public Script
{
private:
	int m_iState;
	float m_fTime;

	float m_fKOTime;

public:
	CGameMsg();
	virtual ~CGameMsg();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CGameMsg);

public:
	void SetState(int _iState);
	int GetState() { return m_iState; }
	
	float GetKOTime() { return m_fKOTime; }
};

