#include "stdafx.h"
#include "UIPanel.h"

#include "Material.h"
#include "MeshRender.h"

CUIPanel::CUIPanel()
	: m_bCall(false)
{

}


CUIPanel::~CUIPanel()
{
}

void CUIPanel::Awake()
{
}

void CUIPanel::Start()
{
}

void CUIPanel::Update()
{
	if (true == m_bCall)
	{
		m_pTex = (CTexture*)CResManager::Manager().FindTexture(L"UIBarShape");
		GetMeshRender()->GetMaterial()->SetData(SHADER_PARAM::SP_TEX_0, &m_pTex);

		float f = 1.f;
		GetMeshRender()->GetMaterial()->SetData(SHADER_PARAM::SP_FLOAT_0, &f);
		GetMeshRender()->GetMaterial()->SetData(SHADER_PARAM::SP_FLOAT_1, &f);
	}
}

void CUIPanel::LateUpdate()
{
}
