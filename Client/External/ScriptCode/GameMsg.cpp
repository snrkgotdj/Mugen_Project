#include "stdafx.h"
#include "GameMsg.h"
#include "AnimationEX.h"
#include "AnimatorEX.h"

CGameMsg::CGameMsg()
	: m_iState(0)
	, m_fTime(0.f)
	, m_fKOTime(0.f)
{
}


CGameMsg::~CGameMsg()
{
}

void CGameMsg::Awake()
{
}

void CGameMsg::Start()
{
}

void CGameMsg::Update()
{
	if (m_iState < 2)
	{
		m_fTime += DT;

		if (m_fTime <= 2.f)
		{
			SetState(0);
		}
		else if (m_fTime <= 4.f)
		{
			SetState(1);
		}
		else
		{
			SetState(-1);
		}
	}

	if (m_iState >= 2)
		m_fKOTime += DT;

//if (m_fKOTime > 2.f)
//	m_fKOTime = 0.f;
}

void CGameMsg::LateUpdate()
{
}

void CGameMsg::SetState(int _iState)
{
	m_iState = _iState;

	switch (_iState)
	{
	case 0:
		GetAnimatorEX()->ChangeAnimation(L"Ready");
		break;
	case 1:
		GetAnimatorEX()->ChangeAnimation(L"Fight");
		break;
	case 2:
	//GetAnimatorEX()->ChangeAnimation(L"KO");
	//GetTransform()->SetLocalScale(Vec3(600.f, 600.f, 100.f));
		break;
	case 3:
		GetAnimatorEX()->ChangeAnimation(L"Win1p");
		GetTransform()->SetLocalScale(Vec3(400.f, 100.f, 100.f));
		break;
	case 4:
		GetAnimatorEX()->ChangeAnimation(L"Win2p");
		GetTransform()->SetLocalScale(Vec3(400.f, 100.f, 100.f));
		break;
	case 5:
		GetAnimatorEX()->ChangeAnimation(L"Draw");
		break;
	case 6:
		GetAnimatorEX()->ChangeAnimation(L"Perfect");
		break;
	case 7:
		GetAnimatorEX()->ChangeAnimation(L"DKO");
		break;
	default:
		GetAnimatorEX()->SetCurAnim(NULL);
		break;
	}
}
