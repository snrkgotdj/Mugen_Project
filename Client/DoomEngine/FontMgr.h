#pragma once

#include "global.h"

#include "../External/FontWrapper/FW1FontWrapper.h"
#include "../External/FontWrapper/FW1CompileSettings.h"

#pragma comment(lib, "FW1FontWrapper")

class CFontMgr
{
public:
	static CFontMgr& Manager()
	{
		static CFontMgr inst;
		return inst;
	}

private:
	CFontMgr();
	~CFontMgr();

private:
	IFW1Factory*		m_pFactory;
	IFW1FontWrapper*	m_pFontWrapper;

public:
	void init();
	void DrawFont(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fSize, UINT _Color);
};

