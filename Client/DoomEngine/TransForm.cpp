#include "TransForm.h"

#include "TimeManager.h"
#include "KeyManager.h"
#include "Device.h"
#include "GameObject.h"

tTransform		g_Transform;

void CTransform::Save(FILE* _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::TRANSFORM;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	fwrite(&m_vtxPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vtxScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vtxRot, sizeof(Vec3), 1, _pFile);
}

void CTransform::Load(FILE* _pFile)
{
	fread(&m_vtxPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vtxScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vtxRot, sizeof(Vec3), 1, _pFile);
}

CTransform::CTransform()
	: m_vtxPos(Vec3(0.f, 0.f, 0.f))
	, m_vtxScale(Vec3(1.f, 1.f, 1.f))
	, m_vtxRot(Vec3(0.f, 0.f, 0.f))
{
}

CTransform::CTransform(Vec3 _vecPos, Vec3 _vecScale, Vec3 _vecRot)
	: m_vtxPos(_vecPos)
	, m_vtxScale(_vecScale)
	, m_vtxRot(_vecRot)
{
}

CTransform::~CTransform()
{
}

void CTransform::FinalUpdate()
{
	m_matWorld = XMMatrixIdentity();
	 
	Matrix matScale = XMMatrixScaling(m_vtxScale.x, m_vtxScale.y, m_vtxScale.z);

	Matrix matRot = XMMatrixRotationX(m_vtxRot.x);
	matRot *= XMMatrixRotationY(m_vtxRot.y);
	matRot *= XMMatrixRotationZ(m_vtxRot.z);

	Matrix matTrans = XMMatrixTranslation(m_vtxPos.x, m_vtxPos.y, m_vtxPos.z);

	m_matWorld = matScale* matRot* matTrans;

	CGameObject* pParentObject = GetParentObject();

	if (NULL != pParentObject)
	{
		m_matFinalWorld = m_matWorld * pParentObject->GetTransform()->GetWorldMat();
	}
	else
	{
		m_matFinalWorld = m_matWorld;
	}

}

void CTransform::UpdateData()
{
	g_Transform.matWorld = m_matFinalWorld;
	g_Transform.matWorld = XMMatrixTranspose(g_Transform.matWorld);

	const BUFFER* pBuffer = CDevice::GetInst().FindConstBuffer(L"Transform");

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(pBuffer->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &g_Transform, pBuffer->iSize);
	CONTEXT->Unmap(pBuffer->pConstBuffer, 0);

	CONTEXT->VSSetConstantBuffers(pBuffer->iRegister, 1, &pBuffer->pConstBuffer);
}

Vec3 CTransform::GetWorldPos()
{
	XMVECTOR v = XMLoadFloat3(&Vec3(0.f, 0.f, 0.f));
	v = XMVector3TransformCoord(v, m_matFinalWorld);

	Vec3 vWorldPos;
	XMStoreFloat3(&vWorldPos, v);

	return vWorldPos;
}

Vec3& CTransform::GetWorldScale()
{
	if (GetParentObject() != NULL)
	{
		Vec3 vParentsScale = GetParentObject()->GetTransform()->GetWorldScale();
		return m_vtxScale * vParentsScale;
	}
	
	return m_vtxScale;
}
