#include "stdafx.h"
#include "ColScript.h"

CColScript::CColScript()
	: m_pCollider(NULL)
	, m_pTransForm(NULL)
	, m_pAnimator(NULL)
	, m_pPlayer(NULL)
	, m_b1P(true)
{
}

CColScript::~CColScript()
{
}

void CColScript::Awake()
{
	m_pAnimator = GetGameObject()->GetParent()->GetAnimatorEX();
	m_pTransForm = GetTransform();
	m_pCollider = GetCollider();
	m_pPlayer = GetGameObject()->GetParent();
}