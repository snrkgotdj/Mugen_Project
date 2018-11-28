#pragma once
#include "Single\SingletonScript.h"

class CEffectManager :
	public CSingletonScript<CEffectManager>
{
private:
	map<wstring, vector<CGameObject*>> m_mapEffect;

private:
	int m_iCallCnt;

public:
	void ActiveEffect(const wstring& _strEffect, const Vec3& _vecPos, bool _bContinue = false);
	void ActiveEffect(const wstring& _strEffect, const wstring& _strLayer, 
		const Vec3& _vecPos, bool _bContinue = false);
	void ActiveEffect(const wstring& _strEffect, const Vec3& _vecPos
		, const Vec3& _vScale, bool _bContinue = false);
	void LoadEffect(const wstring& _strEffectFolder);
	void StopEffect(const wstring& _strEffect);
	void FollowPos(const wstring& _strEffect, const Vec3& _vecPos, int _Dir = 1);
	void FollowPos(const wstring& _strEffect, const Vec3& _vecPos, const Vec3& _vScale, int _Dir = 1);

private:
	vector<CGameObject*>& FindEffect(const wstring& _strEffect);

public:
	virtual void Awake();
	virtual void Update();

public:
	CLONE(CEffectManager);

public:
	CEffectManager();
	virtual ~CEffectManager();
};

