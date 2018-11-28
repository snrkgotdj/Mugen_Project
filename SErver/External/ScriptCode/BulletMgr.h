#pragma once
#include "Single\SingletonScript.h"
class CBulletMgr :
	public CSingletonScript<CBulletMgr>
{
public:
	CBulletMgr();
	virtual ~CBulletMgr();

public:
	virtual void Awake();
	virtual void Update();

public:
	void LoadBullet(const wstring& _strFolder);
	void ActiveBullet(const wstring& _strBulletTag, int _iDir);

public:
	CLONE(CBulletMgr);
};

