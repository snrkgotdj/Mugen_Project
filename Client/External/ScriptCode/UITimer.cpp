#include "stdafx.h"
#include "UITimer.h"

#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "MeshRender.h"
#include "GameObject.h"
#include "ResManager.h"
#include "Material.h"
#include "Texture.h"


CUITimer::CUITimer()
	: m_bChanged(false)
	, m_bFirstCall(true)
	, m_fTime(1.f)
	, m_pOneObj(NULL)
	, m_pTenObj(NULL)
{
}


CUITimer::~CUITimer()
{
}

void CUITimer::Awake()
{	

}

void CUITimer::Start()
{
}

void CUITimer::Update()
{
	list<CGameObject*>::iterator iter = GetGameObject()->GetChildList().begin();
	list<CGameObject*>::iterator iterEnd = GetGameObject()->GetChildList().end();

	for (iter; iter != iterEnd; ++iter)
	{
		if ((*iter)->GetTagName() == L"01")
		{
			m_pOneObj = (*iter);
			if (m_bFirstCall)
			{
				int bVertical = 1;
				float fFull = 1.f;
				m_pMtrl = m_pOneObj->GetMeshRender()->GetMaterial();
				m_pMtrl->SetData(SHADER_PARAM::SP_INT_0, &bVertical);
				m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_0, &fFull);
				m_bFirstCall = false;
			}

			CResPtr<CTexture> pTex = m_pOneObj->GetAnimatorEX()->GetCurFrameInfo()->pTex;
			m_pMtrl->SetData(SHADER_PARAM::SP_TEX_1, &pTex);
			m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_1, &m_fTime);
			m_pOneObj->GetMeshRender()->SetMaterial(m_pMtrl);

		}
		else if ((*iter)->GetTagName() == L"10")
		{
			m_pTenObj = (*iter);
			//CResPtr<CTexture> pTex1 = m_pTenObj->GetAnimatorEX()->GetCurFrameInfo()->pTex;
			//m_pMtrl->SetData(SHADER_PARAM::SP_TEX_1, &pTex1);
			m_pMtrl->SetData(SHADER_PARAM::SP_FLOAT_1, &m_fTime);
			m_pTenObj->GetMeshRender()->SetMaterial(m_pMtrl);
		}
	}

	if (m_pOneObj->GetAnimatorEX()->GetCurAnim()->GetCurIndex() == 0)
	{
		if (NULL != m_pTenObj && !m_bChanged)
		{
			int iCurFrame = m_pTenObj->GetAnimatorEX()->GetCurAnim()->GetCurIndex();
			m_pTenObj->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(iCurFrame + 1);

			m_bChanged = true;
		}
	}
	else
	{
		m_bChanged = false;
	}

	m_fTime -= 0.01f*DT;
}

void CUITimer::LateUpdate()
{
}
