#include "Collider.h"

#include "Script.h"
#include "Material.h"
#include "Mesh.h"

#include "Device.h"

UINT CCollider::g_iColID = 0;

CCollider::CCollider()
	: m_iColID(g_iColID++)
	, m_vObjPos(Vec3(0.f, 0.f, 0.f))
	, m_vOffsetPos(Vec3(0.f, 0.f, 0.f))
	, m_vOffsetScale(Vec3(1.f, 1.f, 1.f))
	, m_iCollision(0)
{
}

CCollider::~CCollider()
{
}

void CCollider::FinalUpdate()
{
	m_vObjPos = GetTransform()->GetWorldPos();

	m_vObjPos.x += m_vOffsetPos.x * GetTransform()->GetFinalWorldMat()._11;
	m_vObjPos.y += m_vOffsetPos.y * GetTransform()->GetFinalWorldMat()._22;

	m_matColWorld = XMMatrixIdentity();

	m_matColWorld = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);

	m_matColWorld *= XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	m_matColWorld *= GetTransform()->GetFinalWorldMat();

}

void CCollider::OnCollisionEnter(CCollider * _pOther)
{
	list<Script*>& listScript = GetGameObject()->GetScriptList();
	list<Script*>::iterator iter = listScript.begin();

	for (; iter != listScript.end(); ++iter)
	{
		(*iter)->OnCollisionEnter(_pOther);
	}
}

void CCollider::OnCollision(CCollider * _pOther)
{
	list<Script*>& listScript = GetGameObject()->GetScriptList();
	list<Script*>::iterator iter = listScript.begin();

	for (; iter != listScript.end(); ++iter)
	{
		(*iter)->OnCollision(_pOther);
	}
}

void CCollider::OnCollisionExit(CCollider * _pOther)
{
	list<Script*>& listScript = GetGameObject()->GetScriptList();
	list<Script*>::iterator iter = listScript.begin();

	for (; iter != listScript.end(); ++iter)
	{
		(*iter)->OnCollisionExit(_pOther);
	}
}

void CCollider::UpdateData()
{
	g_Transform.matWorld = m_matColWorld;
	g_Transform.matWorld = XMMatrixTranspose(g_Transform.matWorld);

	const BUFFER* pBuffer = CDevice::GetInst().FindConstBuffer(L"Transform");

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(pBuffer->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &g_Transform, pBuffer->iSize);
	CONTEXT->Unmap(pBuffer->pConstBuffer, 0);

	CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pConstBuffer);
}

void CCollider::Save(FILE * _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::COLLIDER_2D;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);

	SaveResKey((CRes*)m_pMesh, _pFile);
	SaveResKey((CRes*)m_pMtrl, _pFile);
}

void CCollider::Load(FILE * _pFile)
{
	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);

	wstring strKey = LoadResKey(_pFile);
	if (!strKey.empty())
		m_pMesh = (CMesh*)CResManager::Manager().Load<CMesh>(strKey);

	strKey = LoadResKey(_pFile);
	if (!strKey.empty())
		m_pMtrl = CResManager::Manager().FindMaterial(strKey);
}
