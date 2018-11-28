#include "Material.h"
#include "ResManager.h"
#include "ShaderManager.h"

#include "Device.h"
#include "Texture.h"
#include "Shader.h"

UINT g_arrSPRegister[(UINT)SHADER_PARAM::END]=
{
	1,//SP_INT_0,
	1,//SP_INT_1,
	1,//SP_INT_2,
	1,//SP_INT_3,
	1,//SP_INT_END,
	2,//SP_FLOAT_0,
	2,//SP_FLOAT_1,
	2,//SP_FLOAT_2,
	2,//SP_FLOAT_3,
	2,//SP_FLOAT_END,
	3,//SP_VEC4_0,
	3,//SP_VEC4_1,
	3,//SP_VEC4_2,
	3,//SP_VEC4_3,
	3,//SP_VEC4_END,
	0,//SP_TEX_0,
	1,//SP_TEX_1,
	2,//SP_TEX_2,
	3,//SP_TEX_3,
	0,//SP_TEX_END,	
};

wchar_t* g_arrSPName[(UINT)SHADER_PARAM::END] =
{
	L"SP_INT_0"		,
	L"SP_INT_1"		,
	L"SP_INT_2"		,
	L"SP_INT_3"		,
	L"SP_INT_END"	,
	L"SP_FLOAT_0"	,
	L"SP_FLOAT_1"	,
	L"SP_FLOAT_2"	,
	L"SP_FLOAT_3"	,
	L"SP_FLOAT_END"	,
	L"SP_VEC4_0"	,
	L"SP_VEC4_1"	,
	L"SP_VEC4_2"	,
	L"SP_VEC4_3"	,
	L"SP_VEC4_END"	,
	L"SP_TEX_0"		,
	L"SP_TEX_1"		,
	L"SP_TEX_2"		,
	L"SP_TEX_3"		,
	L"SP_TEX_END"	,
};

CMaterial::CMaterial()
	:m_pShader(NULL)
{
}

CMaterial::CMaterial(const CMaterial & _other)
	:m_pShader(_other.m_pShader)
	, m_vecParam(_other.m_vecParam)
	, CRes(_other)
{
	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		switch (m_vecParam[i].eParam)
		{
		case SHADER_PARAM::SP_INT_0:
		case SHADER_PARAM::SP_INT_1:
		case SHADER_PARAM::SP_INT_2:
		case SHADER_PARAM::SP_INT_3:
			m_vecParam[i].pData = new int;
			memcpy(m_vecParam[i].pData, _other.m_vecParam[i].pData, sizeof(int));
			break;
		case SHADER_PARAM::SP_FLOAT_0:
		case SHADER_PARAM::SP_FLOAT_1:
		case SHADER_PARAM::SP_FLOAT_2:
		case SHADER_PARAM::SP_FLOAT_3:
			m_vecParam[i].pData = new float;
			memcpy(m_vecParam[i].pData, _other.m_vecParam[i].pData, sizeof(float));
			break;
		case SHADER_PARAM::SP_VEC4_0:
		case SHADER_PARAM::SP_VEC4_1:
		case SHADER_PARAM::SP_VEC4_2:
		case SHADER_PARAM::SP_VEC4_3:
			m_vecParam[i].pData = new Vec4;
			memcpy(m_vecParam[i].pData, _other.m_vecParam[i].pData, sizeof(Vec4));
			break;
		case SHADER_PARAM::SP_TEX_0:
		case SHADER_PARAM::SP_TEX_1:
		case SHADER_PARAM::SP_TEX_2:
		case SHADER_PARAM::SP_TEX_3:
			m_vecParam[i].pData = new void*;
			memcpy(m_vecParam[i].pData, _other.m_vecParam[i].pData, sizeof(void*));
			break;
		default:
			continue;
		}
	}
}

CMaterial::~CMaterial()
{
	ClearShaderParam();
}

void CMaterial::SetShader(CShader * _pShader)
{
	if (m_pShader == _pShader)
		return;

	m_pShader = _pShader;
	ClearShaderParam();
	AdjustShaderParam();
}

void CMaterial::ClearShaderParam()
{
	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		SAFE_DELETE(m_vecParam[i].pData);
	}
	m_vecParam.clear();
}

void CMaterial::AdjustShaderParam()
{
	vector<tShaderParam> & vecParam = m_pShader->GetShaderParam();

	for (UINT i = 0; i < vecParam.size(); ++i)
	{
		tShaderParam_EX param;
		param.eParam = vecParam[i].eParam;
		param.iRegister = vecParam[i].iRegister;
		param.iTiming = vecParam[i].iTiming;

		switch (vecParam[i].eParam)
		{
		case SHADER_PARAM::SP_INT_0:
		case SHADER_PARAM::SP_INT_1:
		case SHADER_PARAM::SP_INT_2:
		case SHADER_PARAM::SP_INT_3:
			param.pData = new int;
			*((int*)param.pData) = 0;
			break;
		case SHADER_PARAM::SP_FLOAT_0:
		case SHADER_PARAM::SP_FLOAT_1:
		case SHADER_PARAM::SP_FLOAT_2:
		case SHADER_PARAM::SP_FLOAT_3:
			param.pData = new float;
			*((float*)param.pData) = 0.f;
			break;
		case SHADER_PARAM::SP_VEC4_0:
		case SHADER_PARAM::SP_VEC4_1:
		case SHADER_PARAM::SP_VEC4_2:
		case SHADER_PARAM::SP_VEC4_3:
			param.pData = new Vec4;
			*((Vec4*)param.pData) = Vec4(0.f, 0.f, 0.f, 0.f);
			break;
		case SHADER_PARAM::SP_TEX_0:
		case SHADER_PARAM::SP_TEX_1:
		case SHADER_PARAM::SP_TEX_2:
		case SHADER_PARAM::SP_TEX_3:
			param.pData = new void*;
			*((int*)param.pData) = NULL;
			break;
		default:
			continue;
		}
		m_vecParam.push_back(param);
	}

}

void CMaterial::UpdateData()
{
	if (NULL == m_pShader)
		assert(NULL);

	m_pShader->UpdateData();

	int iArr[(UINT)SHADER_PARAM::SP_INT_END - (UINT)SHADER_PARAM::SP_INT_0] = {};
	float fArr[(UINT)SHADER_PARAM::SP_FLOAT_END - (UINT)SHADER_PARAM::SP_FLOAT_0] = {};
	Vec4 vecArr[(UINT)SHADER_PARAM::SP_VEC4_END - (UINT)SHADER_PARAM::SP_VEC4_0] = {};

	void* Arr[3] = { iArr, fArr, vecArr };

	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		switch (m_vecParam[i].eParam)
		{
		case SHADER_PARAM::SP_INT_0:
		case SHADER_PARAM::SP_INT_1:
		case SHADER_PARAM::SP_INT_2:
		case SHADER_PARAM::SP_INT_3:
			iArr[(UINT)m_vecParam[i].eParam - (UINT)SHADER_PARAM::SP_INT_0] = *((int*)m_vecParam[i].pData);
		break;
		case SHADER_PARAM::SP_FLOAT_0:
		case SHADER_PARAM::SP_FLOAT_1:
		case SHADER_PARAM::SP_FLOAT_2:
		case SHADER_PARAM::SP_FLOAT_3:
			fArr[(UINT)m_vecParam[i].eParam - (UINT)SHADER_PARAM::SP_FLOAT_0] = *((float*)m_vecParam[i].pData);
			break;
		case SHADER_PARAM::SP_VEC4_0:
		case SHADER_PARAM::SP_VEC4_1:
		case SHADER_PARAM::SP_VEC4_2:
		case SHADER_PARAM::SP_VEC4_3:
			vecArr[(UINT)m_vecParam[i].eParam - (UINT)SHADER_PARAM::SP_VEC4_0] = *((Vec4*)m_vecParam[i].pData);
			break;
		case SHADER_PARAM::SP_TEX_0:
		case SHADER_PARAM::SP_TEX_1:
		case SHADER_PARAM::SP_TEX_2:
		case SHADER_PARAM::SP_TEX_3:
			if (NULL != *((CTexture**)m_vecParam[i].pData))
			{
				(*((CTexture**)m_vecParam[i].pData))->UpdateData(m_vecParam[i].iTiming, m_vecParam[i].iRegister);
			}
			break;
		default:
			continue;
		}
	}

	
	static const BUFFER* pArrConstBuffer[3] = {
		CDevice::GetInst().FindConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_INT_END]),
		CDevice::GetInst().FindConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_FLOAT_END]),
		CDevice::GetInst().FindConstBuffer(g_arrSPName[(UINT)SHADER_PARAM::SP_VEC4_END])
	};

	for (int i = 0; i < 3; ++i)
	{
		D3D11_MAPPED_SUBRESOURCE tSub = {};
		CONTEXT->Map(pArrConstBuffer[i]->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
		memcpy(tSub.pData, Arr[i], pArrConstBuffer[i]->iSize);
		CONTEXT->Unmap(pArrConstBuffer[i]->pConstBuffer, 0);

		CONTEXT->VSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
		CONTEXT->HSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
		CONTEXT->DSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
		CONTEXT->GSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
		CONTEXT->CSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
		CONTEXT->PSSetConstantBuffers(pArrConstBuffer[i]->iRegister, 1, &pArrConstBuffer[i]->pConstBuffer);
	}
}

void CMaterial::Save(FILE * _pFile)
{
	CRes::Save(_pFile);

	BYTE bShader = 0;
	if (m_pShader)
		bShader = 1;

	fwrite(&bShader, 1, 1, _pFile);

	if (m_pShader)
		SaveWString(m_pShader->GetKey(), _pFile);

	SaveShaderParam(_pFile);
}

void CMaterial::Load(FILE * _pFile)
{
	BYTE bShader = 0;
	fread(&bShader, 1, 1, _pFile);

	if (bShader)
	{
		wstring strShaderKey = LoadWString(_pFile);
		SetShader(CShaderManager::Manager().FindShader(strShaderKey));
	}
	LoadShaderParam(_pFile);
}

void CMaterial::SaveShaderParam(FILE * _pFile)
{
	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		switch (m_vecParam[i].eParam)
		{
		case SHADER_PARAM::SP_INT_0:
		case SHADER_PARAM::SP_INT_1:
		case SHADER_PARAM::SP_INT_2:
		case SHADER_PARAM::SP_INT_3:
		{
			fwrite(m_vecParam[i].pData, sizeof(UINT), 1, _pFile);
		}
		break;
		case SHADER_PARAM::SP_FLOAT_0:
		case SHADER_PARAM::SP_FLOAT_1:
		case SHADER_PARAM::SP_FLOAT_2:
		case SHADER_PARAM::SP_FLOAT_3:
			fwrite(m_vecParam[i].pData, sizeof(float), 1, _pFile);
			break;
		case SHADER_PARAM::SP_VEC4_0:
		case SHADER_PARAM::SP_VEC4_1:
		case SHADER_PARAM::SP_VEC4_2:
		case SHADER_PARAM::SP_VEC4_3:
			fwrite(m_vecParam[i].pData, sizeof(Vec4), 1, _pFile);
			break;
		case SHADER_PARAM::SP_TEX_0:
		case SHADER_PARAM::SP_TEX_1:
		case SHADER_PARAM::SP_TEX_2:
		case SHADER_PARAM::SP_TEX_3:
		{
			CTexture* pTex = *((CTexture**)(m_vecParam[i].pData));

			BYTE has = 0;
			if (NULL != pTex)
				has = 1;
			fwrite(&has, 1, 1, _pFile);

			if (has)
				SaveWString(pTex->GetKey(), _pFile);
		}
			break;
		}
	}
}

void CMaterial::LoadShaderParam(FILE * _pFile)
{
	for (UINT i = 0; i < m_vecParam.size(); ++i)
	{
		switch (m_vecParam[i].eParam)
		{
		case SHADER_PARAM::SP_INT_0:
		case SHADER_PARAM::SP_INT_1:
		case SHADER_PARAM::SP_INT_2:
		case SHADER_PARAM::SP_INT_3:
		{
			fread(m_vecParam[i].pData, sizeof(UINT), 1, _pFile);
		}
		break;
		case SHADER_PARAM::SP_FLOAT_0:
		case SHADER_PARAM::SP_FLOAT_1:
		case SHADER_PARAM::SP_FLOAT_2:
		case SHADER_PARAM::SP_FLOAT_3:
			fread(m_vecParam[i].pData, sizeof(float), 1, _pFile);
			break;
		case SHADER_PARAM::SP_VEC4_0:
		case SHADER_PARAM::SP_VEC4_1:
		case SHADER_PARAM::SP_VEC4_2:
		case SHADER_PARAM::SP_VEC4_3:
			fread(m_vecParam[i].pData, sizeof(Vec4), 1, _pFile);
			break;
		case SHADER_PARAM::SP_TEX_0:
		case SHADER_PARAM::SP_TEX_1:
		case SHADER_PARAM::SP_TEX_2:
		case SHADER_PARAM::SP_TEX_3:
		{
			BYTE has = 0;
			fread(&has, 1, 1, _pFile);
			if (has)
			{
				wstring strKey = LoadWString(_pFile);
				*((CResPtr<CTexture>*)(m_vecParam[i].pData)) = (CTexture*)CResManager::Manager().Load<CTexture>(strKey);
			}
		}
		break;
		}
	}
}


void CMaterial::SetData(SHADER_PARAM _eParam, void * _pData)
{
	bool bFind = false;
	UINT i = 0;
	for (i = 0; i < m_vecParam.size(); ++i)
	{
		if (_eParam == m_vecParam[i].eParam)
		{
			bFind = true;
			break;
		}
	}

	if (false == bFind)
		return;

	switch (m_vecParam[i].eParam)
	{
	case SHADER_PARAM::SP_INT_0:
	case SHADER_PARAM::SP_INT_1:
	case SHADER_PARAM::SP_INT_2:
	case SHADER_PARAM::SP_INT_3:
		*((int*)m_vecParam[i].pData) = *((int*)_pData);
		break;
	case SHADER_PARAM::SP_FLOAT_0:
	case SHADER_PARAM::SP_FLOAT_1:
	case SHADER_PARAM::SP_FLOAT_2:
	case SHADER_PARAM::SP_FLOAT_3:
		*((float*)m_vecParam[i].pData) = *((float*)_pData);
		break;
	case SHADER_PARAM::SP_VEC4_0:
	case SHADER_PARAM::SP_VEC4_1:
	case SHADER_PARAM::SP_VEC4_2:
	case SHADER_PARAM::SP_VEC4_3:
		*((Vec4*)m_vecParam[i].pData) = *((Vec4*)_pData);
		break;
	case SHADER_PARAM::SP_TEX_0:
	case SHADER_PARAM::SP_TEX_1:
	case SHADER_PARAM::SP_TEX_2:
	case SHADER_PARAM::SP_TEX_3:
		(*((CResPtr<CTexture>*)m_vecParam[i].pData)) = *((CTexture**)_pData);
		break;
	default:
		break;
	}
}
