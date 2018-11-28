#include "stdafx.h"
#include "UICombo.h"
#include "AnimationEX.h"
#include "AnimatorEX.h"

CUICombo::CUICombo()
	: m_iVal(0)
	, m_fTime(0.f)
{
}


CUICombo::~CUICombo()
{
}

void CUICombo::Awake()
{
}

void CUICombo::Start()
{
}

void CUICombo::Update()
{
	wstring strTag=GetGameObject()->GetTagName();

	if (m_iVal < 2)
	{
		GetAnimatorEX()->SetCurAnim(NULL);
		GetParentObject()->GetAnimatorEX()->SetCurAnim(NULL);
	}
	else
	{
		GetParentObject()->GetAnimatorEX()->ChangeAnimation(L"aaaa");
		GetAnimatorEX()->ChangeAnimation(L"number");
		if (strTag == L"01")
		{
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex((9 - m_iVal % 10));
		}
		else if (strTag == L"10")
		{
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex((9 - (m_iVal / 10)));
		}
	}

	m_fTime += DT;

	if (m_iVal >= 99)
		m_iVal = 99;

	if (m_fTime > 2.f)
	{
		m_fTime = 0.f;
		m_iVal = 0;
		GetAnimatorEX()->SetCurAnim(NULL);
	}
}

void CUICombo::LateUpdate()
{
}

void CUICombo::AddValue()
{
	if (m_fTime > 0.01f)
	{
		m_iVal++;
		m_fTime = 0.f;
	}
}
