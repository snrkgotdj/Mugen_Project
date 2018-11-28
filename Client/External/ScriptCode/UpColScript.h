#pragma once
#include "Script.h"
#include "ColScript.h"
#include "..\Doom\define_client.h"

class CUpColScript :
	public CColScript
{
public:
	virtual void MoveLayer(bool _1P);

public:
	virtual void Update();
	virtual void FinalUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

	CLONE(CUpColScript);

public:
	CUpColScript();
	virtual ~CUpColScript();
};

