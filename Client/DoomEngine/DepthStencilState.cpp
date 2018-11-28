#include "DepthStencilState.h"
#include "Device.h"

CDepthStencilState::CDepthStencilState()
{
}


CDepthStencilState::~CDepthStencilState()
{
	SAFE_RELEASE(m_pState);
}

CDepthStencilState * CDepthStencilState::Create(bool _bDepthEnable
	, D3D11_DEPTH_WRITE_MASK _eMask
	, D3D11_COMPARISON_FUNC _eFunc
	, bool _bstencilEnabel
	, D3D11_DEPTH_STENCILOP_DESC _tFront
	, D3D11_DEPTH_STENCILOP_DESC _tBack
	, UINT _iStencilReadMask
	, UINT _iStencilWriteMask)
{
	D3D11_DEPTH_STENCIL_DESC tDesc = {};

	tDesc.DepthEnable = _bDepthEnable;
	tDesc.DepthWriteMask = _eMask;
	tDesc.DepthFunc = _eFunc;
	tDesc.StencilEnable = _bstencilEnabel;
	tDesc.FrontFace = _tFront;
	tDesc.BackFace = _tBack;
	tDesc.StencilReadMask = _iStencilReadMask;
	tDesc.StencilWriteMask = _iStencilWriteMask;

	ID3D11DepthStencilState* pState = NULL;
	
	if(FAILED(DEVICE->CreateDepthStencilState(&tDesc, &pState)))
	{
		return NULL;
	}

	CDepthStencilState* pDepthStencilState = new CDepthStencilState;

	pDepthStencilState->m_tDesc = tDesc;
	pDepthStencilState->m_pState = pState;

	return pDepthStencilState;
}

