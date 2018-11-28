#pragma once
#include "CharactorScript.h"

class CTestScKyo :
	public CCharactorScript
{
private:
	float m_fTime;
	bool m_bEffect;
	bool m_bSound;
	int m_iBeforeIdx;

public:
	CTestScKyo();
	virtual ~CTestScKyo();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

private:
	void Aerial();
	void ClipEnd();
	void Gravity();
};

