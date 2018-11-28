#pragma once
#include "Script.h"
class CUIFont :
	public Script
{
private:
	CResPtr<CMesh> m_pMesh;
	CResPtr<CMaterial> m_pMtrl;

	CTexture* m_pTex;

	char m_cLetter;

	Vec4 m_vUV;

public:
	CUIFont();
	CUIFont(char _cLetter);
	virtual ~CUIFont();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUIFont);

public:
	void ChangeLetter(char _c) { m_cLetter = _c; }
};

