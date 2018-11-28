#include "stdafx.h"
#include "UIFont.h"

#include "MeshRender.h"
#include "Material.h"
#include "Mesh.h"

#include "ResManager.h"

#define H_UV 1.f/13.f
#define V_UV 0.2f

CUIFont::CUIFont()
	: m_vUV(Vec4Zero)
	, m_cLetter('a')
{
	m_pMesh = (CMesh*)CResManager::Manager().Load<CMesh>(L"RectMesh");
	m_pMtrl = CResManager::Manager().FindMaterial(L"UIFontMtrl");

	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_0, &m_vUV);
}

CUIFont::CUIFont(char _cLetter)
	: m_vUV(Vec4Zero)
	, m_cLetter(_cLetter)
{
	m_pMesh = (CMesh*)CResManager::Manager().Load<CMesh>(L"RectMesh");
	m_pMtrl = CResManager::Manager().FindMaterial(L"UIFontMtrl");

	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_0, &m_vUV);
}


CUIFont::~CUIFont()
{
}

void CUIFont::Awake()
{

}

void CUIFont::Start()
{
}

void CUIFont::Update()
{
	int h;
	int v;

	switch (m_cLetter)
	{
	case 'A': case 'B':case 'C':case 'D':case 'E':case 'F':case 'G':case 'H':case 'I':case 'J':case 'K':case 'L':
	case 'M':case 'N':case 'O':case 'P':case 'Q':case 'R':case 'S':case 'T':case 'U':case 'V':case 'W':case 'X':
	case 'Y':case 'Z':
		h = ((int)m_cLetter - 65) % 13;
		v = ((int)m_cLetter - 65) / 13;
		m_vUV = Vec4(h*H_UV, v*V_UV, H_UV, V_UV);
		break;
	case 'a': case 'b':case 'c':case 'd':case 'e':case 'f':case 'g':case 'h':case 'i':case 'j':case 'k':case 'l':
	case 'm':case 'n':case 'o':case 'p':case 'q':case 'r':case 's':case 't':case 'u':case 'v':case 'w':case 'x':
	case 'y':case 'z':
		h = ((int)m_cLetter - 97) % 13;
		v = ((int)m_cLetter - 97) / 13 + 2;
		m_vUV = Vec4(h*H_UV, v*V_UV, H_UV, V_UV);
		break;
	case '0': case '1':case '2':case '3':case '4':case '5':case '6':case '7':case '8':case '9':
		h = ((int)m_cLetter - 48) % 13;
		m_vUV = Vec4(h*H_UV, 4 * V_UV, H_UV, V_UV);
		break;
	}

	m_pTex = (CTexture*)CResManager::Manager().FindTexture(L"UIFont");
	m_pMtrl = GetMeshRender()->GetSharedMaterial();

	m_pMtrl->SetData(SHADER_PARAM::SP_TEX_0, &m_pTex);
	m_pMtrl->SetData(SHADER_PARAM::SP_VEC4_0, &m_vUV);
}

void CUIFont::LateUpdate()
{
	m_pMesh->SetShader(m_pMtrl->GetShader());

	m_pMtrl->UpdateData();

	m_pMesh->UpdateData();

	m_pMesh->Render();
}
