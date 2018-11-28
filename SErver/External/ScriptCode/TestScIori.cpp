#include "stdafx.h"
#include "TestScIori.h"

#include "Collider.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "GameObject.h"

#include "ShaderManager.h"
#include "ResManager.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Layer.h"

#include "Material.h"
#include "ScriptCode\ShadowScript.h"

CTestScIori::CTestScIori()
	: m_fTime(0.f)
	, m_bEffect(false)
	, m_bSound(false)
{
	m_fJumpSpeed = 1300;
	m_fGravAccel = 4000;
	m_iDir = DIR_RIGHT;
	m_vecPos = Vec3Zero;
	m_fFloorY = 0.f;
	m_iBeforeIdx = -1;
}


CTestScIori::~CTestScIori()
{
}

void CTestScIori::Awake()
{
	m_fFloorY = 0.f;
}

void CTestScIori::Start()
{
	CreateShadow();

	((CShadowScript*)(m_pShadow->GetFirstScript()))->init(GetGameObject(), 0.03f, 0.10f);
}

void CTestScIori::Update()
{
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = GetTransform()->GetLocalScale().x;

	if (GetAnimatorEX() == NULL)
		return;

	if (m_iDir == DIR_LEFT)
		fDir = 300.f;
	else if (m_iDir == DIR_RIGHT)
		fDir = -300.f;

	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	bool bClipEnd = GetAnimatorEX()->GetCurAnim()->IsAnimFinish();
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurFrameCnt();
	int iMaxFrame = GetAnimatorEX()->GetCurAnim()->GetMaxFrameCnt();

	if (bClipEnd)
		ClipEnd();

	Aerial();

	if (strClipName == L"Hit1" || strClipName == L"Hit2")//쳐맞았다면
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(3);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
			m_fStunTime = 0.f;
			m_fStunHitTime = 0.f;
		}
	}
	else if (strClipName == L"Knockback")
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
			m_fStunTime = 0.f;
			m_fStunHitTime = 0.f;
		}
	}
	else if (strClipName == L"Knockdown")
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
		}
	}

	if ((INPUT(KEY_LEFT, STATE_UP) || INPUT(KEY_RIGHT, STATE_UP))
		&& (strClipName == L"BackWalk" || strClipName == L"FrontWalk" || strClipName == L"Run"))
		GetAnimatorEX()->ChangeAnimation(L"Idle", 9, -1, 0);

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));
	GetTransform()->SetLocalPos(vPos);

	Gravity();
	EffectedTime();

#if defined DEBUG | _DEBUG
	//char str[255];
	//GET_SINGLE(CDebugMgr)->SetResetTime(0.1f);
	//sprintf_s(str, "Cmd : (%d, %f) \n", m_vecCmd.size(), m_fTime);
	//GET_SINGLE(CDebugMgr)->OutputConsole(str);
#endif
}

void CTestScIori::LateUpdate()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	Vec3 vPos = GetTransform()->GetLocalPos();

	if (GetAnimatorEX() == NULL)
		return;

	if (vPos.y <= m_fFloorY)
	{
		vPos.y = m_fFloorY;
		m_fGravity = 0.f;
		m_fJumpSpeed = 0.f;
		m_bOnGround = true;
	}

	GetTransform()->SetLocalPos(vPos);
}

void CTestScIori::FinalUpdate()
{
	m_vecPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = vScale.x;
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	
	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->fSpeed * DT;
		m_vecPos.x -= m_fKnockback*DT;
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x -= GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->fSpeed * DT;
		m_vecPos.x += m_fKnockback*DT;
	}

	if (m_iBeforeIdx != GetAnimatorEX()->GetCurAnim()->GetCurIndex())
	{
		if (m_iDir == DIR_RIGHT)
		{
			m_vecPos.x -= GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}
		else if (m_iDir == DIR_LEFT)
		{
			m_vecPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}

		m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	}

	if (m_vecPos.x > 550.f)m_vecPos.x = 550.f;
	else if (m_vecPos.x < -550.f)m_vecPos.x = -550.f;

	GetTransform()->SetLocalPos(m_vecPos);

	m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
}

void CTestScIori::OnCollisionEnter(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = _pOther->GetGameObject()->GetParent();
	HIT_STATE eOtherHitState = ((CCharactorScript*)pOtherPlayer->GetFirstScript())->GetHitState();

	wstring strOtherTag = _pOther->GetGameObject()->GetTagName();

	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	if (strOtherTag == L"HitCol")
	{
		if (HIT_UP == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"Hit1", 3, -1, 0);
		}
		else if (HIT_MIDDLE == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
		}
		else if (HIT_DOWN == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
		}
		else if (HIT_CATCH == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
		}
		else if (HIT_AIRUP == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"AirHit", 2, -1, 0);
		}
		else if (HIT_KNOCKDOWN == eOtherHitState)
		{
			GetAnimatorEX()->ChangeAnimation(L"Knockdown", 2, -1, 0);
		}
	}
}

void CTestScIori::Aerial()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	bool bClipEnd = GetAnimatorEX()->GetCurAnim()->IsAnimFinish();

	if (strClipName == L"JumpUp")
	{
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;

		if (INPUT(KEY_LEFT, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront", 1, -1, 0);
		}
		if (INPUT(KEY_RIGHT, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack", 1, -1, 0);
		}

		if (m_fJumpSpeed <= 0.f)
			GetAnimatorEX()->ChangeAnimation(L"JumpDown", 1, -1, 0);
	}

	if (strClipName == L"AirHit")
	{
		if (m_iBeforeIdx == 0)
		{
			m_fJumpSpeed = 1300.f;
			m_bOnGround = false;
		}
		if (m_bOnGround)
		{
			if (bClipEnd)
			{
				GetAnimatorEX()->ChangeAnimation(L"StandUp2");
			}
		}
	}

	if (strClipName == L"JumpDown" || strClipName == L"JumpFront" || strClipName == L"JumpBack")
	{
		if (m_bOnGround == true)
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle", 8, -1, 0);
		}
	}
}

void CTestScIori::ClipEnd()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	float fDir = GetTransform()->GetLocalScale().x;

	if (strClipName != L"Idle"&&strClipName != L"JumpFront" && strClipName != L"JumpBack")
	{
		if (strClipName != L"Crouch"&&strClipName != L"AirHit")
			GetAnimatorEX()->ChangeAnimation(L"Idle", 8, -1, 0);

		m_eHitState = HIT_END;
		m_fKnockback = 0.f;
	}
	if (strClipName == L"JumpUp")
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown", 1, -1, 0);
	}
	if (strClipName == L"JumpDown"&&m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle", 8, -1, 0);
	}
	if (strClipName == L"FrontWalk")
	{
		if ((KEYPUSH(KEY_RIGHT) && fDir < 0) || KEYPUSH(KEY_LEFT) && fDir > 0)
		{
			GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 8, -1, 0);
		}
	}
	if (strClipName == L"BackWalk")
	{
		if ((KEYPUSH(KEY_LEFT) && fDir < 0) || KEYPUSH(KEY_RIGHT) && fDir > 0)
		{
			GetAnimatorEX()->ChangeAnimation(L"BackWalk", 8, -1, 0);
		}
	}
}

void CTestScIori::Gravity()
{
	if (m_bOnGround == true)
		return;

	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();

	vPos.y += m_fJumpSpeed * DT;
	m_fGravity += m_fGravAccel * DT;
	vPos.y -= m_fGravity * DT;

	GetTransform()->SetLocalPos(vPos);
}
