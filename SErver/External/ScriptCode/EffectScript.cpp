#include "stdafx.h"
#include "EffectScript.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"


CEffectScript::CEffectScript()
	:m_bManager(true)
	, m_bContinue(false)
	, m_bActive(false)
	, m_bMove(false)
	, m_fSpeed(300.f)
{
}


CEffectScript::~CEffectScript()
{
}

void CEffectScript::Update()
{
	if (m_bActive == false)
		return;

	if (false == m_bManager)
	{
		if (true == GetAnimatorEX()->IsAnimFinish())	
			GetAnimatorEX()->SetCurAnim(NULL);
	}

	else if (true == m_bManager)
	{
		if (false == m_bContinue)
		{
			UnContinueEffect();
		}

		else if (true == m_bContinue)
		{
			Vec3 vPos = GetTransform()->GetLocalPos();
			vPos.z = 0.f;
			GetTransform()->SetLocalPos(vPos);
			ContinueEffect();
		}
	}
}

void CEffectScript::ContinueEffect()
{
	if (true == GetAnimatorEX()->IsAnimFinish())
	{	
		GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
	}
}

void CEffectScript::UnContinueEffect()
{
	if (true == GetAnimatorEX()->IsAnimFinish())
	{
		GetTransform()->SetLocalPos(Vec3(0.f, 0.f, 3000.f));
		GetAnimatorEX()->SetPause(true);
		GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
		GetAnimatorEX()->GetCurAnim()->SetAnimFinish(false);
		m_bActive = false;
	}
}
