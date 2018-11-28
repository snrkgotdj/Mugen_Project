#include "stdafx.h"
#include "Collider2D.h"
#include "ResManager.h"
#include "Material.h"
#include "Mesh.h"

Collider2D::Collider2D()
{
	m_pMesh = (CMesh*)CResManager::Manager().Load<CMesh>(L"ColliderRectMesh");
	m_pMtrl = CResManager::Manager().FindMaterial(L"ColliderMtrl");

	m_iCollision = 0;
	m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &m_iCollision);
}


Collider2D::~Collider2D()
{
}



void Collider2D::Awake()
{

}

void Collider2D::Render()
{
	m_pMesh->SetShader(m_pMtrl->GetShader());

	UpdateData();

	m_pMtrl->UpdateData();

	m_pMesh->UpdateData();

	m_pMesh->Render();
}

void Collider2D::OnCollisionEnter(CCollider * _pOther)
{
	CCollider::OnCollisionEnter(_pOther);
	m_iCollision = 1;
	m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &m_iCollision);
}

void Collider2D::OnCollisionExit(CCollider * _pOther)
{
	CCollider::OnCollisionExit(_pOther);
	m_iCollision = 0;
	m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &m_iCollision);
}

void Collider2D::Save(FILE* _pFile)
{
	CCollider::Save(_pFile);
}

void Collider2D::Load(FILE* _pFile)
{
	CCollider::Load(_pFile);
}

