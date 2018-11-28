#include "stdafx.h"
#include "UpColScript.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "Collider.h"
#include "TransForm.h"
#include "SceneManager.h"
#include "Layer.h"

CUpColScript::CUpColScript()
{
}

CUpColScript::~CUpColScript()
{
}

void CUpColScript::MoveLayer(bool _1P)
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

void CUpColScript::Update()
{
	
}

void CUpColScript::FinalUpdate()
{
	if (m_pAnimator == NULL || m_pCollider == NULL || NULL == m_pTransForm)
		return;

	if (m_pAnimator->GetCurAnim() == NULL)
		return;

	const tFrameInfo* tCurFrameInfo = m_pAnimator->GetCurAnim()->GetCurFrameInfo();

	m_vecPos.x = tCurFrameInfo->tUpCol.vPos.x;
	m_vecPos.y = tCurFrameInfo->tUpCol.vPos.y;

	m_vecScale.x = tCurFrameInfo->tUpCol.vSize.x;
	m_vecScale.y = tCurFrameInfo->tUpCol.vSize.y;

	m_pCollider->SetPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CUpColScript::OnCollisionEnter(CCollider * _pOther)
{
	if(m_pTransForm->GetLocalScale()!=Vec3Zero)
		m_pPlayer->GetFirstScript()->OnCollisionEnter(_pOther);
}

void CUpColScript::OnCollision(CCollider * _pOther)
{
}

void CUpColScript::OnCollisionExit(CCollider * _pOther)
{
}
