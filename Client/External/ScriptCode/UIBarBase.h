#pragma once
#include "Script.h"

class CTexture;
class CMaterial;

class CUIBarBase :
	public Script
{
private:
	CMesh* m_pMesh;
	CMaterial* m_pMtrl;

	CResPtr<CTexture> m_pTex;

	Vec3 vBeginPos;
	Vec3 vBeginScale;

	bool m_bCall;

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	CUIBarBase();
	virtual ~CUIBarBase();

	CLONE(CUIBarBase);
};

