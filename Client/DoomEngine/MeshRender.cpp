#include "MeshRender.h"
#include "ResManager.h"

#include "Device.h"

#include "Shader.h"
#include "Mesh.h"
#include "TransForm.h"
#include "Camera.h"
#include "Texture.h"
#include "Material.h"
#include "Collider.h"
#include "Animation.h"
#include "Animator.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "FontMgr.h"


CMeshRender::CMeshRender()
	: m_pMesh(NULL)
	, m_pMtrl(NULL)
	, m_eRSType(RS_TYPE::RS_ALL)
{
}


CMeshRender::~CMeshRender()
{
}

void CMeshRender::Render()
{
#ifdef _DEBUG | DEBUG
	if (NULL != GetCollider())
		GetCollider()->Render();
#endif

	if (NULL == m_pMtrl || NULL == m_pMesh)
	{
		return;
	}

	CDevice::GetInst().SetRSState(m_eRSType);

	m_pMesh->SetShader(m_pMtrl->GetShader());
	GetTransform()->UpdateData();

	if (GetAnimatorEX())
	{
		GetAnimatorEX()->UpdateData();
	}

	else
	{
		CAnimatorEX::Disable();
	}

	m_pMtrl->UpdateData();
	m_pMesh->UpdateData();
	m_pMesh->Render();
	TextRender();
}

CMaterial * CMeshRender::GetMaterial()
{
	if (NULL == m_pMtrl)
		return NULL;

	m_pMtrl = m_pMtrl->Clone();
	CResManager::Manager().AddCloneRes(m_pMtrl);
	return m_pMtrl;
}

void CMeshRender::TextRender()
{
	if (!m_vecText.empty())
	{
		tTextOutput tText = {};

		for (int i = 0; i < m_vecText.size(); ++i)
		{
			tText.strText = m_vecText[i].strText;
			tText.fPosX = m_vecText[i].fPosX;
			tText.fPosY = m_vecText[i].fPosY;
			tText.fSize = m_vecText[i].fSize;
			tText.iColor = m_vecText[i].iColor;
			CFontMgr::Manager().DrawFont(tText.strText.c_str(),
				tText.fPosX,
				tText.fPosY,
				tText.fSize,
				tText.iColor);
		}
	}
}

void CMeshRender::ModifyTextInfo(UINT _iIdx, tTextOutput _tText)
{
	if (_iIdx > m_vecText.size())
		return;

	m_vecText[_iIdx] = _tText;
}

void CMeshRender::Save(FILE * _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::MESHRENDER;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	SaveResKey((CRes*)m_pMesh, _pFile);

	SaveResKey((CRes*)m_pMtrl, _pFile);

	fwrite(&m_eRSType, sizeof(UINT), 1, _pFile);
}

void CMeshRender::Load(FILE * _pFile)
{
	wstring strKey = LoadResKey(_pFile);
	if (!strKey.empty())
		m_pMesh = (CMesh*)CResManager::Manager().Load<CMesh>(strKey);

	strKey = LoadResKey(_pFile);
	if (!strKey.empty())
		m_pMtrl = CResManager::Manager().FindMaterial(strKey);

	fread(&m_eRSType, sizeof(UINT), 1, _pFile);
}
