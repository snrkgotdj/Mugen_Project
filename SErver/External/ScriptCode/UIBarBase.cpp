#include "stdafx.h"
#include "UIBarBase.h"

#include "Material.h"
#include "ResManager.h"


CUIBarBase::CUIBarBase()
	: m_bCall(false)
{
	
}

CUIBarBase::~CUIBarBase()
{
}

void CUIBarBase::Awake()
{
	vBeginPos = GetTransform()->GetLocalPos();
	vBeginScale = GetTransform()->GetLocalScale();
	//CResManager::Manager().Load<CTexture>(L"UIBarBase", L"Texture\\UI\\UIBarBase.png");
}

void CUIBarBase::Start()
{

}

void CUIBarBase::Update()
{
	m_pMtrl = GetMeshRender()->GetMaterial();
	//m_pTex = CResManager::Manager().FindTexture(L"UIBarBase");
	m_pMtrl->SetData(SHADER_PARAM::SP_TEX_0, &m_pTex);
	GetMeshRender()->SetMaterial(m_pMtrl);

	GetTransform()->SetLocalScale(Vec3(500.f, 50.f, 1.f));

	//2p¿ë
	if (GetTransform()->GetLocalPos().x > 0)
	{
		GetTransform()->SetLocalPos(Vec3(350.f, 250.f, 1.f));
	}
	//1p¿ë
	else
	{
		GetTransform()->SetLocalPos(Vec3(-350.f, 250.f, 1.f));
	}
}

void CUIBarBase::LateUpdate()
{
}