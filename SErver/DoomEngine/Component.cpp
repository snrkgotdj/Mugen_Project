#include "Component.h"

#include "GameObject.h"

CComponent::CComponent()
{
}

CComponent::~CComponent()
{
}

CGameObject * CComponent::GetParentObject()
{
	return m_pGameObj->GetParent();
}

CTransform * CComponent::GetTransform()
{
	return m_pGameObj->GetTransform();
}

CMeshRender * CComponent::GetMeshRender()
{
	return m_pGameObj->GetMeshRender();;
}

CCamera * CComponent::GetCamera()
{
	return m_pGameObj->GetCamera();
}

CCollider * CComponent::GetCollider()
{
	return m_pGameObj->GetCollider();
}

CAnimatorEX * CComponent::GetAnimatorEX()
{
	return m_pGameObj->GetAnimatorEX();
}

Script * CComponent::GetFirstScript()
{
	return m_pGameObj->GetFirstScript();
}
