#pragma once
#include "HitColScript.h"
class CIoriBullet :
	public CHitColScript
{
private:
	float m_fTime;
	bool m_bHit;

public:
	CIoriBullet();
	virtual ~CIoriBullet();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);

	CLONE(CIoriBullet);

public:
	void ResetTime() { m_fTime = 0.f; }
	void ChangeMtrl(CGameObject* _pOtherObj);
	void EffectOutput(Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt);
};

