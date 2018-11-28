#pragma once

#include "global.h"

class CDepthStencilState
{
private:
	ID3D11DepthStencilState* m_pState;
	D3D11_DEPTH_STENCIL_DESC m_tDesc;

public:
	static CDepthStencilState* Create(bool _bDepthEnable, D3D11_DEPTH_WRITE_MASK _eMask
		, D3D11_COMPARISON_FUNC _eFunc
		, bool _bstencilEnabel = false
		, D3D11_DEPTH_STENCILOP_DESC _tFront = D3D11_DEPTH_STENCILOP_DESC{}
		, D3D11_DEPTH_STENCILOP_DESC _tBack = D3D11_DEPTH_STENCILOP_DESC{}
		, UINT _iStencilReadMask = 0xffffffff, UINT _iStencilWriteMask = 0xffffffff);

	ID3D11DepthStencilState* GetState() { return m_pState; }

public:
	CDepthStencilState();
	~CDepthStencilState();
};

