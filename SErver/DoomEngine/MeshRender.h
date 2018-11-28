#pragma once
#include "Component.h"

#include "ResPtr.h"

class CMesh;
class CShader;
class CTexture;
class CMaterial;

class CMeshRender :
	public CComponent
{
private:
	CResPtr<CMesh>	m_pMesh;
	CResPtr<CMaterial> m_pMtrl;
	RS_TYPE m_eRSType;

private:
	vector<tTextOutput> m_vecText;

public:
	virtual void Update() {}
	void Render();

public:
	RS_TYPE GetRSType() { return m_eRSType; }

	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	CMesh* GetMesh() { return m_pMesh; }

	void SetMaterial(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }

	CMaterial* GetMaterial();
	CMaterial* GetSharedMaterial() { return m_pMtrl; }

	void SetRSType(RS_TYPE _RSType) { m_eRSType = _RSType; }

public:
	void TextRender();
	void ModifyTextInfo(UINT _iIdx, tTextOutput _tText);
	void TextInfoPushBack(tTextOutput _tText) { m_vecText.push_back(_tText); }
	void TextInfoClear(){m_vecText.clear();}

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	virtual CMeshRender* Clone() { return new CMeshRender(*this); }

public:
	CMeshRender();
	virtual ~CMeshRender();
};

