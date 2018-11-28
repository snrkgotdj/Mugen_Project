#pragma once

#include "global.h"

enum KEY_TYPE
{
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_0,
	KEY_Q,
	KEY_W,
	KEY_E,
	KEY_R,
	KEY_T,
	KEY_Y,
	KEY_U,
	KEY_I,
	KEY_O,
	KEY_P,
	KEY_A,
	KEY_S,
	KEY_D,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_Z,
	KEY_X,
	KEY_C,
	KEY_V,
	KEY_B,
	KEY_N,
	KEY_M,
	KEY_UP,
	KEY_DOWN,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_SPACE,
	KEY_INSERT,
	KEY_DEL,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_ESC,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_NUM0,
	KEY_NUM1,
	KEY_NUM2,
	KEY_NUM3,
	KEY_NUM4,
	KEY_NUM5,
	KEY_NUM6,
	KEY_NUM7,
	KEY_NUM8,
	KEY_NUM9,
	KEY_ENTER,
	KEY_CTRL,
	KEY_ALT,
	KEY_TAB,
	KEY_CAPS,
	KEY_LBTN,
	KEY_RBTN,
	KEY_MBTN,
	KEY_TYPE_END
};

enum KEY_STATE
{
	STATE_DOWN,
	STATE_PUSH,
	STATE_UP,
	STATE_NONE,
};

struct Client_Key
{
	bool bPlayer;
	KEY_TYPE iKey;
	KEY_STATE eKeyState;
};

struct KEY
{
	int key;
	KEY_STATE state;
};

class CKeyManager
{
private:
	KEY m_arrKey_1P[KEY_TYPE_END];
	KEY	m_arrKey_2P[KEY_TYPE_END];

	Client_Key m_tClientKey;
	bool m_bUpdateServer;
	bool m_b1P;
	bool m_b2P;

public:
	void init();
	void Update();
	void UpdateClient();
	void UpdateServer();

private:
	void Update_1P(int i);
	void Update_2P(int i);

public:
	bool GetKeyState(KEY_TYPE _key, KEY_STATE _state, bool _is1P = true);
	void Set1PKey(KEY_TYPE _key, KEY_STATE _state) { m_arrKey_1P[(int)_key].state = _state; }
	void Set2PKey(KEY_TYPE _key, KEY_STATE _state) { m_arrKey_2P[(int)_key].state = _state; }
	void OnServer(bool _on);

	void Active1P(bool _true) { m_b1P = _true; }
	void Active2P(bool _true) { m_b2P = _true; }

public:
	char* GetKey(KEY_TYPE _Key);
	bool isServer() { return m_bUpdateServer; }
public:
	void SendKey(KEY_TYPE _key, KEY_STATE _state, bool _is1P);

public:
	static CKeyManager& Manager()
	{
		static CKeyManager inst;
		return inst;
	}
public:
	CKeyManager();
	~CKeyManager();
};

