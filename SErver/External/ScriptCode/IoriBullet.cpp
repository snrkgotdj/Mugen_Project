#include "stdafx.h"
#include "IoriBullet.h"
#include "Collider.h"
#include "GameObject.h"
#include "CharactorScript.h"
#include "EffectScript.h"
#include "AnimatorEX.h"
#include "EffectManager.h"

CIoriBullet::CIoriBullet()
	: m_fTime(0.f)
	, m_bHit(false)
{
}


CIoriBullet::~CIoriBullet()
{
}

void CIoriBullet::Awake()
{
	m_fTime = 0.f;
}

void CIoriBullet::Start()
{
}

void CIoriBullet::Update()
{
	Vec3 vPos = GetTransform()->GetLocalPos();
	float fSpeed = GetTransform()->GetLocalScale().x*4.f;

	wstring strLayerName = GetGameObject()->GetLayerName();

	if (strLayerName == L"Hit_1P")
		m_b1P = true;
	else if (strLayerName == L"Hit_2P")
	{
		m_b1P = false;
	}

	vPos.x -= fSpeed*DT;

	GetTransform()->SetLocalPos(vPos);

	m_fTime += DT;
}

void CIoriBullet::LateUpdate()
{
	wstring strTag = GetGameObject()->GetTagName();

	if (m_fTime > 0.5f)
	{
		if (strTag == L"IoriBullet_108½Ä_Clone")
		{
			m_fTime = 0.f;

			GetGameObject()->Die();
		}
	}
}

void CIoriBullet::FinalUpdate()
{
}

void CIoriBullet::OnCollisionEnter(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetParentObject();
	CCharactorScript* pOtherPlayerScript = ((CCharactorScript*)pOtherPlayer->GetFirstScript());

	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();
	Vec3 vOtherScale = pOtherPlayer->GetTransform()->GetLocalScale();

	bool bOtherGuard = pOtherPlayerScript->IsGuard();

	if (m_bHit||GetGameObject()->isDead())
		return;

	m_bHit = true;

	if (true == bOtherGuard)
	{
		pOtherPlayerScript->SetStunHitTime(0.2f);
		pOtherPlayerScript->PushPlayerX(10.f);
		GuardDamage(5.f);
		GetGameObject()->Die();

		return;
	}

	Damage(5.f);
	((CCharactorScript*)pOtherPlayer->GetFirstScript())->SetKnockback(150.f);
	pOtherPlayerScript->SetStunHitTime(0.2f);
	ChangeMtrl(pOtherPlayer);
	EffectOutput(vOtherPos, Vec2(-50.f, 50.f), Vec2(-75.f, 0.f), Vec2(50.f, 100.f), 3);

	CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00013.wav");
	pSound->Play(1);

	GetGameObject()->Die();
}

void CIoriBullet::ChangeMtrl(CGameObject * _pOtherObj)
{
	CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Blue");
	_pOtherObj->GetMeshRender()->SetMaterial(pMtrl);
	((CCharactorScript*)_pOtherObj->GetFirstScript())->SetEffected(true);
}

void CIoriBullet::EffectOutput(Vec3 _vPos, Vec2 _vXRange, Vec2 _vYRange, Vec2 _vScaleRange, int _iCnt)
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