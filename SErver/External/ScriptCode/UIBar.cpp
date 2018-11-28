#include "stdafx.h"
#include "UIBar.h"

#include "MeshRender.h"
#include "Material.h"
#include "Mesh.h"

#include "ResManager.h"
#include "ShaderManager.h"

#include "Device.h"

#include "AnimationEX.h"
#include "AnimatorEX.h"
#include "NetworkManager.h"
#include "..\Doom\define_server.h"

CUIBar::CUIBar()
	: m_fTerm(0.f)
	, m_bCall(false)
	, m_fDepleted(1.f)
	, m_fReduced(1.f)
	, m_fPrevReduced(1.f)
	, m_bis1P(true)
{
	
}


CUIBar::~CUIBar()
{
}

void CUIBar::Awake()
{
	m_pMtrl = GetMeshRender()->GetMaterial();

	m_vCol[0] = Vec4(0.5f, 0.5f, 0.5f, 1.f);
	m_vCol[1] = Vec4(1.5f, 0.5f, 0.5f, 1.f);
	m_vCol[2] = Vec4(0.33f, 1.2f, 1.25f, 1.f);
	//테스트용
	//m_fDepleted = (float)((rand() % 100) / 100.f);
	//m_fReduced = m_fDepleted / 2.f;
}

void CUIBar::Start()
{
}

void CUIBar::Update()
{
	if (GetGameObject()->GetTagName() == L"HPBar2P")
	{
		m_fDepleted = m_fReduced*4.f*DT + m_fDepleted*(1 - 4.f*DT);
		if (abs(m_fDepleted - m_fReduced) <= 0.01f)
			m_fDepleted = m_fReduced;

		float fVal1 = 1.f - m_fDepleted;
		float fVal2 = 1.f - m_fReduced;

		if (fVal1 > fVal2)
			fVal1 = fVal2;

		int bVertical = 0;
		CResPtr<CTexture> pTex = GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->pTex;
		m_pMtrl->SetData(SHADER_PARAM::SP_TEX_1, &pTex);
		m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &bVertical);

		m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_0, &fVal1);
		m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_1, &fVal2);
		m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_0, &m_vCol[0]);
		m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_1, &m_vCol[1]);
		m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_2, &m_vCol[2]);

		GetMeshRender()->SetMaterial(m_pMtrl);

		if (m_fPrevReduced != m_fReduced)
		{
			if (true == CKeyManager::Manager().isServer())
			{
				PACKET_HP tPacket;
				tPacket.bPlayer = m_bis1P;
				tPacket.fDamage = m_fReduced;
				ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_HP_EX, (char*)&tPacket, sizeof(tPacket));
			}
		}

		m_fPrevReduced = m_fReduced;
	}
	
//#ifdef _DEBUG | DEBUG
	//if (CKeyManager::Manager().GetKeyState(KEY_F3, STATE_DOWN))
	//{
	//	m_fDepleted = 1.f;
	//	m_fReduced = 1.f;
	//}
//#endif
	//	GetTransform()->SetLocalScale(Vec3(m_fDepleted*vBeginScale.x, vBeginScale.y, vBeginScale.z));

	//GetTransform()->SetLocalPos(Vec3(m_fDepleted*vBeginPos.x / 2.f, vBeginPos.y, vBeginPos.z));
}

void CUIBar::LateUpdate()
{
	
}
