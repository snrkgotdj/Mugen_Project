#pragma once
#include "HitColScript.h"
class CKyoHitCol :
	public CHitColScript
{
private:
	float m_fTime;
	float m_fCharge;
	float m_fEffectTime;
	bool m_bDamaged;
	bool m_bEffect;

private:
	CSound* m_pSound;

private:
	bool m_b1Player;

public:
	CKyoHitCol();
	virtual ~CKyoHitCol();

public:
	virtual void Update();

	void Collision_Update(CCollider* _pOther);
	void Collision_NoUpdate(CCollider* _pOther);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

public:
	void EffectOutput(Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt);
	void EffectOutput(const wstring& _strKey, Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange,
		Vec2 _vScaleRange, float _fScale, int _iCnt);
	void ChangeMtrl(CGameObject* _pOtherObj);
	void ChangeMtrl(CGameObject* _pOtherObj, float _fTime);
	void SoundOutput(const wstring& _strKey);
};

