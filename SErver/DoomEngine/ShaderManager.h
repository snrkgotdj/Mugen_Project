#pragma once

#include "global.h"

class CShader;
class CSampler;
class CBlendState;
class CDepthStencilState;

class CShaderManager
{
private:
	map<wstring, CShader*>	m_mapShader;
	map<wstring, CSampler*>	m_mapSampler;
	map<wstring, CBlendState*> m_mapBlendState;
	map<wstring, CDepthStencilState*> m_mapDepthStencilState;

public:
	void init();
	CShader* FindShader(const wstring& _strKey);
	CSampler* FindSampler(const wstring& _strKey);
	CBlendState* FindBlendState(const wstring& _strKey);
	CDepthStencilState* FindDepthStencilState(const wstring& _strKey);

private:
	void AddShader(const wstring& _strKey, CShader* _pShader);
	void AddSampler(const wstring& _strKey, CSampler* _pSampler);
	void AddBlendState(const wstring& _strKey, CBlendState* _pState);
	void AddDepthStencilState(const wstring& _strKey, CDepthStencilState* _pState);

	void CreateSampler();
	void CreateShader();
	void CreateBlendState();
	void CreateDepthStencilState();

private:
	void CreateColorShader();
	void CreateTextureShader();
	void CreateStd2DShader();
	void CreateColider2DShader();
	void CreateUIShader();
	void CreateGrayShader();
	void CreateRedShader();
	void CreateBlueShader();
	void CreateBlackShader();
	void CreateEffectShader();
	void CreateReverseShader();
	void CreateAllRedShader();
	void CreateAllBlackShader();

public:
	static CShaderManager& Manager()
	{
		static CShaderManager inst;
		return inst;
	}

private:
	CShaderManager();
	~CShaderManager();
};

