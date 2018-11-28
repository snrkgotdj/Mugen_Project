#pragma once
#include "Script.h"
#include "ColScript.h"
#include "..\Doom\define_client.h"

class CHitColScript :
	public CColScript
{
public:
	virtual void MoveLayer(bool _1P);

protected:
	void Damage(float _Damage);
	void GuardDamage(float _fDmg);
	void GuardRecover();

public:
	virtual void Update();
	virtual void FinalUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CLONE(CHitColScript);

public:
	CHitColScript();
	virtual ~CHitColScript();
};

