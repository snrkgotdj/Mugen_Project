#pragma once
#include "Script.h"
#include "CharactorScript.h"
#include "..\Doom\define_client.h"

class CPlayerScIori :
	public CCharactorScript
{
public:
	CPlayerScIori();
	virtual ~CPlayerScIori();

private:
	float m_fTime;
	int m_iCmdLinkCnt;
	bool m_bEffect;
	bool m_bSound;
	float m_fFloorY;
	float m_fDashCooldown;
	CGameObject* m_pEffect;
	CSound* m_pSound;

	UINT m_iSkillNum;//잡기 관련 변수   1:금월 2:설풍
	bool m_bPracticeMode;
	IORI_STATE m_eState;
	IORI_STATE m_ePrevState;


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
	

public:
	virtual void SoundOutput(const wstring& _strSound, int _iBefIdx);
	virtual void SoundOutput(const wstring& _strSound);
	virtual void SoundOutputAnimFin(const wstring& _strSound);

public:
	IORI_STATE GetState() { return m_eState; }
	void SetState(IORI_STATE _eState) { m_eState = _eState; }

public:
	CSound* GetSound() { return m_pSound; }

private:
	void CommandInput();
	void CommandDecode();

private:
	int CommandDecode_Size8();
	int CommandDecode_Size6();
	int CommandDecode_Size5();
	int CommandDecode_Size4();
	int CommandDecode_Size3();
	int CommandDecode_Size2();
	int CommandDecode_Size1();
	int CommandDecode_Size0();

	CLONE(CPlayerScIori);

private:
	virtual void Gravity();

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
	void OutType_Yumebiki_A();
	void OutType_Yumebiki_B();
	void OutType_Goufu();
	void Type100_Oniyaki();
	void Type108_Yamibarai();
	void Type127_Aoihana_A();
	void Type127_Aoihana_B();
	void Type127_Aoihana_C();
	void Type212_Kototsuki();
	void Type212_Kototsuki_Fail();
	void Type212_Kototsuki_Success();
	void Kuzukaze();
	void KuzukazeFail();
	void KuzukazeSuccess();
	void KType1211_Yaotome_Start();
	void KType1211_Yaotome();
	void UType316_Saika();
	void UType1029_Homurabotogi_Start();
	void UType1029_Homurabotogi();

	void CrouchLP();
	void CrouchSP();
	void CrouchLK();
	void CrouchSK();

	void JumpLP();
	void JumpSP();
	void JumpLK();
	void JumpSK();
	void OutType_Yuriori();

	void HitDown();
	void HitMid();
	void HitUp();
	void HitAerial();
	void HitKnockdown();
	void StandUp();
	void StandUpQuick();
};

