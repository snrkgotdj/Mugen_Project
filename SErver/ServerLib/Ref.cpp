#include "Ref.h"

SERVER_USING

CRef::CRef()
{
}

CRef::~CRef()
{
}

int CRef::Release()
{
	--m_iRef;

	if (m_iRef == 0)
	{
		delete this;
		return 0;
	}

	return m_iRef;
}
