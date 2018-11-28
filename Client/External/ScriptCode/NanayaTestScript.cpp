#include "stdafx.h"
#include "NanayaTestScript.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"

#include "GameObject.h"

#include "AnimatorEX.h"
#include "TransForm.h"
#include "Collider2D.h"
#include "AnimationEX.h"

#include "KeyCommand.h"

#include "EffectScript.h"
#include "ScriptCode\ShadowScript.h"

CNanayaTestScript::CNanayaTestScript()
	:m_eState(NANAYA_IDLE)
	, m_fWalkSpeed(300)
{
	m_fJumpSpeed = 0;
	m_fGravAccel = 4000;
	m_fKnockback = 0;
}

CNanayaTestScript::~CNanayaTestScript()
{
}

void CNanayaTestScript::Awake()
{
	CCharactorScript::Awake();

	m_pEffect = GetGameObject()->GetChildByName(L"Nanaya_Effect");
	if (m_pEffect != NULL)
	{
		m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);
		((CEffectScript*)m_pEffect->GetFirstScript())->SetManaged(false);
	}
}

void CNanayaTestScript::Start()
{
	m_eState = NANAYA_IDLE;
	m_pAnimator->ChangeAnimation(L"Idle");

	CreateShadow();

	((CShadowScript*)(m_pShadow->GetFirstScript()))->init(GetGameObject(), 0.07f, 0.23f);
}

void CNanayaTestScript::Update()
{
	if (m_pKeyCommand == NULL)
		return;

	m_pKeyCommand->Update();

	m_vecPos = m_pTransForm->GetLocalPos();
	m_vecScale = m_pTransForm->GetLocalScale();

	if (m_iDir == DIR_RIGHT)
	{
		if (m_vecScale.x <= 0)
		{
			m_vecScale.x *= -1;
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		if (m_vecScale.x >= 0)
		{
			m_vecScale.x *= -1;
		}
	}

	switch (m_eState)
	{
	case NANAYA_IDLE:
		Idle();
		break;
	case NANAYA_HITTED_UP:
		Hitted_Up();
		break;
	case NANAYA_HITTED_MIDDLE:
		Hitted_Middle();
		break;
	case NANAYA_HITTED_DOWN:
		Hitted_Down();
		break;
	case NANAYA_KNOCKDOWN:
		KnockDown();
		break;
	case NANAYA_STANDUP:
		StandUp();
		break;

	default:
		break;
	}

	EffectedTime();
	m_pTransForm->SetLocalPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CNanayaTestScript::LateUpdate()
{
	if (NULL == m_pTransForm || NULL == m_pAnimator)
		return;

	m_vecPos = m_pTransForm->GetLocalPos();

	m_vecPos.x += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->fSpeed * DT;
	m_vecPos.y += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->fJumpSpeed * DT;

	Gravity();

	if (m_vecPos.y <= m_fFloorY && m_eState != NANAYA_JUMP_STAY)
	{
		m_vecPos.y = m_fFloorY;
		m_fGravity = 0;
		m_bOnGround = true;
	}

	m_pTransForm->SetLocalPos(m_vecPos);
}

void CNanayaTestScript::OnCollisionEnter(CCollider * _pOther)
{
	CGameObject* m_pOtherPlayer = NULL;

	CCharactorScript* m_pOtherPlayerScript = NULL;

	HIT_STATE eOtherHitState = HIT_MIDDLE;

	if (_pOther->GetGameObject()->GetParent() != NULL)
	{
		m_pOtherPlayer = _pOther->GetGameObject()->GetParent();
		eOtherHitState = ((CCharactorScript*)m_pOtherPlayer->GetFirstScript())->GetHitState();
		m_pOtherPlayerScript = ((CCharactorScript*)m_pOtherPlayer->GetFirstScript());
	}
	else
	{
		m_pOtherPlayer = _pOther->GetGameObject();
	}

	if (HIT_UP == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_UP;
		m_pAnimator->ChangeAnimation(L"상단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}	
	}
	else if (HIT_MIDDLE == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_MIDDLE;
		m_pAnimator->ChangeAnimation(L"중단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}
	}
	else if (HIT_DOWN == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_DOWN;
		m_pAnimator->ChangeAnimation(L"하단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}
	}
	else if (HIT_AIRUP == eOtherHitState)
	{
		m_eState = NANAYA_KNOCKDOWN;
		m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
	}

	else if (HIT_KNOCKDOWN == eOtherHitState)
	{
		m_eState = NANAYA_KNOCKDOWN;
		m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
	}
}

void CNanayaTestScript::KnockDown()
{
	m_vecPos.x -= m_fKnockback * DT*m_iDir;
	if (true == m_pAnimator->IsAnimFinish())
	{
		if (true == CKeyManager::Manager().GetKeyState(KEY_RIGHT, STATE_PUSH)
			|| true == CKeyManager::Manager().GetKeyState(KEY_LEFT, STATE_PUSH)
			|| true == CKeyManager::Manager().GetKeyState(KEY_UP, STATE_PUSH)
			|| true == CKeyManager::Manager().GetKeyState(KEY_DOWN, STATE_PUSH))
		{
			m_eState = NANAYA_STANDUP;
			m_pAnimator->ChangeAnimation(L"일어서기2",5, -1, 0);
		}
	}

	if( true == m_bOnGround)
		m_fKnockback = 0;
}

void CNanayaTestScript::StandUp()
{
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaTestScript::Gravity()
{
	if (m_bOnGround == true)
		return;

	if (m_fStunHitTime == 0)
	{
		m_vecPos.y += m_fJumpSpeed * DT;
		m_fGravity += m_fGravAccel * DT;
		m_vecPos.y -= m_fGravity * DT;
	}

	else
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			m_fStunTime = 0;
			m_fStunHitTime = 0;
			GetAnimatorEX()->SetPause(false);
		}
	}

	GetTransform()->SetLocalPos(m_vecPos);
}

void CNanayaTestScript::Idle()
{
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaTestScript::Hitted_Middle()
{
	m_vecPos.x -= m_fKnockback * DT*m_iDir;
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		m_fKnockback = 0;
	}
}

void CNanayaTestScript::Hitted_Up()
{
	m_vecPos.x -= m_fKnockback * DT*m_iDir;
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		m_fKnockback = 0;
	}
}

void CNanayaTestScript::Hitted_Down()
{
	m_vecPos.x -= m_fKnockback * DT*m_iDir;
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		m_fKnockback = 0;
	}
}
