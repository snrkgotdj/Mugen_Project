#pragma once
#include "Script.h"

#include "..\Doom\define_client.h"

class CCameraScript :
	public Script
{
private:
	CGameObject* m_pGameObject_1P;
	CGameObject* m_pGameObject_2P;

private:
	Vec3 m_vecMapPos;
	Vec3 m_vecMapScale;

private:
	float m_iMaxScale;

private:
	float m_LimitCamScale;
	float m_LimitX;
	float m_LimitY;
	float m_StartChangeScale;

private:
	float m_fVibTime;
	float m_fVibTimeLimit;
	float m_fVibRange;

public:
	const Vec3& GetMapScale() const { return m_vecMapScale; }
	const Vec3& GetMapPos() const { return m_vecMapPos; }

public:
	void Set1P(CGameObject* _p1P) { m_pGameObject_1P = _p1P; }
	void Set2P(CGameObject* _p2P) { m_pGameObject_2P = _p2P; }
	void SetMapPos(const Vec3& _mapPos) { m_vecMapPos = _mapPos; }
	void SetMaxScale(const float _iScale) { m_iMaxScale = _iScale; }
	void SetMapScale(const Vec3& _mapScale) { m_vecMapScale = _mapScale; }

	void SetVibRange(float _fRange) { m_fVibRange = _fRange; }
	void SetVibTimeLimit(float _fLim) { m_fVibTimeLimit = _fLim; }

public:
	void Vibration(float _fRange, float _fTime);

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CCameraScript);

public:
	CCameraScript();
	virtual ~CCameraScript();
};

