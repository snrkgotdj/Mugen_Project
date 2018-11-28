#pragma once

#include "GameObject.h"
#include "Layer.h"

class CSaveLoadManager
{
public:
	static CSaveLoadManager& Manager()
	{
		static CSaveLoadManager inst;
		return inst;
	}
private:
	CSaveLoadManager();
	~CSaveLoadManager();

public:
	int SaveScene(const wstring& _strFilePath);
	int LoadScene(const wstring& _strFilePath);

	int SaveLayer(CLayer* _pLayer, FILE* _pFile);
	CLayer* LoadLayer(FILE* _pFile);

	int SaveGameObject(CGameObject* _pObj, FILE* _pFile);
	CGameObject* LoadGameObject(FILE* _pFile);

	int SaveGameObjectOnly(const wstring& _strFilePath, CGameObject* _pObj);
	CGameObject* LoadGameObjectOnly(const wstring& _strFilePath);
};

