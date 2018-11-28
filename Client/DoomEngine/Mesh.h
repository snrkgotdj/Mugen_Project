#pragma once
#include "Res.h"
#include "global.h"

class CShader;

class CMesh :
	public CRes
{
private:
	ID3D11Buffer*	m_pVB;
	UINT			m_iVtxCount;
	UINT			m_iVtxSize;
	D3D11_USAGE		m_eVtxUsage;

	ID3D11Buffer*	m_pIB;
	UINT			m_iIdxCount;
	UINT			m_iIdxSize;
	D3D11_USAGE		m_eIdxUsage;
	DXGI_FORMAT		m_eIdxFormat;

	D3D11_PRIMITIVE_TOPOLOGY			m_eTopology;
	ID3D11InputLayout*					m_pLayout;
	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecDesc;
	UINT								m_iDescOffset;
	CShader*								m_pRecentShader;

	void*	m_pVtxSysMem;
	void*	m_pIdxSysMem;

public:
	static CMesh* Create(UINT _iVtxCount, UINT _iVtxSize, D3D11_USAGE _eVtxUsage
		, void* _pVtxSysMem, UINT _iIdxCount, UINT _iIdxSize
		, D3D11_USAGE _eIdxUsage, void* _pIdxSysMem, DXGI_FORMAT _eIdxFormat
		, D3D11_PRIMITIVE_TOPOLOGY _eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	void AddDesc(char* _strSemantic, UINT _iSemanticIdx, DXGI_FORMAT _eFormat, UINT _iInputSlot, UINT _iInstancing);

	int SetShader(CShader* _pShader);
	virtual void UpdateData();

	void Render();

private:
	void SetVtxSysMem(void* _pVtxSysMem);
	void SetIdxSysMem(void* _pIdxSysMem);

public:
	CMesh();
	virtual ~CMesh();

	friend class CResPtr<CMesh>;
};

