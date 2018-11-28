#pragma once

#include "ServerEngine.h"

SERVER_BEGIN

class CRef
{
protected:
	CRef();
	virtual ~CRef() = 0;

protected:
	int m_iRef;
	string m_strTag;

public:
	void SetTagName(const string& _strTag) { m_strTag = _strTag; }
	string GetTagName() const { return m_strTag; }

public:
	void AddRef() { ++m_iRef; }
	int Release();
};

SERVER_END