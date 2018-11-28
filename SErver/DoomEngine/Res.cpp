#include "stdafx.h"
#include "Res.h"

CRes::CRes()
	:m_iRefCnt(0)
{
}

CRes::CRes(const CRes & _other)
	: m_iRefCnt(0)
	, m_strKey(_other.m_strKey)
	, m_strPath(_other.m_strPath)
{
}

CRes::~CRes()
{
}

void CRes::Save(FILE * _pFile)
{
	SaveWString(m_strKey, _pFile);
	SaveWString(m_strPath, _pFile);
}
