#pragma once
#include "Res.h"

class CShader;

struct tShaderParam_EX : public tShaderParam
{
	void* pData;
};

class CMaterial :
	public CRes
{
private:
	CShader* m_pShader;
	vector<tShaderParam_EX> m_vecParam;

public:
	void SetShader(CShader* _pShader);
	void SetData(SHADER_PARAM _eParam, void* _pData);
	CShader* GetShader() { return m_pShader; }

private:
	void ClearShaderParam();
	void AdjustShaderParam();

public:
	virtual void UpdateData();

private:
	virtual void Save(FILE* _pFile);
	void Load(FILE* _pFile);

	void SaveShaderParam(FILE* _pFile);
	void LoadShaderParam(FILE* _pFile);

	CMaterial* Clone() { return new CMaterial(*this); }

public:
	CMaterial();
	CMaterial(const CMaterial& _other);
	virtual ~CMaterial();

	friend class CMeshRender;
	friend class CSaveLoadManager;
	friend class CResManager;
};

