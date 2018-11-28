#pragma once
#include "Single/SingletonScript.h"
class CModeMgr :
	public CSingletonScript<CModeMgr>
{
private:
	UINT m_iMode;
	bool m_bOnline;

public:
	CModeMgr();
	virtual ~CModeMgr();

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update();
	virtual void LateUpdate() {}

public:
	void SetMode(UINT _iMode);
	UINT GetMode() { return m_iMode; }
	void SetOnline(bool _bTrue) { m_bOnline = _bTrue; }
	bool GetOnline() { return m_bOnline; }

	CLONE(CModeMgr);
};

