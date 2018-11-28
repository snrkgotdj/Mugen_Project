#include "stdafx.h"
#include "ModeMgr.h"


CModeMgr::CModeMgr()
	: m_iMode(1)
{
}


CModeMgr::~CModeMgr()
{
}

void CModeMgr::Update()
{
	if (m_bOnline)
		m_iMode = 3;
	//if (m_iMode == 0)
	//	m_iMode = 1;
}

void CModeMgr::SetMode(UINT _iMode)
{
	m_iMode = _iMode;
}
