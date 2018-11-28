#pragma once

#include "global.h"

class CSampler;
class CBlendState;
class CDepthStencilState;

struct SAMPLER_INFO
{
	CSampler*	pSampler;
	UINT		iShaderTiming;
};

class CShader
{
private:
	wstring m_strKey;

	ID3DBlob* m_pVtxBlob;
	ID3DBlob* m_pPixelBlob;

	ID3DBlob* m_pVtxErrBlob;
	ID3DBlob* m_pPixelErrBlob;

	ID3D11VertexShader* m_pVtxShader;
	ID3D11PixelShader* m_pPixelShader;

	vector<SAMPLER_INFO> m_vecSampler;

	vector<tShaderParam> m_vecShaderParam;

	CBlendState* m_pBlendState;

	CDepthStencilState* m_pDepthStencilState;

public:
	int CreateVtxShader(const wstring& _strFilePath, const string& _strVSFuncName, UINT _iHigh, UINT _iRow);
	int createPixelShader(const wstring& _strFilePath, const string& _strPSFuncName, UINT _iHigh, UINT _iRow);

	ID3DBlob* GetVSBlob() { return m_pVtxBlob; }
	vector<tShaderParam>& GetShaderParam() { return m_vecShaderParam; }

	const wstring& GetKey() { return m_strKey; }
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }

	void AddSamplerState(CSampler* _pSampler, UINT _iTiming);
	void AddShaderParam(SHADER_PARAM _eShaderParam, UINT _iTiming);
	void SetBlendState(CBlendState* _pBlendState) { m_pBlendState = _pBlendState; }
	void SetDepthStencilState(CDepthStencilState* _pState) { m_pDepthStencilState = _pState; }
	void UpdateData();

public:
	CShader();
	~CShader();
};

