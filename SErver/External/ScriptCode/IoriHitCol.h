#pragma once
#include "HitColScript.h"
class CIoriHitCol :
	public CHitColScript
{
private:
	float m_fTime;
	bool m_bDamaged;
	bool m_bBullet;
	bool m_bSound;
	CGameObject* m_pOtherObj;

public:
	CIoriHitCol();
	virtual ~CIoriHitCol();

public:
	virtual void Start();
	virtual void Update();

	void Collision_Update(CCollider* _pOther);
	void Collision_NoUpdate(CCollider* _pOther);

	void OnCollision_Update(CCollider* _pOther);
	void OnCollision_NoUpdate(CCollider* _pOther);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther) {}

public:
	void EffectOutput(Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt);
	void ChangeMtrl(CGameObject* _pOtherObj);
	void SoundOutput(const wstring& _strKey);
	void SetOtherObj(CGameObject* _pOtherObj) { m_pOtherObj = _pOtherObj; }
};

