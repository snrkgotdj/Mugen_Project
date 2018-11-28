#include "stdafx.h"
#include "UIGuardBar.h"
#include "Material.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "NetworkManager.h"
#include "..\Doom\define_server.h"

CUIGuardBar::CUIGuardBar()
	: m_fTerm(0.f)
	, m_bCall(false)
	, m_fDepleted(1.f)
	, m_fReduced(1.f)
	, m_fPrevReduced(1.f)
	, m_bis1P(true)
{
}


CUIGuardBar::~CUIGuardBar()
{
}

void CUIGuardBar::Awake()
{
	m_pMtrl = GetMeshRender()->GetMaterial();
}

void CUIGuardBar::Start()
{
}

void CUIGuardBar::Update()
{
	int bVertical = 0;
	CResPtr<CTexture> pTex = GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->pTex;
	m_pMtrl->SetData(SHADER_PARAM::SP_TEX_1, &pTex);
	m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &bVertical);
	float fVal1 = 1.f - m_fDepleted;
	float fVal2 = 1.f - m_fReduced;

	Vec4 vCol0;
	Vec4 vCol1;
	Vec4 vCol2;

	vCol0 = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	vCol1 = Vec4(1.5f, 1.5f, 0.5f, 1.f);
	vCol2 = Vec4One;
	m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_0, &fVal1);
	m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_1, &fVal2);
	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_0, &vCol0);
	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_1, &vCol1);
	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_2, &vCol2);

	m_fDepleted = m_fReduced*4.f*DT + m_fDepleted*(1 - 4.f*DT);
	if (abs(m_fDepleted - m_fReduced) <= 0.01f)
		m_fDepleted = m_fReduced;

	GetMeshRender()->SetMaterial(m_pMtrl);

	#ifdef _DEBUG | DEBUG
	if (CKeyManager::Manager().GetKeyState(KEY_F3, STATE_DOWN))
	{
		m_fDepleted = 1.f;
		m_fReduced = 1.f;
	}
	#endif
	//	GetTransform()->SetLocalScale(Vec3(m_fDepleted*vBeginScale.x, vBeginScale.y, vBeginScale.z));

	//GetTransform()->SetLocalPos(Vec3(m_fDepleted*vBeginPos.x / 2.f, vBeginPos.y, vBeginPos.z));
	if (m_fPrevReduced != m_fReduced)
	{
		if (true == CKeyManager::Manager().isServer())
		{
			PACKET_HP tPacket;
			tPacket.bPlayer = m_bis1P;
			tPacket.fDamage = m_fReduced;
			ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_GUARD_HP, (char*)&tPacket, sizeof(tPacket));
		}
	}
	
	m_fPrevReduced = m_fReduced;
}

void CUIGuardBar::LateUpdate()
{
}
