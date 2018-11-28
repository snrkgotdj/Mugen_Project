#include "stdafx.h"
#include "UIBtnOutput.h"
#include "KeyManager.h"

#include "AnimatorEX.h"
#include "AnimationEX.h"

CUIBtnOutput::CUIBtnOutput()
	: m_iSize(-1)
	, m_b1Player(true)
	, m_pTarget(NULL)
	, m_bActive(true)
	, m_fTime(0.f)
{
	m_tKey.Right = KEY_RIGHT;
	m_tKey.Left = KEY_LEFT;
	m_tKey.Up = KEY_UP;
	m_tKey.Down = KEY_DOWN;
	m_tKey.Z = KEY_Z;
	m_tKey.X = KEY_X;
	m_tKey.C = KEY_C;
	m_tKey.A = KEY_A;
	m_tKey.S = KEY_S;
	m_tKey.D = KEY_D;
}


CUIBtnOutput::~CUIBtnOutput()
{
}

void CUIBtnOutput::Awake()
{
	list<CGameObject*> listChild = GetGameObject()->GetChildList();
	list<CGameObject*>::iterator iter = listChild.begin();
	list<CGameObject*>::iterator iterEnd = listChild.end();

	for (iter; iter != iterEnd; ++iter)
	{
		(*iter)->GetAnimatorEX()->SetPause(true);
		(*iter)->GetAnimatorEX()->SetCurAnim(NULL);
	}
}

void CUIBtnOutput::Start()
{
}

void CUIBtnOutput::Update()
{
	if (!m_bActive)
		return;

	m_fTime += DT;

	if (m_fTime > 2.f||KEYDOWN(KEY_F3))
		Clear();

	if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X) || KEYDOWN_PLAYER(m_tKey.C) ||
		KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S) || KEYDOWN_PLAYER(m_tKey.D) ||
		KEYDOWN_PLAYER(m_tKey.Down) || KEYDOWN_PLAYER(m_tKey.Right) || KEYDOWN_PLAYER(m_tKey.Left))
	{
		m_fTime = 0.f;

		if (m_iSize < 9)
			m_iSize++;
		else
			m_iSize = 9;

		switch (m_iSize)
		{
		case 0:
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 1:
			SetFrame();		
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 2:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 3:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 4:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 5:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 6:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 7:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 8:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;
		case 9:
			SetFrame();
			m_pTarget = FindTarget(L"Idx0");
			break;

		default:
			m_pTarget = NULL;
		}
	}

	if (m_pTarget == NULL)
		return;

	if (KEYDOWN_PLAYER(m_tKey.Z))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
	}
	else if (KEYDOWN_PLAYER(m_tKey.X))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(1);
	}
	else if (KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.C))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
	}
	else if (KEYDOWN_PLAYER(m_tKey.S) || KEYDOWN_PLAYER(m_tKey.D))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(3);
	}
	else if (KEYDOWN_PLAYER(m_tKey.Down))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(4);
	}
	else if (KEYDOWN_PLAYER(m_tKey.Left))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(5);
	}
	else if (KEYDOWN_PLAYER(m_tKey.Right))
	{
		m_pTarget->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(6);
	}
}

void CUIBtnOutput::LateUpdate()
{
}

void CUIBtnOutput::SetFrame(const wstring& _strPrev, const wstring& _strNext)
{
	wstring strChildName = L"Idx";
	wchar_t szIdx[8];
	wchar_t szPrevIdx[8];
	_itow_s(m_iSize, szIdx, 10);
	_itow_s(m_iSize-1, szPrevIdx, 10);

	strChildName += szIdx;

	int iPrevFrame = FindTarget(_strPrev)->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	
	FindTarget(_strNext)->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
	FindTarget(_strNext)->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(iPrevFrame);
}

void CUIBtnOutput::SetFrame()
{
	if (m_iSize < 1)
		return;

	for (int i = m_iSize; i > 0; --i)
	{
		wstring strChildName = L"Idx";
		wstring strPrevChildName = L"Idx";
		wchar_t szIdx[8];
		wchar_t szPrevIdx[8];

		_itow_s(i, szIdx, 10);
		_itow_s(i - 1, szPrevIdx, 10);

		strChildName += szIdx;
		strPrevChildName += szPrevIdx;

		if (FindTarget(strPrevChildName)->GetAnimatorEX()->GetCurAnim() == NULL)
			continue;
		int iPrevFrame = FindTarget(strPrevChildName)->GetAnimatorEX()->GetCurAnim()->GetCurIndex();

		FindTarget(strChildName)->GetAnimatorEX()->ChangeAnimation(L"BtnOutput");
		FindTarget(strChildName)->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(iPrevFrame);
	}
}

void CUIBtnOutput::Clear()
{
	list<CGameObject*> listChild = GetGameObject()->GetChildList();
	list<CGameObject*>::iterator iter = listChild.begin();
	list<CGameObject*>::iterator iterEnd = listChild.end();

	for (iter; iter != iterEnd; ++iter)
	{
		(*iter)->GetAnimatorEX()->SetPause(true);
		(*iter)->GetAnimatorEX()->SetCurAnim(NULL);
	}
}

CGameObject * CUIBtnOutput::FindTarget(const wstring& _strChildTag)
{
	return GetGameObject()->GetChildByName(_strChildTag);
}
