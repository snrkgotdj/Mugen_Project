#include "Shader.h"

#include "Device.h"
#include "Sampler.h"
#include "BlendState.h"
#include "DepthStencilState.h"

CShader::CShader()
	:m_pVtxBlob(NULL)
	, m_pPixelBlob(NULL)
	, m_pVtxErrBlob(NULL)
	, m_pPixelErrBlob(NULL)
	, m_pVtxShader(NULL)
	, m_pPixelShader(NULL)
	, m_pDepthStencilState(NULL)
{
}


CShader::~CShader()
{
	SAFE_RELEASE(m_pVtxBlob);
	SAFE_RELEASE(m_pPixelBlob);
	SAFE_RELEASE(m_pVtxErrBlob);
	SAFE_RELEASE(m_pPixelErrBlob);
	SAFE_RELEASE(m_pVtxShader);
	SAFE_RELEASE(m_pPixelShader);
}

int CShader::CreateVtxShader(const wstring & _strFilePath, const string & _strVSFuncName, UINT _iHigh, UINT _iRow)
{
	UINT iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif

	char szBuffer[255] = {};
	sprintf_s(szBuffer, "vs_%d_%d", _iHigh, _iRow);

	if (FAILED(D3DCompileFromFile(_strFilePath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strVSFuncName.c_str(), szBuffer, iFlag, 0, &m_pVtxBlob, &m_pVtxErrBlob)))
	{
		return RET_FAIL;
	}

	if (FAILED(DEVICE->CreateVertexShader(m_pVtxBlob->GetBufferPointer(), m_pVtxBlob->GetBufferSize()
		, NULL, &m_pVtxShader)))
	{
		return RET_FAIL;
	}

	return RET_SUCCESS;
}

int CShader::createPixelShader(const wstring & _strFilePath, const string & _strPSFuncName, UINT _iHigh, UINT _iRow)
{
	UINT iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D10_SHADER_DEBUG;
#endif // _DEBUG

	char szBuffer[255] = {};
	sprintf_s(szBuffer, "ps_%d_%d", _iHigh, _iRow);

	if (FAILED(D3DCompileFromFile(_strFilePath.c_str(), NULL, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strPSFuncName.c_str(), szBuffer, iFlag, 0, &m_pPixelBlob, &m_pPixelBlob)))
	{
		return RET_FAIL;
	}

	if (FAILED(DEVICE->CreatePixelShader(m_pPixelBlob->GetBufferPointer(), m_pPixelBlob->GetBufferSize()
		, NULL, &m_pPixelShader)))
	{
		return RET_FAIL;
	}

	return RET_SUCCESS;
}

void CShader::AddSamplerState(CSampler * _pSampler, UINT _iTiming)
{
	m_vecSampler.push_back(SAMPLER_INFO{ _pSampler, _iTiming });
}

void CShader::AddShaderParam(SHADER_PARAM _eShaderParam, UINT _iTiming)
{
	tShaderParam param = { _eShaderParam, g_arrSPRegister[(UINT)_eShaderParam], _iTiming };
	m_vecShaderParam.push_back(param);
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVtxShader, NULL, NULL);
	CONTEXT->PSSetShader(m_pPixelShader, NULL, NULL);

	for (UINT i = 0; i < m_vecSampler.size(); ++i)
	{
		m_vecSampler[i].pSampler->UpdateDate(m_vecSampler[i].iShaderTiming);
	}

	if (NULL != m_pBlendState)
	{
		ID3D11BlendState* pState = m_pBlendState->GetState();
		CONTEXT->OMSetBlendState(pState, NULL, 0xffffffff);
	}
	else
	{
		CONTEXT->OMSetBlendState(NULL, NULL, 0xffffffff);
	}

	if (NULL != m_pDepthStencilState)
	{
		ID3D11DepthStencilState* pState = m_pDepthStencilState->GetState();
		CONTEXT->OMSetDepthStencilState(pState, 0);
	}

	else
	{
		CONTEXT->OMSetDepthStencilState(NULL, 0);
	}

}
