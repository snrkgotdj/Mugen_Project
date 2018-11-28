#pragma once
#include "CharactorScript.h"
class CNanayaTestScript :
	public CCharactorScript
{
private:
	NANAYA_STATE m_eState;

	float m_fWalkSpeed;

private:
	void Idle();
	void Hitted_Middle();
	void Hitted_Up();
	void Hitted_Down();
	void KnockDown();
	void StandUp();

protected:
	virtual void Gravity();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	CLONE(CNanayaTestScript);

public:
	CNanayaTestScript();
	virtual ~CNanayaTestScript();
};

