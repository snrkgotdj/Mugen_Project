#pragma once
#include "global.h"
#include "ResPtr.h"
#include "Texture.h"
#include "PathManager.h"
#include "Sound.h"

class CMesh;
class CPrefab;
class CGameObject;
class CMaterial;

class CResManager
{
private:
	map<wstring, CResPtr<CMesh>>		m_mapMesh;
	map<wstring, CResPtr<CTexture>>		m_mapTex;
	map<wstring, CResPtr<CPrefab>>		m_mapPrefab;
	map<wstring, CResPtr<CMaterial>>	m_mapMaterial;
	map<wstring, CResPtr<CSound>>		m_mapSound;

	vector<CResPtr<CRes>> m_vecCloneRes;

	bool m_bChange;
public:
	void init();

private:
	int AddMesh(const wstring& _strKey, CMesh* _pMesh);
	void CreateFMODSystem();

public:
	int AddPrefab(const wstring& _strKey, CGameObject* _pObj);
	int AddMaterial(const wstring& _strKey, CMaterial* _pMtrl);
	CMesh* FindMesh(const wstring& _strKey);
	CPrefab* FindPrefab(const wstring& _strKey);
	CTexture* FindTexture(const wstring& _strKey);
	CMaterial* FindMaterial(const wstring& _strKey);
	CSound* FindSound(const wstring& _strKey);
	void AddCloneRes(CRes* _pRes) { m_bChange = true;  m_vecCloneRes.push_back(_pRes); }
	bool IsChange() { return m_bChange; }
	void SetChange(bool _change) { m_bChange = _change; }

private:
	void Save(FILE* _pFile); 
	void Load(FILE* _pFile);

public:
	map<wstring, CResPtr<CMesh>>& GetMeshList() { return m_mapMesh; }
	map<wstring, CResPtr<CTexture>>& GetTextureList() { return m_mapTex; }
	map<wstring, CResPtr<CPrefab>>& GetPrefabList() { return m_mapPrefab; }
	map<wstring, CResPtr<CMaterial>>& GetMaterialList() { return m_mapMaterial; }
	map<wstring, CResPtr<CSound>>& GetSoundList() { return m_mapSound; }

	vector<CResPtr<CRes>>& GetCloneList() { return m_vecCloneRes; }
	
private:
	void CreateMesh();
	void CreateRectMash();
	void CreatePlayerMash();
	void CreateCollierMash();
	void CreateShadowMesh();

	void CreateMaterial();
	void CreateColliderMtrl();
	void CreateColorMtrl();
	void CreateStd2DMtrl();
	void CreateGrayMtrl();
	void CreateRedMtrl();
	void CreateBlueMtrl();
	void CreateBlackMtrl();
	void CreateEffectMtrl();
	void CreateReverseMtrl();
	void CreateAllRedMtrl();
	void CreateAllBlackMtrl();

public:
	void LoadMultiTextures(const wstring& _strKey, const wstring& _strPath, UINT _iMin, UINT _iMax);

public:
	template<typename T>
	CRes* Load(const wstring& _strKey, const wstring& _strPath = L"");

	friend class CSaveLoadManager;

private:
	static CResManager* m_pInst;

public:
	static CResManager& Manager()
	{
		if (NULL == m_pInst)
		{
			m_pInst = new CResManager;
		}		
		return (*m_pInst);
	}
	static void DestoryInst()
	{
		SAFE_DELETE(m_pInst);
	}

private:
	CResManager();
	~CResManager();
};

template<typename T>
inline CRes * CResManager::Load(const wstring & _strKey, const wstring & _strPath)
{
	const type_info& info = typeid(T);

	if (info.hash_code() == typeid(CMesh).hash_code())
	{
		CMesh* pMesh = FindMesh(_strKey);
		return (CRes*)pMesh;
	}
	else if (info.hash_code() == typeid(CTexture).hash_code())
	{
		CTexture* pTexture = FindTexture(_strKey);
		
		if(pTexture != NULL)
			return (CRes*)pTexture;

		wstring strFullPath = CPathManager::GetResPathEX();
		strFullPath += _strPath;
		pTexture = CTexture::Create(strFullPath);

		if (NULL == pTexture)
			return NULL;

		m_bChange = true;

		pTexture->SetKey(_strKey);
		pTexture->SetPath(_strPath);
		m_mapTex.insert(make_pair(_strKey, pTexture));
		return (CRes*)pTexture;
	}
	else if (info.hash_code() == typeid(CPrefab).hash_code())
	{
		CPrefab* _pPrefab = FindPrefab(_strKey);
		return (CRes*)_pPrefab;
	}

	else if (info.hash_code() == typeid(CSound).hash_code())
	{
		CSound* pSound = FindSound(_strKey);

		if (NULL == pSound)
		{
			wstring strFullPath = CPathManager::GetResPathEX();
			strFullPath += L"Sound\\";
			strFullPath += _strPath;
			pSound = CSound::Create(strFullPath);
			pSound->SetKey(_strKey);
			m_mapSound.insert(make_pair(pSound->GetKey(), pSound));
		}

		return pSound;
	}

	return NULL;
}
