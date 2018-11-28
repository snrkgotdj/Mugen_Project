#pragma once
#include "ResPtr.h"
#include "global.h"

class CRes
{
private:
	int m_iRefCnt;
	wstring m_strKey;
	wstring m_strPath;

public:
	void AddRef() { ++m_iRefCnt; }
	void SubRef() {	--m_iRefCnt; }
	int GetRefCount() { return m_iRefCnt; }

public:
	virtual void UpdateData() {};

public:
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	void SetPath(const wstring& _strPath) { m_strPath = _strPath; }
	const wstring& GetKey() { return m_strKey; }
	const wstring& GetPath() { return m_strPath; }

public:
	virtual void Save(FILE* _pFile);

public:
	CRes();
	CRes(const CRes& _other);
	virtual ~CRes();
};

