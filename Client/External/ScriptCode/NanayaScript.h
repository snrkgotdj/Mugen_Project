#pragma once

#include "CharactorScript.h"

class CNanayaScript :
	public CCharactorScript
{
private:
	NANAYA_STATE m_eState;
	NANAYA_STATE m_ePrevState;

	float m_fWalkSpeed;
	float m_fAccTime;
	bool m_bSound;
	bool m_bEffect;

private:
	CSound* m_pSound;

private:
	void Update_Client();
	void Update_Server();
	void OnColisionEnter_Client(CCollider * _pOther);
	void OnColisionEnter_Server(CCollider * _pOther);

private:
	void SendState();
	void AdjustDir();

private:
	void Idle();
	void Walk();
	void Walk_Back();
	void Sit();
	void Attack_Light();
	void Attack_Midium();
	void Attack_Midium_Addition();
	void Attack_Power();
	void Attack_Sit_Z();
	void Attack_Sit_X();
	void Attack_Jump_Z();
	void Attack_Jump_X();
	void Attack_Jump_C();
	void Front_Dash();
	void Back_Dash();
	void Back_Dumbling();
	void Jump_Stay();
	void Jump_Front();
	void Jump_Back();
	void Jump_Front_Dash();
	void Jump_Back_Dash();
	void Hitted_Middle();
	void Hitted_Up();
	void Hitted_Down();
	void KnockDown();
	void StandUp();
	void PalJumChoong();
	void Chilya();
	void Palchun();
	void Yukto();
	void GuiksyNanaya();
	void GuiksyNanaya_Fail();
	void GuiksyNanaya_Hit();
	void Guard();

public:
	NANAYA_STATE GetState() { return m_eState; }
	void SetState(NANAYA_STATE _state) { m_eState = _state; }

protected:
	virtual void Gravity();
	virtual void SetDir(int _iDir);
	virtual void SetState(UINT _iState);
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	virtual void PushOtherPlayer(CGameObject* _pOther);

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

	CLONE(CNanayaScript);
public:
	CNanayaScript();
	virtual ~CNanayaScript();
};

