#include "stdafx.h"
#include "KeyCommand.h"
#include "TimeManager.h"


CKeyCommand::CKeyCommand()
	: m_bCommandOn(false)
	, m_bIs1P(true)
	, m_fAccTime(0)
{
	
}

CKeyCommand::~CKeyCommand()
{
}

void CKeyCommand::init(const PLAYER_KEY & _PlayerKey)
{
	m_tKey = _PlayerKey;

	vector<KEY_TYPE> vecCommand;
	vecCommand.push_back(m_tKey.X); vecCommand.push_back(m_tKey.X);
	m_mapCommand.insert(make_pair(L"XX", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Right);
	m_mapCommand.insert(make_pair(L"RR", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Left);
	m_mapCommand.insert(make_pair(L"LL", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Right);
	m_mapCommand.insert(make_pair(L"RRR", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Left);
	m_mapCommand.insert(make_pair(L"LLL", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right);
	m_mapCommand.insert(make_pair(L"RDR", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left);
	m_mapCommand.insert(make_pair(L"LDL", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Z);
	m_mapCommand.insert(make_pair(L"DRZ", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Z);
	m_mapCommand.insert(make_pair(L"DLZ", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.X);
	m_mapCommand.insert(make_pair(L"DRX", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.X);
	m_mapCommand.insert(make_pair(L"DLX", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.X);
	m_mapCommand.insert(make_pair(L"LRX", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.X);
	m_mapCommand.insert(make_pair(L"RLX", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Z);
	m_mapCommand.insert(make_pair(L"LRZ", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Z);
	m_mapCommand.insert(make_pair(L"RLZ", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.C);
	m_mapCommand.insert(make_pair(L"DRC", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.C);
	m_mapCommand.insert(make_pair(L"DLC", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.C);
	m_mapCommand.insert(make_pair(L"DLDRC", vecCommand));

	vecCommand.clear();
	vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Right); vecCommand.push_back(m_tKey.Down); vecCommand.push_back(m_tKey.Left); vecCommand.push_back(m_tKey.C);
	m_mapCommand.insert(make_pair(L"DRDLC", vecCommand));
}

void CKeyCommand::Update()
{
	if (false == m_bCommandOn)
		return;

	m_fAccTime += DT;

	if (m_fAccTime >= 0.5f)
	{
		m_bCommandOn = false;
		Clear();
		return;
	}

	//방향키
	if (CKeyManager::Manager().GetKeyState(KEY_RIGHT, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_RIGHT);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_LEFT, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_LEFT);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_UP, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_UP);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_DOWN, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_DOWN);
		m_fAccTime = 0;
	}

	// 공격키
	else if (CKeyManager::Manager().GetKeyState(KEY_Z, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_Z);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_X, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_X);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_C, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_C);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_A, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_A);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_S, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_S);
		m_fAccTime = 0;
	}
	else if (CKeyManager::Manager().GetKeyState(KEY_D, STATE_DOWN, m_bIs1P))
	{
		m_vecKey.push_back(KEY_D);
		m_fAccTime = 0;
	}
}

void CKeyCommand::Clear()
{
	m_vecKey.clear();
	m_fAccTime = 0;
}

bool CKeyCommand::GetCommand(const wstring& _strCommandKey, bool _bClear)
{
	map<wstring, vector<KEY_TYPE>>::iterator iter = m_mapCommand.find(_strCommandKey);
	if (iter == m_mapCommand.end())
	{
		MessageBoxW(NULL, L"해당 커맨드가 없습니다.", L"오류", MB_OK);
		return false;
	}

	if (m_vecKey.size() < iter->second.size())
		return false;

	for (UINT i = 0; i < iter->second.size(); ++i)
	{
		if (m_vecKey[i] != iter->second[i])
			return false;
	}

	if( true == _bClear)
		Clear();

	return true;
}

void CKeyCommand::SetKey(KEY_TYPE _type)
{
	if (false == m_bCommandOn)
	{
		Clear();
		m_vecKey.push_back(_type);
	}

	m_bCommandOn = true;
}
