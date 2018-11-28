#include "Stream.h"


void CStream::SetBuffer(char * _pBuffer)
{
	m_pBuffer = _pBuffer;
	m_iSize = 0;
}

void CStream::SetOffset(int _iOffset)
{
	m_iSize = _iOffset;
}

CStream::CStream()
{
}


CStream::~CStream()
{
}
