#pragma once
#include "Script.h"
#include "KeyCommand.h"
#include "..\Doom\define_client.h"

class CAnimatorEX;

class CCharactorScript :
	public Script
{
protected:
	bool m_b1Player;
	bool m_bOnline;
	PLAYER_KEY m_tKey;

protected:
	CGameObject* m_pEffect;
	CGameObject* m_pShadow;
	CSound* m_pSound;

protected:
	HIT_STATE m_eHitState;
	CKeyCommand* m_pKeyCommand;
	CAnimatorEX* m_pAnimator;
	CTransform* m_pTransForm;
	CGameObject* m_pOpponent;

protected:
	Vec3 m_vecPos;
	Vec3 m_vecPrevPos;
	Vec3 m_vecScale;
	int m_iDir;
	bool m_bGuard;
	bool m_bCrouch;

	int m_iBeforeIdx;
	float m_fFloorY;
	bool m_bEffected;
	float m_fAccEffect;
	float m_fEffectTimeLimit;

protected:
	float m_fGravity;
	float m_fJumpSpeed;
	float m_fKnockback;
	float m_fGravAccel;
	bool m_bOnGround;
	float m_fStunAttTime;	//상대에게 주는 스턴 시간
	float m_fStunHitTime;		//공격받을 때 스턴
	float m_fStunTime;

	bool m_bCatch;
	bool m_bDefeated;
	bool m_bStop;
	bool m_bSound;

protected:
	void CreateShadow();
	void EffectedTime();
	void SendPos();

public:
	CGameObject* GetEffect() const { return m_pEffect; }
	HIT_STATE GetHitState() const { return m_eHitState; }
	int GetDir() const { return m_iDir; }
	bool is1P() const { return m_b1Player; }

	void SetOnGround(bool _false) { m_bOnGround = false; }
	void SetJumpSpeed(float _speed) { m_fJumpSpeed = _speed; }
	void SetEffected(bool _true) { m_bEffected = _true; }
	void SetEffected(bool _bTrue, float _fTime);
	void SetDefeated(bool _bTrue) { m_bDefeated = _bTrue; }
	void SetKey(const PLAYER_KEY& _Key) { m_tKey = _Key; }
	void Set1P();
	void Set2P();
	void Online(bool _true) { m_bOnline = _true; }
public:
	void PushPlayerX(float _distance);

public:
	float GetStunAttTime() { return m_fStunAttTime; }
	void SetStunAttTime(float _fTime) { m_fStunAttTime = _fTime; }
	float GetStunHitTime() { return m_fStunHitTime; }
	void SetStunHitTime(float _fTime) { m_fStunTime = 0.f; m_fStunHitTime = _fTime; }
	float GetStunTime() { return m_fStunTime; }
	void SetStunTime(float _fTime) { m_fStunTime = _fTime; }
	float GetKnockback() { return m_fKnockback; }
	void SetKnockback(float _fDist) { m_fKnockback = _fDist; }
	
	bool GetOnGround() { return m_bOnGround; }

	float GetEffectTimeLimit() { return m_fEffectTimeLimit; }
	void SetEffectTimeLimit(float _fTime) { m_fEffectTimeLimit = _fTime; }

	void SetStop(bool _bTrue) { m_bStop = _bTrue; }

	float GetFloorY() { return m_fFloorY; }
	bool IsGuard() { return m_bGuard; }
	bool IsCrouch() { return m_bCrouch; }

protected:
	virtual void Gravity();
public:
	virtual void SetDir(int _iDir) { if (m_bOnGround == true) m_iDir = _iDir; }
	virtual void SetState(UINT _iState);
	void SetPos(const Vec3& _vecPos);
	void SetOnlineKey();
protected:
	void SendKey();

public:
	virtual void SoundOutput(const wstring& _strSound, int _iBefIdx);
	virtual void SoundOutput(const wstring& _strSound);
	virtual void SoundOutputAnimFin(const wstring& _strSound);

public:
	virtual void Awake();
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
	virtual void FinalUpdate();

public:
	virtual void PushOtherPlayer(CGameObject* _pOther) {};

	virtual void OnCollisionEnter(CCollider* _pOther) {};
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	CLONE(CCharactorScript);

public:
	CCharactorScript();
	~CCharactorScript();
};

