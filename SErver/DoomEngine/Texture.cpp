#include "Texture.h"
#include "PathManager.h"
#include "Device.h"

CTexture::CTexture()
	: m_pImage(NULL)
	, m_pTex2D(NULL)
	, m_pView(NULL)
{
}


CTexture::~CTexture()
{
	SAFE_RELEASE(m_pTex2D);
	SAFE_RELEASE(m_pView);
	SAFE_DELETE(m_pImage);
}

CTexture * CTexture::Create(const wstring & _strFilePath)
{
	wstring strExt = CPathManager::GetExt(_strFilePath.c_str());

	ScratchImage* pScratchImg = new ScratchImage;

	if (strExt == L".dds" || strExt == L".DDS")
	{
		if (FAILED(LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS_NONE, NULL, *pScratchImg)))
		{
			SAFE_DELETE(pScratchImg);
			return NULL;
		}
	}
	else if (strExt == L".tga" || strExt == L".TGA")
	{
		if (FAILED(LoadFromTGAFile(_strFilePath.c_str(), NULL, *pScratchImg)))
		{
			SAFE_DELETE(pScratchImg);
			return NULL;
		}
	}
	else
	{
		if (FAILED(LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS_NONE, NULL, *pScratchImg)))
		{
			SAFE_DELETE(pScratchImg);
			return NULL;
		}
	}

	ID3D11ShaderResourceView* pView = NULL;
	if (FAILED(CreateShaderResourceView(DEVICE
		, pScratchImg->GetImages()
		, pScratchImg->GetImageCount()
		, pScratchImg->GetMetadata()
		, &pView)))
	{
		SAFE_DELETE(pScratchImg);
		return NULL;
	}
	
	ID3D11Texture2D* pTex2D = NULL;
	pView->GetResource((ID3D11Resource**)(&pTex2D));

	CTexture* pTexture = new CTexture;
	pTexture->m_pImage = pScratchImg;
	pTexture->m_pTex2D = pTex2D;
	pTexture->m_pView = pView;

	return pTexture;
}

void CTexture::UpdateData()
{
}

void CTexture::UpdateData(UINT _iTiming, UINT _iRegister)
{
	if((UINT)SHADER_TYPE::ST_VERTEX & _iTiming)
		CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pView);

	if((UINT)SHADER_TYPE::ST_HULL & _iTiming)
		CONTEXT->HSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_DOMAIN & _iTiming)
		CONTEXT->DSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_GEOMETRY & _iTiming)
		CONTEXT->GSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_COMPUTE & _iTiming)
		CONTEXT->CSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_PIXEL & _iTiming)
		CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pView);
}

void CTexture::UpdateData(UINT _iTiming, UINT _iRegister) const
{
	if ((UINT)SHADER_TYPE::ST_VERTEX & _iTiming)
		CONTEXT->VSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_HULL & _iTiming)
		CONTEXT->HSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_DOMAIN & _iTiming)
		CONTEXT->DSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_GEOMETRY & _iTiming)
		CONTEXT->GSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_COMPUTE & _iTiming)
		CONTEXT->CSSetShaderResources(_iRegister, 1, &m_pView);

	if ((UINT)SHADER_TYPE::ST_PIXEL & _iTiming)
		CONTEXT->PSSetShaderResources(_iRegister, 1, &m_pView);
}
