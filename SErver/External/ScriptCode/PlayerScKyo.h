#pragma once
#include "Script.h"
#include "CharactorScript.h"

class CPlayerScKyo :
	public CCharactorScript
{
public:
	CPlayerScKyo();
	virtual ~CPlayerScKyo();

private:
	float m_fTime;
	int m_iCmdLinkCnt;
	bool m_bEffect;
	bool m_bSound;
	int m_iBeforeIdx;
	bool m_bPracticeMode;
	CGameObject* m_pEffect;
	CSound* m_pSound;
	KYO_STATE m_eState;
	KYO_STATE m_ePreInput;
	KYO_STATE m_ePrevState;
	float m_fDashCooldown;
	float m_fCharge;

private:
	vector<KEY_TYPE> m_vecCmd;

private:
	void Update_Client();
	void Update_Server();

	void SendState();
	
	virtual void SetState(UINT _iState);

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();
	virtual void FinalUpdate();

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther) {};
	virtual void OnCollisionExit(CCollider* _pOther) {};

public:
	virtual void PushOtherPlayer(CGameObject* _pOther);
	KYO_STATE GetState() { return m_eState; }

private:
	void CommandInput();
	void CommandDecode();

private:
	int CommandDecode_Size4();
	int CommandDecode_Size3();
	int CommandDecode_Size2();
	int CommandDecode_Size1();
	int CommandDecode_Size0();

private:
	virtual void Gravity();
	

public:
	virtual void SoundOutput(const wstring& _strSound, int _iBefIdx);
	virtual void SoundOutput(const wstring& _strSound);
	virtual void SoundOutputAnimFin(const wstring& _strSound);

	PLAYER_KEY GetPlayerKey() { return m_tKey; }

private:
	void Idle();
	void FrontWalk();
	void BackWalk();
	void FrontDash();
	void BackDash();
	void Crouch();
	void JumpUp();
	void JumpDown();
	void JumpFront();
	void JumpBack();
	void Roll();
	void Guard();

	void GroundLP();
	void GroundSP();
	void GroundLK();
	void GroundSK();
	void Type75_Kai();
	void RedKicK();
	void Type100_Oniyaki();
	void TypeU108_Orochinagi();
	void Type114_Aragami();
	void Type115_Dokugami();
	void Type125_Nanase();
	void Type127_Yanosabi();
	void Type128_Konokizu();
	void Type182();
	void Type401_Tsumiyomi();
	void Type402_Batsuyomi();
	void Type427_Hikigane_L();
	void Type427_Hikigane_S();
	void OutType_Goufu();
	void OutType_Migiri();
	void NoType();

	void CrouchLP();
	void CrouchSP();
	void CrouchLK();
	void CrouchSK();
	void Type88();

	void JumpLP();
	void JumpSP();
	void JumpLK();
	void JumpSK();
	void OutType_Naraku();

	void HitDown();
	void HitMid();
	void HitUp();
	void HitAerial();
	void HitKnockdown();
	void StandUp();
	void StandUpQuick();

	CLONE(CPlayerScKyo);

};

