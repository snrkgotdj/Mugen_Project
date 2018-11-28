#pragma once
#include "Res.h"


#include "..\External\DirectXTex.h"
#pragma comment (lib, "DirectXTex")


class CTexture :
	public CRes
{
private:
	ScratchImage*				m_pImage;
	ID3D11Texture2D*			m_pTex2D;
	ID3D11ShaderResourceView*	m_pView;

public:
	static CTexture* Create(const wstring& _strFilePath);

public:
	virtual void UpdateData();
	void UpdateData(UINT _iTiming, UINT _iRegister);
	void UpdateData(UINT _iTiming, UINT _iRegister) const;

public:
	float GetWith() { return (float)m_pImage->GetMetadata().width; }
	float GEtHeight() { return (float)m_pImage->GetMetadata().height; }

public:
	CTexture();
	virtual ~CTexture();
};

