#include "ShaderManager.h"

#include "PathManager.h"
#include "Device.h"
#include "Shader.h"
#include "Sampler.h"
#include "BlendState.h"
#include "DepthStencilState.h"

CShaderManager::CShaderManager()
{
}


CShaderManager::~CShaderManager()
{
	Safe_Delete_Map(m_mapShader);
	Safe_Delete_Map(m_mapSampler);
	Safe_Delete_Map(m_mapBlendState);
	Safe_Delete_Map(m_mapDepthStencilState);
}


void CShaderManager::init()
{
	CreateSampler();

	CreateBlendState();

	CreateDepthStencilState();

	CreateShader();
}

CShader * CShaderManager::FindShader(const wstring & _strKey)
{
	map<wstring, CShader*>::iterator iter = m_mapShader.find(_strKey);

	if (iter == m_mapShader.end())
	{
		return NULL;
	}
	return iter->second;
}

CSampler * CShaderManager::FindSampler(const wstring & _strKey)
{
	map<wstring, CSampler*>::iterator iter = m_mapSampler.find(_strKey);

	if (iter == m_mapSampler.end())
		return NULL;

	return iter->second;
}

CBlendState * CShaderManager::FindBlendState(const wstring & _strKey)
{
	map<wstring, CBlendState*>::iterator iter = m_mapBlendState.find(_strKey);

	if (iter == m_mapBlendState.end())
		return NULL;

	return iter->second;
}

CDepthStencilState * CShaderManager::FindDepthStencilState(const wstring & _strKey)
{
	map<wstring, CDepthStencilState*>::iterator iter = m_mapDepthStencilState.find(_strKey);

	if (iter == m_mapDepthStencilState.end())
		return NULL;

	return iter->second;
}

void CShaderManager::AddShader(const wstring & _strKey, CShader * _pShader)
{
	CShader* pShader = FindShader(_strKey);

	if (NULL != pShader)
	{
		SAFE_DELETE(_pShader);
		return;
	}

	_pShader->SetKey(_strKey);
	m_mapShader.insert(make_pair(_strKey, _pShader));
}

void CShaderManager::AddSampler(const wstring & _strKey, CSampler * _pSampler)
{
	CSampler* pSampler = FindSampler(_strKey);

	if (NULL != pSampler)
	{
		SAFE_DELETE(pSampler);
		return;
	}

	m_mapSampler.insert(make_pair(_strKey, _pSampler));
}

void CShaderManager::AddBlendState(const wstring & _strKey, CBlendState * _pState)
{
	CBlendState* pState = FindBlendState(_strKey);

	if (NULL != pState)
	{
		SAFE_DELETE(_pState);
		return;
	}

	m_mapBlendState.insert(make_pair(_strKey, _pState));
}

void CShaderManager::AddDepthStencilState(const wstring & _strKey, CDepthStencilState * _pState)
{
	CDepthStencilState* pState = FindDepthStencilState(_strKey);

	if (NULL != pState)
	{
		SAFE_DELETE(_pState);
		return;
	}

	m_mapDepthStencilState.insert(make_pair(_strKey, _pState));
}

void CShaderManager::CreateSampler()
{
	CSampler* pSampler = CSampler::Create(D3D11_FILTER_ANISOTROPIC, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, 0);
	AddSampler(L"Sampler_Default", pSampler);
}

void CShaderManager::CreateShader()
{
	// Color Shader
	CreateColorShader();
	
	// Texture Shader
	CreateTextureShader();

	//Collider_2D shader
	CreateColider2DShader();

	//Std_2D Shader
	CreateStd2DShader();

	//UI Shader
	CreateUIShader();
	
	//Gray Shader
	CreateGrayShader();
	CreateRedShader();
	CreateBlueShader();
	CreateBlackShader();
	CreateEffectShader();
	CreateReverseShader();
	CreateAllRedShader();
	CreateAllBlackShader();
}

void CShaderManager::CreateBlendState()
{
	CBlendState* pState = new CBlendState;
	pState->SetMRT(false);

	D3D11_RENDER_TARGET_BLEND_DESC tDesc = {};

	tDesc.BlendEnable = true;
	tDesc.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	tDesc.BlendOp = D3D11_BLEND_OP_ADD;
	tDesc.SrcBlend = D3D11_BLEND_SRC_ALPHA;
	tDesc.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	tDesc.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	tDesc.SrcBlendAlpha = D3D11_BLEND_ONE;
	tDesc.DestBlendAlpha = D3D11_BLEND_ZERO;

	pState->AddTargetDesc(tDesc, 0);

	pState->CreateBlendState();

	AddBlendState(L"AlphaBlend", pState);
}

void CShaderManager::CreateDepthStencilState()
{
	CDepthStencilState* pState = CDepthStencilState::Create(true, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS_EQUAL);

	if (NULL != pState)
		AddDepthStencilState(L"LESS_EQUAL", pState);
}

void CShaderManager::CreateColorShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_ColorVtx", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_ColorVtx", 5, 0);
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	AddShader(L"Shader_Color", pShader);
}

void CShaderManager::CreateTextureShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_TexVtx", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_TexVtx", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_INT_0, (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Texture", pShader);

	strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_TexVtx", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_TexVtx", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_INT_0, (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->AddShaderParam(SHADER_PARAM::SP_FLOAT_0, (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->AddShaderParam(SHADER_PARAM::SP_FLOAT_1, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Texture_EX", pShader);
}

void CShaderManager::CreateStd2DShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D", pShader);
}

void CShaderManager::CreateColider2DShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Collider", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Collider", 5, 0);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));
	pShader->AddShaderParam(SHADER_PARAM::SP_INT_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"ColliderShader", pShader);
}

void CShaderManager::CreateUIShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_UIBar", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_UIBar", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_1, (UINT)SHADER_TYPE::ST_PIXEL);//텍스처
	pShader->AddShaderParam(SHADER_PARAM::SP_FLOAT_0, (UINT)SHADER_TYPE::ST_PIXEL);//기준점1
	pShader->AddShaderParam(SHADER_PARAM::SP_FLOAT_1, (UINT)SHADER_TYPE::ST_PIXEL);//기준점2
	pShader->AddShaderParam(SHADER_PARAM::SP_INT_0, (UINT)SHADER_TYPE::ST_PIXEL);//가로or세로
	pShader->AddShaderParam(SHADER_PARAM::SP_VEC4_0, (UINT)SHADER_TYPE::ST_PIXEL);//기준점1
	pShader->AddShaderParam(SHADER_PARAM::SP_VEC4_1, (UINT)SHADER_TYPE::ST_PIXEL);//기준점1
	pShader->AddShaderParam(SHADER_PARAM::SP_VEC4_2, (UINT)SHADER_TYPE::ST_PIXEL);//기준점1

	AddShader(L"Shader_UIBar", pShader);

	pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_UIFont", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_UIFont", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);//텍스처
	pShader->AddShaderParam(SHADER_PARAM::SP_VEC4_0, (UINT)SHADER_TYPE::ST_PIXEL);//기준점1

	AddShader(L"Shader_UIFont", pShader);
}

void CShaderManager::CreateGrayShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Gray", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Gray", pShader);
}

void CShaderManager::CreateRedShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Red", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Red", pShader);
}

void CShaderManager::CreateBlueShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Blue", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Blue", pShader);
}

void CShaderManager::CreateBlackShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Black", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Black", pShader);
}

void CShaderManager::CreateEffectShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Effect", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Effect", pShader);
}

void CShaderManager::CreateReverseShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_Reverse", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_Reverse", pShader);
}

void CShaderManager::CreateAllRedShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_AllRed", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_AllRed", pShader);
}

void CShaderManager::CreateAllBlackShader()
{
	wstring strFilePath = CPathManager::GetResPath();
	strFilePath += L"\\Shader\\Shader.fx";

	CShader* pShader = new CShader;
	pShader->CreateVtxShader(strFilePath, "VS_Std_2D", 5, 0);
	pShader->createPixelShader(strFilePath, "PS_Std_2D_AllBlack", 5, 0);
	pShader->AddSamplerState(FindSampler(L"Sampler_Default"), (UINT)SHADER_TYPE::ST_PIXEL);
	pShader->SetBlendState(FindBlendState(L"AlphaBlend"));
	pShader->SetDepthStencilState(FindDepthStencilState(L"LESS_EQUAL"));

	pShader->AddShaderParam(SHADER_PARAM::SP_TEX_0, (UINT)SHADER_TYPE::ST_PIXEL);

	AddShader(L"Shader_Std2D_AllBlack", pShader);
}

