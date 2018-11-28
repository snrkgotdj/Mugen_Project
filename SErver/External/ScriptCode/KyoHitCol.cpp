#include "stdafx.h"
#include "KyoHitCol.h"

#include "Collider.h"

#include "GameObject.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "UIBar.h"
#include "UICombo.h"

#include "CharactorScript.h"
#include "PlayerScKyo.h"
#include "EffectManager.h"
#include "UIManager.h"
#include "PlayerManager.h"

CKyoHitCol::CKyoHitCol()
	:m_fTime(0.f)
	, m_fCharge(0.f)
	, m_fEffectTime(0.f)
	, m_bDamaged(false)
	, m_bEffect(false)
	, m_pSound(NULL)
{
}


CKyoHitCol::~CKyoHitCol()
{
}

void CKyoHitCol::Update()
{
	m_b1Player = ((CCharactorScript*)(GetGameObject()->GetParent()->GetFirstScript()))->is1P();

	m_fTime += DT;

	GuardRecover();

	KYO_STATE eState = ((CPlayerScKyo*)GetParentObject()->GetFirstScript())->GetState();
	PLAYER_KEY tKey = ((CPlayerScKyo*)GetParentObject()->GetFirstScript())->GetPlayerKey();
	Vec3 vPos = GetParentObject()->GetTransform()->GetLocalPos();
	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (eState == KYO_TYPE100_ONIYAKI||eState==KYO_REDKICK||eState==KYO_OUTTYPE_NARAKU)
	{
		if (m_fTime >= 0.5f)
			m_bDamaged = false;
	}
	else if (eState == KYO_NOTYPE)
	{
		m_fEffectTime += DT;

		if (iCurFrame > 40&&iCurFrame<47)
		{
			if (m_fTime >=1.f)
				m_bDamaged = false;
		}
		else if (iCurFrame >=47)
		{
			if (m_fEffectTime >= 0.005f)
			{
				m_fEffectTime = 0.f;
				EffectOutput(L"Explosion_04", vPos, Vec2(-WindowSizeX / 2.f, WindowSizeX / 2.f)
					, Vec2(-WindowSizeY / 2.f, WindowSizeY / 2.f), Vec2(150.f, 200.f), 1.157f, 1);
			}
		}
		else
		{
			if (m_fTime >= 0.2f)
				m_bDamaged = false;
		}
	}
	else
	{
		m_fEffectTime = 0.f;

		if (m_fTime >= 0.2f)
			m_bDamaged = false;
	}

	if (eState == KYO_TYPE182||eState==KYO_UTYPE108_OROCHINAGI)
	{
		if (KEYPUSH(tKey.Z) || KEYPUSH(tKey.X))
		{
			if(m_fCharge<=2.f)
				m_fCharge += DT;
			else
			{
				m_fCharge = 4.f;
				if (false == m_bEffect)
				{
					SoundOutput(L"common_00049.wav");
					CEffectManager::GetInst()->ActiveEffect(L"SuperSpark2", vPos);
					m_bEffect = true;
				}
			}
		}
	}
	else
	{
		m_fCharge = 0.f;
		m_bEffect = false;
	}
}

void CKyoHitCol::Collision_Update(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)pOtherPlayer->GetFirstScript();
	CCharactorScript* pPlayerScript = (CCharactorScript*)GetParentObject()->GetFirstScript();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();
	Vec3 vOtherScale = pOtherPlayer->GetTransform()->GetLocalScale();
	int iDir = ((CCharactorScript*)GetParentObject()->GetFirstScript())->GetDir();
	int iCurFrame = m_pPlayer->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	KYO_STATE eState = ((CPlayerScKyo*)GetParentObject()->GetFirstScript())->GetState();
	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

	wstring strClipName = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetKey();

	((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.1f);

	if (true == bOtherGuard)
	{
		if (bOtherCrouch)
		{
			((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->PushPlayerX(10.f);
			GuardDamage(5.f);
			return;
		}
		else
		{
			if (eState != KYO_CROUCH_LK &&eState != KYO_CROUCH_SK &&
				eState != KYO_CROUCH_LP)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				pOtherPlayerScript->PushPlayerX(10.f);
				GuardDamage(5.f);
				return;
			}
		}
	}

	////TODO : 클립 조건 추가

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case KYO_LP:
			Damage(2.5f);
			//Damage(100.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.1f);
			pOtherPlayerScript->SetKnockback(150.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_SP:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_LK:
			Damage(1.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.1f);
			pOtherPlayerScript->SetKnockback(150.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_SK:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE75_KAI:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(450.f);
			SoundOutput(L"common_00005.wav");
			if (iCurFrame > 6)
			{
				pOtherPlayerScript->SetOnGround(false);
				pOtherPlayerScript->SetJumpSpeed(1000.f);
			}
			break;
		case	KYO_REDKICK:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE100_ONIYAKI:
			Damage(9.f);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(1300.f);
			pOtherPlayerScript->SetOnGround(false);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-50.f, 50.f), Vec2(25.f, 50.f), 5);
			ChangeMtrl(pOtherPlayer);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayerScript->SetJumpSpeed(1500.f);
			SoundOutput(L"common_00013.wav");
			break;
		case	KYO_UTYPE108_OROCHINAGI:
			Damage(10.f + 3.f*m_fCharge);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(250.f + m_fCharge*75.f);
			pOtherPlayerScript->SetJumpSpeed(1000.f);
			pOtherPlayerScript->SetOnGround(false);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5 + (int)(m_fCharge * 2));
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00017.wav");
			break;
		case	KYO_TYPE114_ARAGAMI:
			Damage(4.f);
			pOtherPlayerScript->SetKnockback(200.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE115_DOKUGAMI:
			Damage(5.f);
			pOtherPlayerScript->SetKnockback(300.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 5);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE125_NANASE:
			Damage(6.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(550.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE127_YANOSABI:
			Damage(6.f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE128_KONOKIZU:
			Damage(4.5f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE182:
			Damage(20.f + m_fCharge*6.f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(500.f + 500.f*m_fCharge*0.6f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5 + (int)(m_fCharge*3.f));
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00009.wav");
			break;
		case	KYO_TYPE401_TSUMIYOMI:
			Damage(3.5f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE402_BATSUYOMI:
			Damage(4.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(550.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE427_HIKIGANE_L:
			Damage(5.f);
			pOtherPlayerScript->SetKnockback(450.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE427_HIKIGANE_S:
			Damage(8.f);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayerScript->SetJumpSpeed(1000.f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(0.f, 200.f), Vec2(50.f, 200.f), 7);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_OUTTYPE_GOUFU:
			Damage(7.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_OUTTYPE_MIGIRI:
			Damage(7.5f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_NOTYPE:
			if (iCurFrame <= 9)
			{
				Damage(5.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 16 && iCurFrame > 9)
			{
				Damage(4.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 23 && iCurFrame > 16)
			{
				Damage(4.5f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 30 && iCurFrame > 23)
			{
				Damage(6.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 36 && iCurFrame > 30)
			{
				Damage(4.f);
				SoundOutput(L"common_00005.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 40 && iCurFrame > 36)
			{
				Damage(4.5f);
				SoundOutput(L"common_00005.wav");
				pOtherPlayerScript->SetStunHitTime(2.f);
				pOtherPlayerScript->SetKnockback(0.f);
			}
			else
			{
				SoundOutput(L"common_00016.wav");
				Damage(20.f);
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetOnGround(false);
				pOtherPlayerScript->SetJumpSpeed(1600.f);
				pOtherPlayerScript->SetKnockback(150.f);


			}

			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 3);

			ChangeMtrl(pOtherPlayer, 2.f);
			break;
		case	KYO_CROUCH_LP:
			Damage(2.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_CROUCH_SP:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_CROUCH_LK:
			Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_CROUCH_SK:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(800.f);
			pOtherPlayerScript->SetOnGround(false);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE88:
			Damage(5.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_LP:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_SP:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_JUMP_LK:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_SK:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_OUTTYPE_NARAKU:
			Damage(5.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		}
	}

	//CEffectManager::GetInst()->ActiveEffect(L"Spark03", vOtherPos);
}

void CKyoHitCol::Collision_NoUpdate(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)pOtherPlayer->GetFirstScript();
	CCharactorScript* pPlayerScript = (CCharactorScript*)GetParentObject()->GetFirstScript();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();
	Vec3 vOtherScale = pOtherPlayer->GetTransform()->GetLocalScale();
	int iDir = ((CCharactorScript*)GetParentObject()->GetFirstScript())->GetDir();
	int iCurFrame = m_pPlayer->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	KYO_STATE eState = ((CPlayerScKyo*)GetParentObject()->GetFirstScript())->GetState();
	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

	wstring strClipName = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetKey();

	((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.1f);

	if (true == bOtherGuard)
	{
		if (bOtherCrouch)
		{
			((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->PushPlayerX(10.f);
			//GuardDamage(5.f);
			return;
		}
		else
		{
			if (eState != KYO_CROUCH_LK &&eState != KYO_CROUCH_SK &&
				eState != KYO_CROUCH_LP)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				pOtherPlayerScript->PushPlayerX(10.f);
				//GuardDamage(5.f);
				return;
			}
		}
	}

	////TODO : 클립 조건 추가

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case KYO_LP:
			//Damage(2.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.1f);
			pOtherPlayerScript->SetKnockback(150.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_SP:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_LK:
			//Damage(1.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.1f);
			pOtherPlayerScript->SetKnockback(150.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_SK:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE75_KAI:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(450.f);
			SoundOutput(L"common_00005.wav");
			if (iCurFrame > 6)
			{
				pOtherPlayerScript->SetOnGround(false);
				pOtherPlayerScript->SetJumpSpeed(1000.f);
			}
			break;
		case	KYO_REDKICK:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE100_ONIYAKI:
			//Damage(9.f);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(1300.f);
			pOtherPlayerScript->SetOnGround(false);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-50.f, 50.f), Vec2(25.f, 50.f), 5);
			ChangeMtrl(pOtherPlayer);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayerScript->SetJumpSpeed(1500.f);
			SoundOutput(L"common_00013.wav");
			break;
		case	KYO_UTYPE108_OROCHINAGI:
			//Damage(10.f + 3.f*m_fCharge);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(250.f + m_fCharge*75.f);
			pOtherPlayerScript->SetJumpSpeed(1000.f);
			pOtherPlayerScript->SetOnGround(false);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5 + (int)(m_fCharge * 2));
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00017.wav");
			break;
		case	KYO_TYPE114_ARAGAMI:
			//Damage(4.f);
			pOtherPlayerScript->SetStunHitTime(0.1);
			pOtherPlayerScript->SetKnockback(200.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE115_DOKUGAMI:
			//Damage(5.f);
			pOtherPlayerScript->SetStunHitTime(0.1f);
			pOtherPlayerScript->SetKnockback(300.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 5);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE125_NANASE:
			//Damage(6.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(550.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE127_YANOSABI:
			//Damage(6.f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE128_KONOKIZU:
			//Damage(4.5f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE182:
			//Damage(20.f + m_fCharge*6.f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(500.f + 500.f*m_fCharge*0.6f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5 + (int)(m_fCharge*3.f));
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00009.wav");
			break;
		case	KYO_TYPE401_TSUMIYOMI:
			//Damage(3.5f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_TYPE402_BATSUYOMI:
			//Damage(4.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(550.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE427_HIKIGANE_L:
			//Damage(5.f);
			pOtherPlayerScript->SetKnockback(450.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 100.f), 3);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE427_HIKIGANE_S:
			//Damage(8.f);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayerScript->SetJumpSpeed(1000.f);
			pOtherPlayerScript->SetKnockback(150.f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(0.f, 200.f), Vec2(50.f, 200.f), 7);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_OUTTYPE_GOUFU:
			//Damage(7.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_OUTTYPE_MIGIRI:
			//Damage(7.5f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 5);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_NOTYPE:
			if (iCurFrame <= 9)
			{
				//Damage(5.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 16 && iCurFrame > 9)
			{
				//Damage(4.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 23 && iCurFrame > 16)
			{
				//Damage(4.5f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 30 && iCurFrame > 23)
			{
				//Damage(6.f);
				SoundOutput(L"common_00007.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 36 && iCurFrame > 30)
			{
				//Damage(4.f);
				SoundOutput(L"common_00005.wav");
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetKnockback(150.f);
			}
			else if (iCurFrame <= 40 && iCurFrame > 36)
			{
				//Damage(4.5f);
				SoundOutput(L"common_00005.wav");
				pOtherPlayerScript->SetStunHitTime(2.f);
				pOtherPlayerScript->SetKnockback(0.f);
			}
			else
			{
				SoundOutput(L"common_00016.wav");
				//Damage(20.f);
				pOtherPlayerScript->SetStunHitTime(0.5f);
				pOtherPlayerScript->SetOnGround(false);
				pOtherPlayerScript->SetJumpSpeed(1600.f);
				pOtherPlayerScript->SetKnockback(150.f);


			}

			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 150.f), 3);

			ChangeMtrl(pOtherPlayer, 2.f);
			break;
		case	KYO_CROUCH_LP:
			//Damage(2.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_CROUCH_SP:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_CROUCH_LK:
			//Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_CROUCH_SK:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(800.f);
			pOtherPlayerScript->SetOnGround(false);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_TYPE88:
			//Damage(5.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_LP:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_SP:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	KYO_JUMP_LK:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	KYO_JUMP_SK:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00007.wav");
			break;
		case	KYO_OUTTYPE_NARAKU:
			//Damage(5.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.5);
			SoundOutput(L"common_00005.wav");
			break;
		}
	}

	//CEffectManager::GetInst()->ActiveEffect(L"Spark03", vOtherPos);
}

void CKyoHitCol::OnCollisionEnter(CCollider * _pOther)
{
	if (false == CPlayerManager::GetInst()->isNetWork())
		Collision_Update(_pOther);

	else
		Collision_NoUpdate(_pOther);

	
}

void CKyoHitCol::OnCollision(CCollider * _pOther)
{
}

void CKyoHitCol::OnCollisionExit(CCollider * _pOther)
{
	
}

void CKyoHitCol::EffectOutput(Vec3 _vPos,Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt)
{
	for (int i = 0; i < _iCnt; ++i)
	{
		float fRandPosX = getRandomNumber(_vXRange.x, _vXRange.y);
		float fRandPosY = getRandomNumber(_vYRange.x, _vYRange.y);
		float fRandScale = getRandomNumber(_vScaleRange.x, _vScaleRange.y);
		CEffectManager::GetInst()->ActiveEffect(
			L"Explosion1", _vPos + Vec3(fRandPosX, fRandPosY, 0.f),
			Vec3(fRandScale*1.5f, fRandScale, 1.f));
	}
}

void CKyoHitCol::EffectOutput(const wstring& _strKey, Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, 
	Vec2 _vScaleRange, float _fScale, int _iCnt)
{
	for (int i = 0; i < _iCnt; ++i)
	{
		float fRandPosX = getRandomNumber(_vXRange.x, _vXRange.y);
		float fRandPosY = getRandomNumber(_vYRange.x, _vYRange.y);
		float fRandScale = getRandomNumber(_vScaleRange.x, _vScaleRange.y);
		CEffectManager::GetInst()->ActiveEffect(
			_strKey, _vPos + Vec3(fRandPosX, fRandPosY, 0.f),
			Vec3(fRandScale*_fScale, fRandScale, 1.f));
	}
}

void CKyoHitCol::ChangeMtrl(CGameObject* _pOtherObj)
{
	CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Red");
	_pOtherObj->GetMeshRender()->SetMaterial(pMtrl);

	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)(_pOtherObj->GetFirstScript());
	pOtherPlayerScript->SetEffected(true);
}

void CKyoHitCol::ChangeMtrl(CGameObject * _pOtherObj, float _fTime)
{
	CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Red");
	_pOtherObj->GetMeshRender()->SetMaterial(pMtrl);

	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)(_pOtherObj->GetFirstScript());
	pOtherPlayerScript->SetEffected(true, _fTime);
}

void CKyoHitCol::SoundOutput(const wstring & _strKey)
{
	m_pSound = (CSound*)CResManager::Manager().FindSound(_strKey);
	m_pSound->Play(1);
}
