#pragma once
#include "Script.h"
class CColScript :
	public Script
{
protected:
	CGameObject* m_pPlayer;
	CAnimatorEX* m_pAnimator;
	CTransform* m_pTransForm;
	CCollider* m_pCollider;

	Vec3 m_vecPos;
	Vec3 m_vecScale;

	bool m_b1P;

public:
	void Set1P(bool _is1P) { m_b1P = _is1P; }

public:
	virtual void MoveLayer(bool _1P) {};

public:
	virtual void Awake();
	virtual void Update() {};
	virtual void FinalUpdate() {};

public:
	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	CLONE(CColScript);

public:
	CColScript();
	virtual ~CColScript();
};

