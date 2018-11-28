#pragma once
#include "Script.h"
#include "ColScript.h"
#include "..\Doom\define_client.h"

class CDownColScript :
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

	CLONE(CDownColScript);

public:
	CDownColScript();
	virtual ~CDownColScript();
};

