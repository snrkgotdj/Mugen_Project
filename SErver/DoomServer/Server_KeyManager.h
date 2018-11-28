#pragma once

#include "KeyManager.h"

class Server_KeyManager
{
private:
	KEY m_arrKey_1P[KEY_TYPE_END];
	KEY m_arrKey_2P[KEY_TYPE_END];

	bool m_bUpdateServer;
	KEY_TYPE m_iClientKey;
	KEY_STATE m_eClientState;

public:
	char* GetKey(KEY_TYPE _Key);

public:
	void init();
	void Update();
	void UpdateServer();

public:
	bool GetKeyState(KEY_TYPE _key, KEY_STATE _state, bool _is1P = true);
	void SetClientKey(KEY_TYPE _key, KEY_STATE _state, bool _is1P = true);
	void SetNetworkOn(bool _on);

public:
	static Server_KeyManager& Manager()
	{
		static Server_KeyManager inst;
		return inst;
	}
public:
	Server_KeyManager();
	~Server_KeyManager();
};

