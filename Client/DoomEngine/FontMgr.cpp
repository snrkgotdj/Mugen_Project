#include "FontMgr.h"

#include "Device.h"

CFontMgr::CFontMgr()
	: m_pFactory(NULL)
	, m_pFontWrapper(NULL)
{
}


CFontMgr::~CFontMgr()
{
	SAFE_RELEASE(m_pFactory);
	SAFE_RELEASE(m_pFontWrapper);
}

void CFontMgr::init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_pFactory)))
	{
		assert(NULL);
	}

	if (FAILED(m_pFactory->CreateFontWrapper(DEVICE, L"Arial", &m_pFontWrapper)))
	{
		assert(NULL);
	}
}

void CFontMgr::DrawFont(const wchar_t * _pStr, float _fPosX, float _fPosY, float _fSize, UINT _Color)
{
	m_pFontWrapper->DrawString(
		CONTEXT,
		_pStr,// String
		_fSize,// Font size
		_fPosX,// X position
		_fPosY,// Y position
		_Color,// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE// Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
