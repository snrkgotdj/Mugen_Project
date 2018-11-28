#include "BlendState.h"

#include "Device.h"

CBlendState::CBlendState()
	: m_tDesc{}
	, m_bMRT(false)
{
}

CBlendState::~CBlendState()
{
	SAFE_RELEASE(m_pBlendState);
}

void CBlendState::AddTargetDesc(D3D11_RENDER_TARGET_BLEND_DESC _tBlendDesc, UINT _iTargetIdx)
{
	m_tDesc.RenderTarget[_iTargetIdx] = _tBlendDesc;
}

void CBlendState::CreateBlendState()
{
	m_tDesc.AlphaToCoverageEnable = false;
	m_tDesc.IndependentBlendEnable = m_bMRT;

	if (FAILED(DEVICE->CreateBlendState(&m_tDesc, &m_pBlendState)))
	{
		assert(NULL);
	}
}

