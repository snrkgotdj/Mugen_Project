#pragma once
#include "Script.h"
class CUIGuardBar :
	public Script
{
public:
	CUIGuardBar();
	virtual ~CUIGuardBar();

private:
	CResPtr<CMesh> m_pMesh;
	CResPtr<CMaterial> m_pMtrl;

	CResPtr<CTexture> m_pTex;
	float m_fDepleted;
	float m_fReduced;
	float m_fPrevReduced;

	bool m_bCall;
	float m_fTerm;

	Vec4 m_vCol[3];
	bool m_bis1P;

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

public:
	float GetValue() { return m_fReduced; }
	void SetValue(float _fVal) { m_fReduced = _fVal; }//0~1·Î Á¶Á¤
	void SetPlayer(bool _is1P) { m_bis1P = _is1P; }

	CLONE(CUIGuardBar);
};

