#pragma once
#include "Res.h"

template<typename T>

class CResPtr
{
private:
	T* m_pTarget;

public:
	operator T* () { return m_pTarget; }
	T* operator ->() { return m_pTarget; }
	const T* operator ->() const { return m_pTarget; }
	void operator = (T* _pTarget);

	void Delete();

public:
	CResPtr();
	CResPtr(T* _pTarget);
	CResPtr(const CResPtr& _Other);
	~CResPtr();
};

