#include "stdafx.h"
#include "DownColScript.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "Collider.h"

CDownColScript::CDownColScript()
{
}


CDownColScript::~CDownColScript()
{
}


void CDownColScript::MoveLayer(bool _1P)
{
	CLayer* pLayer;

	if (true == _1P)
	{
		pLayer = CSceneManager::Manager().GetCurScene()->FindLayer(L"Collider_1P");
	}

	else
	{
		pLayer = CSceneManager::Manager().GetCurScene()->FindLayer(L"Collider_2P");
	}

	CLayer* pCurLayer = CSceneManager::Manager().GetCurScene()->FindLayer(GetGameObject()->GetLayerName());
	if (NULL != pCurLayer)
	{
		pCurLayer->RemoveListOnly(GetGameObject());
	}
	pLayer->AddGameObject(GetGameObject());
}

void CDownColScript::Update()
{

}

void CDownColScript::FinalUpdate()
{
	if (m_pAnimator == NULL || m_pCollider == NULL || NULL == m_pTransForm)
		return;

	if (m_pAnimator->GetCurAnim() == NULL)
		return;

	const tFrameInfo* tCurFrameInfo = m_pAnimator->GetCurAnim()->GetCurFrameInfo();

	m_vecPos.x = tCurFrameInfo->tDownCol.vPos.x;
	m_vecPos.y = tCurFrameInfo->tDownCol.vPos.y;

	m_vecScale.x = tCurFrameInfo->tDownCol.vSize.x;
	m_vecScale.y = tCurFrameInfo->tDownCol.vSize.y;

	m_pCollider->SetPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CDownColScript::OnCollisionEnter(CCollider * _pOther)
{
	if (m_pTransForm->GetLocalScale() != Vec3Zero)
	m_pPlayer->GetFirstScript()->OnCollisionEnter(_pOther);
}

void CDownColScript::OnCollision(CCollider * _pOther)
{
}

void CDownColScript::OnCollisionExit(CCollider * _pOther)
{
}
