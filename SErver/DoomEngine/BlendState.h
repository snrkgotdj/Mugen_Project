#pragma once

#include "global.h"

class CBlendState
{
private:
	ID3D11BlendState* m_pBlendState;
	D3D11_BLEND_DESC m_tDesc;
	bool m_bMRT;

public:
	void AddTargetDesc(D3D11_RENDER_TARGET_BLEND_DESC _tBlendDesc, UINT _iTargetIdx);
	void CreateBlendState();
	void SetMRT(bool _bTrue) { m_bMRT = _bTrue; }

public:
	ID3D11BlendState* GetState() { return m_pBlendState; }

public:
	CBlendState();
	~CBlendState();
};

