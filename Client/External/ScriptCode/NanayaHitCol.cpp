#include "stdafx.h"
#include "NanayaHitCol.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "Collider.h"

#include "ScriptCode\EffectManager.h"
#include "ScriptCode\PlayerManager.h"
#include "CharactorScript.h"
#include "NanayaScript.h"
#include "ScriptCode\UIManager.h"
#include "UICombo.h"
#include "UIManager.h"


CNanayaHitCol::CNanayaHitCol()
	: m_fTime(0.f)
	, m_bDamaged(false)
	, m_pOtherPlayer(NULL)
	, m_pSound(NULL)
{
}

CNanayaHitCol::~CNanayaHitCol()
{
}

void CNanayaHitCol::SoundOutput(const wstring & _strKey)
{
	m_pSound = (CSound*)CResManager::Manager().FindSound(_strKey);
	m_pSound->Play(1);
}

void CNanayaHitCol::Update()
{
	CNanayaScript* pMyPlayerScript = (CNanayaScript*)(m_pPlayer->GetFirstScript());
	NANAYA_STATE eState = pMyPlayerScript->GetState();
	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	Vec3 vOtherPos = Vec3Zero;

	if(m_pOtherPlayer!=NULL)
	vOtherPos = m_pOtherPlayer->GetTransform()->GetLocalPos();

	if (m_bDamaged)
	{
		m_fTime += DT;

		if (eState == NANAYA_PALJUMCHOONG)
		{
			if (m_fTime > 0.1f)
			{
				m_bDamaged = false;
				m_fTime = 0.f;
			}
		}
		else
		{
			if (m_fTime > 0.2f)
			{
				m_bDamaged = false;
				m_fTime = 0.f;
			}
		}
	}

	if (eState == NANAYA_CHILYA)
	{
		if (iCurFrame == 7)
		{
			if (m_pOtherPlayer != NULL)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", vOtherPos);
				m_pOtherPlayer = NULL;
			}
		}
	}
}

void CNanayaHitCol::Collision_Update(CCollider* _pOther)
{
	const tFrameInfo* tCurFrameInfo = m_pAnimator->GetCurAnim()->GetCurFrameInfo();

	CGameObject* pOtherPlayer = _pOther->GetGameObject()->GetParent();
	CNanayaScript* pMyPlayerScript = (CNanayaScript*)(m_pPlayer->GetFirstScript());

	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)pOtherPlayer->GetFirstScript();

	NANAYA_STATE eState = pMyPlayerScript->GetState();

	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	int iDir = pMyPlayerScript->GetDir();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();

	//상대방이 가드일때
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
			if (eState != NANAYA_ATTACK_SIT_X &&eState != NANAYA_ATTACK_SIT_Z)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				pOtherPlayerScript->PushPlayerX(10.f);
				GuardDamage(5.f);
				return;
			}
		}
	}

	pMyPlayerScript->SetStunAttTime(0.1f);
	pOtherPlayerScript->SetStunHitTime(0.1f);
	m_vecPos = m_pPlayer->GetTransform()->GetLocalPos();

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case NANAYA_ATTACK_Light:
			Damage(1.5f);
			pOtherPlayerScript->SetKnockback(20);

			SoundOutput(L"common_00001.wav");

			CEffectManager::GetInst()->ActiveEffect(L"Spark03", m_vecPos+Vec3(100.f*iDir,0.f,0.f));
			
			break;
		case NANAYA_ATTACK_Medium:
			Damage(2);
			pOtherPlayerScript->SetKnockback(50);
			SoundOutput(L"common_00005.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			CEffectManager::GetInst()->ActiveEffect(L"Blood02", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			break;
		case NANAYA_ATTACK_Medium_Addition:
			Damage(3);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayerScript->SetJumpSpeed(1300.f);
			SoundOutput(L"common_00007.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			CEffectManager::GetInst()->ActiveEffect(L"Blood04", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			break;
		case NANAYA_ATTACK_Power:
			Damage(3);
			pOtherPlayerScript->SetKnockback(330);
			SoundOutput(L"common_00001.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			CEffectManager::GetInst()->ActiveEffect(L"Blood02", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			break;
		case NANAYA_PALJUMCHOONG:
			Damage(1);
			pOtherPlayerScript->SetKnockback(20);
			SoundOutput(L"common_00001.wav");

			CEffectManager::GetInst()->ActiveEffect(L"히트1", 
				m_pPlayer->GetTransform()->GetLocalPos()+Vec3(100.f*iDir, 0.f,0.f));
			CEffectManager::GetInst()->FollowPos(L"Blood02", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f), iDir + 1);
		break;

		case NANAYA_CHILYA:
		{
			if (iCurFrame == 6)
			{
				GetParentObject()->GetTransform()->SetLocalPos(vOtherPos + Vec3(-100.f*iDir, 150.f, 0.f));
				pMyPlayerScript->SetOnGround(false);
				m_pOtherPlayer = pOtherPlayer;
				SoundOutput(L"common_00001.wav");
			}

			Damage(7);
				pOtherPlayerScript->SetStunHitTime(0.5f);
				CEffectManager::GetInst()->ActiveEffect(L"Spark05", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
				CEffectManager::GetInst()->ActiveEffect(L"Blood02", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
	
			break;
		}
		case NANAYA_YUKTO:
		{
			Damage(7.5f);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(500.f);
			SoundOutput(L"common_00005.wav");
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		}
		case NANAYA_PALCHUN:
		{
			Damage(5);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetStunHitTime(0.3f);
			SoundOutput(L"common_00001.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark04", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f));
			CEffectManager::GetInst()->FollowPos(L"Blood03", m_vecPos + Vec3(100.f*iDir, 0.f, 0.f), iDir + 1);
			break;
		}
		case NANAYA_GUIKSYNANAYA:
		{
			Damage(40);

			pMyPlayerScript->SetState(NANAYA_GUIKSYNANAYA_HIT);
			m_pPlayer->GetAnimatorEX()->ChangeAnimation(L"극사나나야", 33, 33, 22);
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"히트2", m_pPlayer->GetTransform()->GetLocalPos());

			m_pPlayer->GetTransform()->SetLocalPos(Vec3(vecOther.x + 220.f*iDir, vecOther.y, vecOther.z));
			
			pOtherPlayerScript->SetStunHitTime(0.3f);
		}

		case NANAYA_ATTACK_SIT_Z:
		{
			Damage(1.5f);
			SoundOutput(L"common_00001.wav");
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		}
		case NANAYA_ATTACK_SIT_X:
		{
			Damage(2.5f);
			SoundOutput(L"common_00005.wav");
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		}
		default:
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y - 15, m_vecPos.z));
			break;
		}
	}
}

void CNanayaHitCol::Collision_NoUpdate(CCollider* _pOther)
{
	const tFrameInfo* tCurFrameInfo = m_pAnimator->GetCurAnim()->GetCurFrameInfo();

	CGameObject* pOtherPlayer = _pOther->GetGameObject()->GetParent();
	CNanayaScript* pMyPlayerScript = (CNanayaScript*)(m_pPlayer->GetFirstScript());

	CCharactorScript* pOtherPlayerScript = (CCharactorScript*)pOtherPlayer->GetFirstScript();

	NANAYA_STATE eState = pMyPlayerScript->GetState();

	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	int iDir = pMyPlayerScript->GetDir();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();

	//상대방이 가드일때
	if (true == bOtherGuard)
	{
		if (bOtherCrouch)
		{
			((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
			pOtherPlayerScript->SetStunHitTime(0.2f);
			return;
		}
		else
		{
			if (eState != NANAYA_ATTACK_SIT_X &&eState != NANAYA_ATTACK_SIT_Z)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				return;
			}
		}
	}

	pMyPlayerScript->SetStunAttTime(0.1f);
	pOtherPlayerScript->SetStunHitTime(0.1f);
	m_vecPos = m_pPlayer->GetTransform()->GetLocalPos();

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case NANAYA_ATTACK_Light:
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		case NANAYA_ATTACK_Medium:
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		case NANAYA_ATTACK_Medium_Addition:
			pOtherPlayerScript->SetOnGround(false);
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		case NANAYA_ATTACK_Power:
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		case NANAYA_PALJUMCHOONG:
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"히트1", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"히트1", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		case NANAYA_CHILYA:
		{
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"히트1", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"히트1", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		}
		case NANAYA_YUKTO:
		{
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		}
		case NANAYA_PALCHUN:
		{
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z));

			break;
		}
		case NANAYA_GUIKSYNANAYA:
		{
			pMyPlayerScript->SetState(NANAYA_GUIKSYNANAYA_HIT);
			m_pPlayer->GetAnimatorEX()->ChangeAnimation(L"극사나나야", 33, 33, 22);
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"히트2", m_pPlayer->GetTransform()->GetLocalPos());
			pOtherPlayerScript->SetStunHitTime(0.3f);
		}

		case NANAYA_ATTACK_SIT_Z:
		{
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		}
		case NANAYA_ATTACK_SIT_X:
		{
			Vec3 vecOther = _pOther->GetTransform()->GetTransform()->GetLocalPos();
			if (pMyPlayerScript->GetDir() == DIR_RIGHT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x + 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			else if (pMyPlayerScript->GetDir() == DIR_LEFT)
				CEffectManager::GetInst()->ActiveEffect(L"Spark03", Vec3(m_pPlayer->GetTransform()->GetLocalPos().x - 100, m_pPlayer->GetTransform()->GetLocalPos().y - 100.f, m_pPlayer->GetTransform()->GetLocalPos().z));
			break;
		}
		}
	}
}

void CNanayaHitCol::OnCollisionEnter(CCollider * _pOther)
{
	if (false == CPlayerManager::GetInst()->isNetWork())
		Collision_Update(_pOther);

	else
		Collision_NoUpdate(_pOther);
}

