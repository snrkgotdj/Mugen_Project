#include "stdafx.h"
#include "ShadowScript.h"
#include "AnimatorEX.h"
#include "Mesh.h"
#include "CharactorScript.h"



CShadowScript::CShadowScript()
	:m_pPlayer(NULL)
{
}

CShadowScript::~CShadowScript()
{
}


void CShadowScript::init(CGameObject* _pPlayer, float _X, float _Y)
{
	if (NULL == _pPlayer)
	{
		MessageBox(NULL, L"그림자 생성 오류", L"오류", MB_OK);
		return;
	}
	X = _X;
	Y = _Y;

	m_pPlayer = _pPlayer;
	GetTransform()->SetLocalScale(m_pPlayer->GetTransform()->GetLocalScale());

	Vec3 vecPos = GetTransform()->GetLocalPos();
	vecPos.x = m_pPlayer->GetTransform()->GetLocalPos().x - m_pPlayer->GetTransform()->GetLocalScale().x * _X;
	vecPos.y = m_pPlayer->GetTransform()->GetLocalPos().y + m_pPlayer->GetTransform()->GetLocalScale().y * _Y;

	GetTransform()->SetLocalPos(vecPos);
	GetAnimatorEX()->SetCurAnim(m_pPlayer->GetAnimatorEX()->GetCurAnim());
	GetAnimatorEX()->SetPause(true);
}

void CShadowScript::Update()
{
	if (NULL == m_pPlayer)
		return;

	GetAnimatorEX()->SetCurAnim(m_pPlayer->GetAnimatorEX()->GetCurAnim());
	Vec3 vecPos = GetTransform()->GetLocalPos();
	Vec3 vecScale = GetTransform()->GetLocalScale();
	
	Vec3 vecPlayer = m_pPlayer->GetTransform()->GetLocalScale();

	CMesh* pMesh = GetMeshRender()->GetMesh();

	int iDir = ((CCharactorScript*)m_pPlayer->GetFirstScript())->GetDir();
	if (vecPlayer.x >= 0)
	{
		vecPos.x = m_pPlayer->GetTransform()->GetLocalPos().x - m_pPlayer->GetTransform()->GetLocalScale().x * X;
		if (vecScale.x <= 0)
		{
			vecScale.x *= -1;
		}
		if (pMesh->GetKey() == L"Shadow_Mesh_Reverse")
		{
			GetMeshRender()->SetMesh(CResManager::Manager().FindMesh(L"Shadow_Mesh"));
		}
	}
	else if (vecPlayer.x <= 0)
	{
		vecPos.x = m_pPlayer->GetTransform()->GetLocalPos().x + m_pPlayer->GetTransform()->GetLocalScale().x * X;
		if (vecScale.x >= 0)
		{
			vecScale.x *= -1;		
		}
		if (pMesh->GetKey() == L"Shadow_Mesh")
		{
			GetMeshRender()->SetMesh(CResManager::Manager().FindMesh(L"Shadow_Mesh_Reverse"));
		}
	}
	
	GetTransform()->SetLocalPos(vecPos);
	GetTransform()->SetLocalScale(vecScale);
}

