#pragma once
#include "..\Doom\define_client.h"
#include "KeyManager.h"

struct PLAYER_KEY
{
	KEY_TYPE Right;
	KEY_TYPE Left;
	KEY_TYPE Up;
	KEY_TYPE Down;
	KEY_TYPE Z;
	KEY_TYPE X;
	KEY_TYPE C;
	KEY_TYPE A;
	KEY_TYPE S;
	KEY_TYPE D;
};

class CKeyCommand
{
private:
	PLAYER_KEY m_tKey;
	bool m_bIs1P;

private:
	map<wstring, vector<KEY_TYPE>> m_mapCommand;
	vector<KEY_TYPE> m_vecKey;
	bool m_bCommandOn;
	float m_fAccTime;

public:
	void init(const PLAYER_KEY& _PlayerKey);

public:
	void Update();
	void Clear();
	void CommandOn(bool _true) { m_bCommandOn = true; }

	bool GetCommand(const wstring& _strCommandKey, bool _bClear = true);
	void SetKey(KEY_TYPE _type); 
	void Set1P(bool _is1P) { m_bIs1P = _is1P; }

public:
	CKeyCommand();
	~CKeyCommand();
};

