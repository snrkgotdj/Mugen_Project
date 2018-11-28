#pragma once
#include "Single\SingletonScript.h"
#include "..\Doom\define_client.h"

class CPlayerManager :
	public CSingletonScript<CPlayerManager>
{
private:
	wstring m_str1P;
	wstring m_str2P;
	wstring m_strMap;

	MAP_STATE m_eMapName;

	CGameObject* m_pGameObject_1P;
	CGameObject* m_pGameObject_2P;
	CGameObject* m_pMap;

	bool m_b1P;
	bool m_bNetwork;

public:
	void NetworkOn_1P();
	void NetworkOn_2P();
public:
	void Set1P(const wstring& _strName) { m_str1P = _strName; }
	void Set2P(const wstring& _strName) { m_str2P = _strName; }
	CGameObject* Get1P() { return m_pGameObject_1P; }
	CGameObject* Get2P() { return m_pGameObject_2P; }
	CGameObject* GetMap() { return m_pMap; }
	MAP_STATE GetMapName() { return m_eMapName; }

	void SetName(const wstring& _strName, char _cPlayer);
	void SetPlayer(bool _is1P) { m_b1P = _is1P; }
	void SetNetwork();
	void SetMapName(MAP_STATE _MapName) { m_eMapName = _MapName; }
	void SetState(UINT _iState, bool _is1P);
	void SetPos(const Vec3& _vecPos, bool _is1P);

	bool is1P() { return m_b1P; }
	bool isNetWork() { return m_bNetwork; }
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();

	CLONE(CPlayerManager);

public:
	CPlayerManager();
	~CPlayerManager();
};

