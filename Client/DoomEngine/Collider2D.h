#pragma once
#include "Collider.h"
class Collider2D :
	public CCollider
{
private:


public:
	virtual void Awake();
	virtual void Render();

	CLONE(Collider2D);

protected:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	virtual bool Is2DCollider() { return true; }
	virtual bool Is3DCollider() { return false; }

public:
	Collider2D();
	virtual ~Collider2D();
};

