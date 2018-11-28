#pragma once
#include "HitColScript.h"
class CNanayaHitCol :
	public CHitColScript
{
private:
	float m_fTime;
	bool m_bDamaged;
	CGameObject* m_pOtherPlayer;
	CSound* m_pSound;

public:
	virtual void Update();

	void Collision_Update(CCollider* _pOther);
	void Collision_NoUpdate(CCollider* _pOther);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);

public:
	CNanayaHitCol();
	virtual ~CNanayaHitCol();

private:
	void SoundOutput(const wstring& _strKey);
};

