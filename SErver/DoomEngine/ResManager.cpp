#include "ResManager.h"
#include "ShaderManager.h"
#include "ResManager.h"

#include "Prefab.h"
#include "Mesh.h"
#include "Material.h"
#include "Collider2D.h"
#include "Collider.h"

#include "MeshRender.h"
#include "TransForm.h"

CResManager* CResManager::m_pInst = NULL;

CResManager::CResManager()
	:m_bChange(false)
{
}

CResManager::~CResManager()
{
	Safe_Delete_Map_Res(m_mapPrefab);
	Safe_Delete_Map_Res(m_mapMesh);
	Safe_Delete_Map_Res(m_mapTex);
	Safe_Delete_Map_Res(m_mapMaterial);
	Safe_Delete_Map_Res(m_mapSound);
	Safe_Delete_Vec_Res(m_vecCloneRes);

#ifndef _DEBUG
	CSound::g_pSystem->release();
#endif // !_DEBUG
}

void CResManager::init()
{
	CreateMesh();

	CreateMaterial();

	CreateFMODSystem();
}

int CResManager::AddMesh(const wstring & _strKey, CMesh * _pMesh)
{
	CMesh* pMesh = FindMesh(_strKey);

	if (NULL != pMesh)
	{
		SAFE_DELETE(_pMesh);
		return RET_FAIL;
	}

	m_bChange = true;
	_pMesh->SetKey(_strKey);
	m_mapMesh.insert(make_pair(_strKey, _pMesh));

	return RET_SUCCESS;
}

CMesh * CResManager::FindMesh(const wstring & _strKey)
{
	map<wstring, CResPtr<CMesh>>::iterator iter = m_mapMesh.find(_strKey);

	if (iter == m_mapMesh.end())
		return NULL;

	return iter->second;
}

CTexture * CResManager::FindTexture(const wstring & _strKey)
{
	map<wstring, CResPtr<CTexture>>::iterator iter = m_mapTex.find(_strKey);

	if (iter == m_mapTex.end())
	{
		return NULL;
	}

	return iter->second;
}

CPrefab * CResManager::FindPrefab(const wstring & _strKey)
{
	map<wstring, CResPtr<CPrefab>>::iterator iter = m_mapPrefab.find(_strKey);

	if (iter == m_mapPrefab.end())
	{
		return NULL;
	}

	return iter->second;
}

void CResManager::CreateFMODSystem()
{
	FMOD::System_Create(&CSound::g_pSystem);

	if (NULL == CSound::g_pSystem)
		assert(NULL);

	CSound::g_pSystem->init(32, FMOD_DEFAULT, NULL);
}

void CResManager::CreateMaterial()
{
	CreateColliderMtrl();
	CreateColorMtrl();
	CreateStd2DMtrl();
	CreateGrayMtrl();
	CreateRedMtrl();
	CreateBlueMtrl();
	CreateBlackMtrl();
	CreateEffectMtrl();
	CreateReverseMtrl();
	CreateAllRedMtrl();
	CreateAllBlackMtrl();
}

void CResManager::CreateColliderMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"ColliderShader"));
	AddMaterial(L"ColliderMtrl", pMtrl);
}

void CResManager::CreateColorMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Color"));
	AddMaterial(L"Material_Color", pMtrl);
}

void CResManager::CreateStd2DMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D"));
	CResManager::Manager().AddMaterial(L"Material_Std2D", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Texture"));
	CResManager::Manager().AddMaterial(L"DefaultMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Texture_EX"));
	pMtrl->SetKey(L"DefaultMtrl_EX");
	CResManager::Manager().AddMaterial(L"DefaultMtrl_EX", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_UIBar"));
	pMtrl->SetKey(L"UIBarMtrl");
	CResManager::Manager().AddMaterial(L"UIBarMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_UIFont"));
	pMtrl->SetKey(L"UIFontMtrl");
	CResManager::Manager().AddMaterial(pMtrl->GetKey(), pMtrl);
}

void CResManager::CreateGrayMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Gray"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Gray", pMtrl);
}

void CResManager::CreateRedMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Red"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Red", pMtrl);
}

void CResManager::CreateBlueMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Blue"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Blue", pMtrl);
}

void CResManager::CreateBlackMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Black"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Black", pMtrl);
}

void CResManager::CreateEffectMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Effect"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Effect", pMtrl);
}

void CResManager::CreateReverseMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_Reverse"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_Reverse", pMtrl);
}

void CResManager::CreateAllRedMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_AllRed"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_AllRed", pMtrl);
}

void CResManager::CreateAllBlackMtrl()
{
	CMaterial* pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderManager::Manager().FindShader(L"Shader_Std2D_AllBlack"));
	CResManager::Manager().AddMaterial(L"Material_Std2D_AllBlack", pMtrl);
}

void CResManager::LoadMultiTextures(const wstring & _strKey, const wstring & _strPath, UINT _iMin, UINT _iMax)
{
	for (UINT i = _iMin; i < _iMax; ++i)
	{
		wstring strKeyName = _strKey;
		wstring strPathName = _strPath;
		wchar_t szIdx[10];
		wsprintfW(szIdx, L"%d", i);
		strKeyName += szIdx;
		strPathName += szIdx;
		strPathName += L".png";
		Load<CTexture>(strKeyName, strPathName);
	}
}

int CResManager::AddPrefab(const wstring & _strKey, CGameObject * _pObj)
{
	CPrefab* pPrefab = FindPrefab(_strKey);

	if (NULL != pPrefab)
	{
		SAFE_DELETE(_pObj);
		return RET_FAIL;
	}
	pPrefab = new CPrefab(_pObj);

	pPrefab->SetKey(_strKey);
	m_mapPrefab.insert(make_pair(_strKey, pPrefab));

	return RET_SUCCESS;
}

int CResManager::AddMaterial(const wstring & _strKey, CMaterial * _pMtrl)
{
	CMaterial* pMtrl = FindMaterial(_strKey);

	if (NULL != pMtrl)
	{
		SAFE_DELETE(pMtrl);
		return RET_FAIL;
	}

	m_bChange = true;
	_pMtrl->SetKey(_strKey);
	m_mapMaterial.insert(make_pair(_strKey, _pMtrl));
	return RET_SUCCESS;
}

CMaterial * CResManager::FindMaterial(const wstring & _strKey)
{
	map<wstring, CResPtr<CMaterial>>::iterator iter = m_mapMaterial.find(_strKey);

	if (iter == m_mapMaterial.end())
		return NULL;

	return iter->second;
}

CSound * CResManager::FindSound(const wstring & _strKey)
{
	map<wstring, CResPtr<CSound>>::iterator iter = m_mapSound.find(_strKey);

	if (iter == m_mapSound.end())
		return NULL;

	return iter->second;
}


void CResManager::CreateMesh()
{
	CreateRectMash();
	CreatePlayerMash();
	CreateCollierMash();
	CreateShadowMesh();
}

void CResManager::CreateRectMash()
{
	VTX arrVtx[4] = {};
	INDEX16 arrIdx[2] = {};

	arrVtx[0].vtxPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vtxUV = Vec2(0.f, 0.f);
	arrVtx[0].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vtxPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vtxUV = Vec2(1.f, 0.f);
	arrVtx[1].vtxColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vtxPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vtxUV = Vec2(0.f, 1.f);
	arrVtx[2].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[3].vtxPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[3].vtxUV = Vec2(1.f, 1.f);
	arrVtx[3].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrIdx[0] = INDEX16(0, 3, 2);
	arrIdx[1] = INDEX16(0, 1, 3);

	//Mesh 积己
	CMesh* pMesh = CMesh::Create(4, sizeof(VTX), D3D11_USAGE_DYNAMIC, arrVtx
		, 6, INDEX16::size(), D3D11_USAGE_DEFAULT, arrIdx, INDEX16::format());

	pMesh->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	pMesh->SetKey(L"RectMesh");
	AddMesh(pMesh->GetKey(), pMesh);
}

void CResManager::CreatePlayerMash()
{
	VTX arrVtx[4] = {};
	INDEX16 arrIdx[2] = {};

	//敲饭捞绢 积己
	arrVtx[0].vtxPos = Vec3(0.f, 0.5f, 0.f);
	arrVtx[0].vtxUV = Vec2(0.f, 0.f);
	arrVtx[0].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vtxPos = Vec3(0.f, -0.25f, 0.f);
	arrVtx[1].vtxUV = Vec2(1.f, 0.f);
	arrVtx[1].vtxColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vtxPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vtxUV = Vec2(0.f, 1.f);
	arrVtx[2].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[3].vtxPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[3].vtxUV = Vec2(1.f, 1.f);
	arrVtx[3].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrIdx[0] = INDEX16(0, 1, 2);
	arrIdx[1] = INDEX16(0, 3, 1);

	//Mesh 积己
	CMesh* pMesh = CMesh::Create(4, sizeof(VTX), D3D11_USAGE_DYNAMIC, arrVtx
		, 6, INDEX16::size(), D3D11_USAGE_DEFAULT, arrIdx, INDEX16::format());

	pMesh->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	pMesh->SetKey(L"PlayerMesh");
	AddMesh(pMesh->GetKey(), pMesh);
}

void CResManager::CreateCollierMash()
{
	VTX arrVtx[4] = {};

	//敲饭捞绢 积己
	arrVtx[0].vtxPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vtxUV = Vec2(0.f, 0.f);
	arrVtx[0].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vtxPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vtxUV = Vec2(1.f, 0.f);
	arrVtx[1].vtxColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vtxPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vtxUV = Vec2(1.f, 1.f);
	arrVtx[2].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	////////////////
	arrVtx[3].vtxPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vtxUV = Vec2(0.f, 1.f);
	arrVtx[3].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	WORD idx[5] = { 0,1,2,3,0 };

	//Mesh 积己
	CMesh* pMesh = CMesh::Create(4, sizeof(VTX), D3D11_USAGE_DYNAMIC, arrVtx
		, 5, INDEX16::size(), D3D11_USAGE_DEFAULT, idx, INDEX16::format(), D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pMesh->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	pMesh->SetKey(L"ColliderRectMesh");
	AddMesh(pMesh->GetKey(), pMesh);
}

void CResManager::CreateShadowMesh()
{
	VTX arrVtx[4] = {};
	INDEX16 arrIdx[2] = {};

	arrVtx[0].vtxPos = Vec3(0.f, -1.f, 0.f);
	arrVtx[0].vtxUV = Vec2(0.f, 0.f);
	arrVtx[0].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vtxPos = Vec3(1.f, -1.f, 0.f);
	arrVtx[1].vtxUV = Vec2(1.f, 0.f);
	arrVtx[1].vtxColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vtxPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vtxUV = Vec2(0.f, 1.f);
	arrVtx[2].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[3].vtxPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[3].vtxUV = Vec2(1.f, 1.f);
	arrVtx[3].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrIdx[0] = INDEX16(0, 3, 2);
	arrIdx[1] = INDEX16(0, 1, 3);

	//Mesh 积己
	CMesh* pMesh = CMesh::Create(4, sizeof(VTX), D3D11_USAGE_DYNAMIC, arrVtx
		, 6, INDEX16::size(), D3D11_USAGE_DEFAULT, arrIdx, INDEX16::format());

	pMesh->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	pMesh->SetKey(L"Shadow_Mesh");
	AddMesh(pMesh->GetKey(), pMesh);


	//////////////////////////
	//////////////////////////////////

	memset(arrVtx, 0, sizeof(VTX) * 4);
	memset(arrIdx, 0, sizeof(INDEX16) * 2);

	arrVtx[0].vtxPos = Vec3(-1.f, -1.f, 0.f);
	arrVtx[0].vtxUV = Vec2(0.f, 0.f);
	arrVtx[0].vtxColor = Vec4(1.f, 0.f, 0.f, 1.f);

	arrVtx[1].vtxPos = Vec3(0.f, -1.f, 0.f);
	arrVtx[1].vtxUV = Vec2(1.f, 0.f);
	arrVtx[1].vtxColor = Vec4(0.f, 1.f, 0.f, 1.f);

	arrVtx[2].vtxPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[2].vtxUV = Vec2(0.f, 1.f);
	arrVtx[2].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrVtx[3].vtxPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[3].vtxUV = Vec2(1.f, 1.f);
	arrVtx[3].vtxColor = Vec4(0.f, 0.f, 1.f, 1.f);

	arrIdx[0] = INDEX16(0, 3, 2);
	arrIdx[1] = INDEX16(0, 1, 3);

	//Mesh 积己
	pMesh = CMesh::Create(4, sizeof(VTX), D3D11_USAGE_DYNAMIC, arrVtx
		, 6, INDEX16::size(), D3D11_USAGE_DEFAULT, arrIdx, INDEX16::format());

	pMesh->AddDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0);
	pMesh->AddDesc("TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 0);
	pMesh->AddDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0);

	pMesh->SetKey(L"Shadow_Mesh_Reverse");
	AddMesh(pMesh->GetKey(), pMesh);
}

void CResManager::Save(FILE* _pFile)
{
	UINT iCount = m_mapMesh.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);
	for (auto& pair : m_mapMesh)
	{
		int iRefCount = pair.second->GetRefCount();
		fwrite(&iRefCount, sizeof(int), 1, _pFile);
		if (iRefCount > 1)
			pair.second->Save(_pFile);
	}

	iCount = m_mapTex.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);
	for (auto& pair : m_mapTex)
	{
		int iRefCount = pair.second->GetRefCount();
		fwrite(&iRefCount, sizeof(int), 1, _pFile);
		if (iRefCount > 1)
			pair.second->Save(_pFile);
	}

	iCount = m_mapMaterial.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);
	for (auto& pair : m_mapMaterial)
	{
		int iRefCount = pair.second->GetRefCount();
		fwrite(&iRefCount, sizeof(int), 1, _pFile);
		if (iRefCount > 1)
			pair.second->Save(_pFile);
	}
}

void CResManager::Load(FILE* _pFile)
{
	UINT iCount = 0;
	int iRefCount = 0;

	// Mesh
	fread(&iCount, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		fread(&iRefCount, sizeof(int), 1, _pFile);
		if (1 < iRefCount)
		{
			wstring strKey = LoadWString(_pFile);
			wstring strPath = LoadWString(_pFile);
			Load<CMesh>(strKey, strPath);
		}
	}

	// Texture
	fread(&iCount, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		fread(&iRefCount, sizeof(int), 1, _pFile);
		if (1 < iRefCount)
		{
			wstring strKey = LoadWString(_pFile);
			wstring strPath = LoadWString(_pFile);
			Load<CTexture>(strKey, strPath);
		}
	}

	// Material
	fread(&iCount, sizeof(UINT), 1, _pFile);
	for (UINT i = 0; i < iCount; ++i)
	{
		fread(&iRefCount, sizeof(int), 1, _pFile);
		if (1 < iRefCount)
		{
			wstring strKey = LoadWString(_pFile);
			wstring strPath = LoadWString(_pFile);

			CMaterial* pMtrl = FindMaterial(strKey);
			if(NULL == pMtrl)
			{
				pMtrl = new CMaterial;
				AddMaterial(strKey, pMtrl);
			}
			pMtrl->Load(_pFile);
		}
	}
}