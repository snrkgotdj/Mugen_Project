#include "stdafx.h"
#include "IoriHitCol.h"

#include "Collider.h"

#include "GameObject.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "CharactorScript.h"
#include "UIManager.h"
#include "UIBar.h"
#include "UICombo.h"
#include "PlayerScIori.h"

#include "EffectManager.h"
#include "SaveLoadManager.h"

#include "IoriBullet.h"
#include "PlayerManager.h"

CIoriHitCol::CIoriHitCol()
	:m_fTime(0.f)
	, m_bDamaged(false)
	, m_bBullet(false)
	, m_bSound(false)
	, m_pOtherObj(NULL)
{
}


CIoriHitCol::~CIoriHitCol()
{
}

void CIoriHitCol::Start()
{
	wstring strLayerName = GetGameObject()->GetLayerName();
	wstring strBulletPath = CPathManager::GetResPathEX();
	strBulletPath += L"Save\\Bullet\\Iori\\Iori_Bullet.goc";

	CGameObject* pObj = CSaveLoadManager::Manager().LoadGameObjectOnly(strBulletPath);
	CSceneManager::Manager().GetCurScene()->FindLayer(strLayerName)->AddGameObject(pObj);
	pObj->GetTransform()->SetLocalPos(Vec3(1000.f,1000.f,3000.f));
}

void CIoriHitCol::Update()
{
	m_fTime += DT;
	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	GuardRecover();

	IORI_STATE eState = ((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetState();

	if (eState == IORI_TYPE212_KOTOTSUKI_SUCCESS)
	{
		if (m_fTime >= 1.f)
			m_bDamaged = false;

		if (!m_bDamaged)
		{
			m_fTime = 0.f;

			if (3 == iCurFrame)
			{
				Vec3 vScale = GetParentObject()->GetTransform()->GetLocalScale();
				CEffectManager::GetInst()->ActiveEffect(L"Explosion9",
					GetParentObject()->GetTransform()->GetLocalPos() - Vec3(vScale.x / 3.f, 0.f, 0.f));
				Damage(12.5f);
				m_bDamaged = true;
				if (m_pOtherObj != NULL)
				{
					ChangeMtrl(m_pOtherObj);
					EffectOutput(m_pOtherObj->GetTransform()->GetLocalPos(),
						Vec2(-75.f, 75.f), Vec2(-100.f, 0.f), Vec2(50.f, 100.f), 5);
				}
				
				SoundOutput(L"common_00017.wav");
			}
		}
	}
	else if (eState == IORI_CROUCH_LK||eState==IORI_CROUCH_LP)
	{
		if (m_fTime >= 1.f)
		{
			m_bDamaged = false;
			m_fTime = 0.f;
		}
	}
	else if (eState == IORI_KTYPE1211_YAOTOME)
	{
		if (iCurFrame < 40)
		{
			if (m_fTime >= 0.1f)
				m_bDamaged = false;
		}
		else
		{
			if (m_fTime >= 1.5f)
			{
				Vec3 vScale = GetParentObject()->GetTransform()->GetLocalScale();

				Damage(10.f);
				m_fTime = 0.f;
				m_bDamaged = false;
				EffectOutput(GetParentObject()->GetTransform()->GetLocalPos() - Vec3(vScale.x / 3.f, 0.f, 0.f),
					Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 7);
				SoundOutput(L"common_00017.wav");
				if (!m_bSound)
				{
					CSound* pSound= (CSound*)CResManager::Manager().FindSound(L"Iori_Yaotome3.wav");
					pSound->Play(1);
					m_bSound = true;
				}
			}
		}

		if (!m_bDamaged)
		{
			if (44 == iCurFrame)
			{
			}
		}
	}

	else if (eState == IORI_TYPE108_YAMIBARAI)
	{
		if (!m_bBullet&&iCurFrame == 2)
		{
			wstring strLayerName = GetGameObject()->GetLayerName();
			Vec3 vPos = GetParentObject()->GetTransform()->GetLocalPos();
			Vec3 vScale = GetParentObject()->GetTransform()->GetLocalScale();

			wstring strBulletPath = CPathManager::GetResPathEX();
			strBulletPath += L"Save\\Bullet\\Iori\\Iori_Bullet.goc";

			CGameObject* pObj = CSaveLoadManager::Manager().LoadGameObjectOnly(strBulletPath);
			CSceneManager::Manager().GetCurScene()->FindLayer(strLayerName)->AddGameObject(pObj);
			pObj->GetTransform()->SetLocalPos(vPos);
			pObj->GetTransform()->SetLocalScale(vScale);
			pObj->GetAnimatorEX()->ChangeAnimation(L"Effect");
			pObj->SetTagName(L"IoriBullet_108½Ä_Clone");
			((CIoriBullet*)(pObj->GetFirstScript()))->ResetTime();
			m_bBullet = true;
		}
	}

	else
	{
		m_bSound = false;
		m_bBullet = false;
		if (m_fTime >= 0.2f)
			m_bDamaged = false;
	}
}

void CIoriHitCol::Collision_Update(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	CCharactorScript* pOtherPlayerScript = ((CCharactorScript*)pOtherPlayer->GetFirstScript());
	CCharactorScript* pPlayerScript = ((CCharactorScript*)GetParentObject()->GetFirstScript());

	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();
	Vec3 vOtherScale = pOtherPlayer->GetTransform()->GetLocalScale();
	Vec3 vScale = GetParentObject()->GetTransform()->GetLocalScale();

	int iDir = pPlayerScript->GetDir();

	wstring strClipName = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetKey();
	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	IORI_STATE eState = ((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetState();
	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

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
			if (eState != IORI_CROUCH_LK &&eState != IORI_CROUCH_SK &&
				eState != IORI_CROUCH_LP)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				pOtherPlayerScript->PushPlayerX(10.f);
				GuardDamage(5.f);
				return;
			}
		}
	}

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case IORI_LP:
			Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_SP:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_LK:
			Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_SK:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_YUMEBIKI_A:
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			Damage(4.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_OUTTYPE_YUMEBIKI_B:
			Damage(6.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_GOUFU:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_TYPE100_ONIYAKI:
			Damage(10.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(1300.f);
			pOtherPlayerScript->SetOnGround(false);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00013.wav");
			break;
		case	IORI_TYPE108_YAMIBARAI:
			break;
		case	IORI_TYPE127_AOIHANA_A:
			Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(200.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_TYPE127_AOIHANA_B:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(200.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_TYPE127_AOIHANA_C:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			SoundOutput(L"common_00007.wav");
			break;
		case	IORI_TYPE212_KOTOTSUKI_START:
			Damage(2.5f);
			pOtherPlayerScript->SetStunHitTime(2.f);

			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_TYPE212_KOTOTSUKI_SUCCESS);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"212½Ä ±Ý¿ù À½_success", 3, -1, 0);
			SetOtherObj(pOtherPlayer);
			pOtherPlayerScript->SetEffectTimeLimit(1.5f);
			SoundOutput(L"common_00001.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", vOtherPos);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			break;
		case IORI_TYPE212_KOTOTSUKI_SUCCESS:

			break;
		case	IORI_KUZUKAZE_START:
			pOtherPlayerScript->SetKnockback(0.f);
			pOtherPlayerScript->SetStunHitTime(1.f);
			pOtherPlayerScript->SetDir(-(pOtherPlayerScript->GetDir()));
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			SoundOutput(L"common_00001.wav");
			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_KUZUKAZE_SUCCESS);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"¼³Ç³_success", 4, -1, 0);

			vOtherPos.x -= 100.f*iDir;
			pOtherPlayer->GetTransform()->SetLocalPos(vOtherPos);
			break;
		case	IORI_KUZUKAZE_FAIL:						 break;
		case	IORI_KUZUKAZE_SUCCESS:					 break;
		case IORI_KTYPE1211_YAOTOME_START:
			pOtherPlayerScript->SetStunHitTime(2.f);
			//((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetSound()->Stop(true);
			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_KTYPE1211_YAOTOME);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"±Ý 1211½Ä ÆÈÄ¡³à", 46, -1, 0);
			break;
		case	IORI_KTYPE1211_YAOTOME:
			if (iCurFrame <= 5)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood01", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark04", vOtherPos, Vec3(200.f*iDir, 100.f, 1.f));

				Damage(1.f);
			}
			else if (iCurFrame <= 31 && iCurFrame > 5)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood02", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark05", vOtherPos, Vec3(200.f*iDir, 250.f, 1.f));

				Damage(1.5f);
			}
			else if (iCurFrame <= 39 && iCurFrame > 31)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood03", vOtherPos, Vec3(200.f*iDir, 200.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark06", vOtherPos, Vec3(200.f*iDir, 200.f, 1.f));

				Damage(2.f);
			}
			else if (iCurFrame <= 44 && iCurFrame > 39)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood04", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));

				Damage(2.5f);
			}
			SoundOutput(L"common_00001.wav");
			((CCharactorScript*)pOtherPlayer->GetFirstScript())->SetStunHitTime(4.f);
			break;
		case	IORI_UTYPE316_SAIKA:
			Damage(15.f);
			pOtherPlayerScript->SetKnockback(600.f);
			pOtherPlayerScript->SetJumpSpeed(1600.f);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayer->GetAnimatorEX()->SetPause(false);
			pOtherPlayerScript->SetStunTime(0.f);
			pOtherPlayerScript->SetStunHitTime(0.f);
			SoundOutput(L"common_00017.wav");
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 7);
			ChangeMtrl(pOtherPlayer);
			break;
		case	IORI_CROUCH_LP:
			Damage(2.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_CROUCH_SP:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_CROUCH_LK:
			Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_CROUCH_SK:
			Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetJumpSpeed(800.f);
			pOtherPlayerScript->SetOnGround(false);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_JUMP_LP:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_JUMP_SP:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_JUMP_LK:
			Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_JUMP_SK:
			Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_YURIORI:
			Damage(5.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.4f);
			SoundOutput(L"common_00007.wav");
			break;
		}
	}

}

void CIoriHitCol::Collision_NoUpdate(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	CCharactorScript* pOtherPlayerScript = ((CCharactorScript*)pOtherPlayer->GetFirstScript());
	CCharactorScript* pPlayerScript = ((CCharactorScript*)GetParentObject()->GetFirstScript());

	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();
	Vec3 vOtherScale = pOtherPlayer->GetTransform()->GetLocalScale();
	Vec3 vScale = GetParentObject()->GetTransform()->GetLocalScale();

	int iDir = pPlayerScript->GetDir();

	wstring strClipName = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetKey();
	int iCurFrame = GetParentObject()->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	IORI_STATE eState = ((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetState();
	bool bOtherGuard = pOtherPlayerScript->IsGuard();
	bool bOtherCrouch = pOtherPlayerScript->IsCrouch();

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
			if (eState != IORI_CROUCH_LK &&eState != IORI_CROUCH_SK &&
				eState != IORI_CROUCH_LP)
			{
				((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
				pOtherPlayerScript->SetStunHitTime(0.2f);
				pOtherPlayerScript->PushPlayerX(10.f);
				GuardDamage(5.f);
				return;
			}
		}
	}

	if (!m_bDamaged)
	{
		m_bDamaged = true;
		m_fTime = 0.f;

		switch (eState)
		{
		case IORI_LP:
			//Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case IORI_SP:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_LK:
			//Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_SK:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_YUMEBIKI_A:
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			//Damage(4.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_OUTTYPE_YUMEBIKI_B:
			//Damage(6.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_GOUFU:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_TYPE100_ONIYAKI:
			//Damage(10.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			pOtherPlayerScript->SetJumpSpeed(1300.f);
			pOtherPlayerScript->SetOnGround(false);
			ChangeMtrl(pOtherPlayer);
			SoundOutput(L"common_00013.wav");
			break;
		case	IORI_TYPE108_YAMIBARAI:
			break;
		case	IORI_TYPE127_AOIHANA_A:
			//Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_TYPE127_AOIHANA_B:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.5f);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_TYPE127_AOIHANA_C:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			SoundOutput(L"common_00007.wav");
			break;
		case	IORI_TYPE212_KOTOTSUKI_START:
			//Damage(2.5f);
			pOtherPlayerScript->SetStunHitTime(2.f);

			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_TYPE212_KOTOTSUKI_SUCCESS);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"212½Ä ±Ý¿ù À½_success", 3, -1, 0);
			SetOtherObj(pOtherPlayer);
			pOtherPlayerScript->SetEffectTimeLimit(1.5f);
			SoundOutput(L"common_00001.wav");
			CEffectManager::GetInst()->ActiveEffect(L"Spark03", vOtherPos);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			break;
		case IORI_TYPE212_KOTOTSUKI_SUCCESS:

			break;
		case	IORI_KUZUKAZE_START:
			pOtherPlayerScript->SetKnockback(0.f);
			pOtherPlayerScript->SetStunHitTime(1.f);
			pOtherPlayerScript->SetDir(-(pOtherPlayerScript->GetDir()));
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			SoundOutput(L"common_00001.wav");
			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_KUZUKAZE_SUCCESS);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"¼³Ç³_success", 4, -1, 0);

			vOtherPos.x -= 100.f*iDir;
			pOtherPlayer->GetTransform()->SetLocalPos(vOtherPos);
			break;
		case	IORI_KUZUKAZE_FAIL:						 break;
		case	IORI_KUZUKAZE_SUCCESS:					 break;
		case IORI_KTYPE1211_YAOTOME_START:
			pOtherPlayerScript->SetStunHitTime(2.f);
			//((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetSound()->Stop(true);
			((CPlayerScIori*)GetParentObject()->GetFirstScript())->SetState(IORI_KTYPE1211_YAOTOME);
			GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"±Ý 1211½Ä ÆÈÄ¡³à", 46, -1, 0);
			break;
		case	IORI_KTYPE1211_YAOTOME:
			if (iCurFrame <= 5)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood01", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark04", vOtherPos, Vec3(200.f*iDir, 100.f, 1.f));

				//Damage(1.f);
			}
			else if (iCurFrame <= 31 && iCurFrame > 5)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood02", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark05", vOtherPos, Vec3(200.f*iDir, 250.f, 1.f));

				//Damage(1.5f);
			}
			else if (iCurFrame <= 39 && iCurFrame > 31)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood03", vOtherPos, Vec3(200.f*iDir, 200.f, 1.f));
				CEffectManager::GetInst()->ActiveEffect(L"Spark06", vOtherPos, Vec3(200.f*iDir, 200.f, 1.f));

				//Damage(2.f);
			}
			else if (iCurFrame <= 44 && iCurFrame > 39)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Blood04", vOtherPos, Vec3(100.f*iDir, 100.f, 1.f));

				//Damage(2.5f);
			}
			SoundOutput(L"common_00001.wav");
			((CCharactorScript*)pOtherPlayer->GetFirstScript())->SetStunHitTime(4.f);
			break;
		case	IORI_UTYPE316_SAIKA:
			//Damage(15.f);
			pOtherPlayerScript->SetKnockback(600.f);
			pOtherPlayerScript->SetJumpSpeed(1600.f);
			pOtherPlayerScript->SetOnGround(false);
			pOtherPlayer->GetAnimatorEX()->SetPause(false);
			pOtherPlayerScript->SetStunTime(0.f);
			pOtherPlayerScript->SetStunHitTime(0.f);
			SoundOutput(L"common_00017.wav");
			EffectOutput(vOtherPos, Vec2(-75.f, 75.f), Vec2(-100.f, 100.f), Vec2(50.f, 200.f), 7);
			ChangeMtrl(pOtherPlayer);
			break;
		case	IORI_CROUCH_LP:
			//Damage(2.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_CROUCH_SP:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_CROUCH_LK:
			//Damage(3.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_CROUCH_SK:
			//Damage(8.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetJumpSpeed(800.f);
			pOtherPlayerScript->SetOnGround(false);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_JUMP_LP:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_JUMP_SP:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_JUMP_LK:
			//Damage(4.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00001.wav");
			break;
		case	IORI_JUMP_SK:
			//Damage(7.f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetKnockback(300.f);
			SoundOutput(L"common_00005.wav");
			break;
		case	IORI_OUTTYPE_YURIORI:
			//Damage(5.5f);
			CEffectManager::GetInst()->FollowPos(L"Spark03", vOtherPos, iDir - 1);
			pOtherPlayerScript->SetStunHitTime(0.4f);
			SoundOutput(L"common_00007.wav");
			break;
		}
	}

}

void CIoriHitCol::OnCollision_Update(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	IORI_STATE eState = ((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetState();
	CCharactorScript* pOtherPlayerScript = ((CCharactorScript*)pOtherPlayer->GetFirstScript());
	bool bOtherGuard = pOtherPlayerScript->IsGuard();

	if (!m_bDamaged)
	{
		//if (true == bOtherGuard)
		//{
		//	//((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
		//	//pOtherPlayerScript->SetStunHitTime(0.2f);
		//	pOtherPlayerScript->PushPlayerX(10.f);
		//	m_bDamaged = true;
		//	//if(eState!=IORI_CROUCH_LK&&eState!=IORI_CROUCH_LP)
		//	//return;
		//}

		if (eState == IORI_TYPE212_KOTOTSUKI_SUCCESS)
		{
			ChangeMtrl(pOtherPlayer);
			m_bDamaged = true;
		}
		else if (eState == IORI_CROUCH_LK)
		{
			Damage(2.5f);
			m_bDamaged = true;
		}
		else if (eState == IORI_CROUCH_LP)
		{
			Damage(3.f);
			m_bDamaged = true;
		}
	}
}

void CIoriHitCol::OnCollision_NoUpdate(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	IORI_STATE eState = ((CPlayerScIori*)GetParentObject()->GetFirstScript())->GetState();
	CCharactorScript* pOtherPlayerScript = ((CCharactorScript*)pOtherPlayer->GetFirstScript());
	bool bOtherGuard = pOtherPlayerScript->IsGuard();

	if (!m_bDamaged)
	{
		//if (true == bOtherGuard)
		//{
		//	//((CCharactorScript*)(m_pPlayer->GetFirstScript()))->SetStunAttTime(0.2f);
		//	//pOtherPlayerScript->SetStunHitTime(0.2f);
		//	pOtherPlayerScript->PushPlayerX(10.f);
		//	m_bDamaged = true;
		//	//if(eState!=IORI_CROUCH_LK&&eState!=IORI_CROUCH_LP)
		//	//return;
		//}

		if (eState == IORI_TYPE212_KOTOTSUKI_SUCCESS)
		{
			ChangeMtrl(pOtherPlayer);
			m_bDamaged = true;
		}
		else if (eState == IORI_CROUCH_LK)
		{
			//Damage(2.5f);
			m_bDamaged = true;
		}
		else if (eState == IORI_CROUCH_LP)
		{
			//Damage(3.f);
			m_bDamaged = true;
		}
	}
}

void CIoriHitCol::OnCollisionEnter(CCollider * _pOther)
{
	if (false == CPlayerManager::GetInst()->isNetWork())
		Collision_Update(_pOther);

	else
		Collision_NoUpdate(_pOther);
}

void CIoriHitCol::OnCollision(CCollider * _pOther)
{
	if (false == CPlayerManager::GetInst()->isNetWork())
		OnCollision_Update(_pOther);

	else
		OnCollision_NoUpdate(_pOther);
}

void CIoriHitCol::EffectOutput(Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt)
{
	for (int i = 0; i < _iCnt; ++i)
	{
		float fRandPosX = getRandomNumber(_vXRange.x, _vXRange.y);
		float fRandPosY = getRandomNumber(_vYRange.x, _vYRange.y);
		float fRandScale = getRandomNumber(_vScaleRange.x, _vScaleRange.y);
		CEffectManager::GetInst()->ActiveEffect(
			L"Explosion8", _vPos + Vec3(fRandPosX, fRandPosY, 0.f),
			Vec3(fRandScale*1.5f, fRandScale, 1.f));
	}
}

void CIoriHitCol::ChangeMtrl(CGameObject * _pOtherObj)
{
	CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Blue");
	_pOtherObj->GetMeshRender()->SetMaterial(pMtrl);
	((CCharactorScript*)_pOtherObj->GetFirstScript())->SetEffected(true);
}

void CIoriHitCol::SoundOutput(const wstring & _strKey)
{
	CSound* pSound = (CSound*)CResManager::Manager().FindSound(_strKey);
	pSound->Play(1);
}
