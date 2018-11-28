#pragma once
#include "ServerEngine.h"

class CStream
{
private:
	char* m_pBuffer;
	unsigned int m_iSize;

public:
	void SetBuffer(char* _pBuffer);
	void SetOffset(int iOffset);
	unsigned int size() const { return m_iSize; }

public:
	template <typename T>
	bool ReadStream(T* _pData, unsigned int _iSize)
	{
		memcpy(_pData, m_pBuffer + m_iSize, _iSize);
		m_iSize += _iSize;

		return true;
	}

	template <typename T>
	bool WriteStream(T* _pData, unsigned int _iSize)
	{
		memcpy(m_pBuffer + m_iSize, _pData, _iSize);
		m_iSize += _iSize;

		return true;
	}

public:
	CStream();
	virtual ~CStream();
};

