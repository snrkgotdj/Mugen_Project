#include "stdafx.h"
#include "Server_KeyManager.h"


Server_KeyManager::Server_KeyManager()
{
}


Server_KeyManager::~Server_KeyManager()
{
}

void Server_KeyManager::init()
{
	for (int i = 0; i < (int)KEY_TYPE::KEY_TYPE_END; ++i)
	{
		m_arrKey_1P[i].state = KEY_STATE::STATE_NONE;
	}

	m_arrKey_1P[(int)KEY_TYPE::KEY_1].key = '1';
	m_arrKey_1P[(int)KEY_TYPE::KEY_2].key = '2';
	m_arrKey_1P[(int)KEY_TYPE::KEY_3].key = '3';
	m_arrKey_1P[(int)KEY_TYPE::KEY_4].key = '4';
	m_arrKey_1P[(int)KEY_TYPE::KEY_5].key = '5';
	m_arrKey_1P[(int)KEY_TYPE::KEY_6].key = '6';
	m_arrKey_1P[(int)KEY_TYPE::KEY_7].key = '7';
	m_arrKey_1P[(int)KEY_TYPE::KEY_8].key = '8';
	m_arrKey_1P[(int)KEY_TYPE::KEY_9].key = '9';
	m_arrKey_1P[(int)KEY_TYPE::KEY_0].key = '0';
	m_arrKey_1P[(int)KEY_TYPE::KEY_Q].key = 'Q';
	m_arrKey_1P[(int)KEY_TYPE::KEY_W].key = 'W';
	m_arrKey_1P[(int)KEY_TYPE::KEY_E].key = 'E';
	m_arrKey_1P[(int)KEY_TYPE::KEY_R].key = 'R';
	m_arrKey_1P[(int)KEY_TYPE::KEY_T].key = 'T';
	m_arrKey_1P[(int)KEY_TYPE::KEY_Y].key = 'Y';
	m_arrKey_1P[(int)KEY_TYPE::KEY_U].key = 'U';
	m_arrKey_1P[(int)KEY_TYPE::KEY_I].key = 'I';
	m_arrKey_1P[(int)KEY_TYPE::KEY_O].key = 'O';
	m_arrKey_1P[(int)KEY_TYPE::KEY_P].key = 'P';
	m_arrKey_1P[(int)KEY_TYPE::KEY_A].key = 'A';
	m_arrKey_1P[(int)KEY_TYPE::KEY_S].key = 'S';
	m_arrKey_1P[(int)KEY_TYPE::KEY_D].key = 'D';
	m_arrKey_1P[(int)KEY_TYPE::KEY_F].key = 'F';
	m_arrKey_1P[(int)KEY_TYPE::KEY_G].key = 'G';
	m_arrKey_1P[(int)KEY_TYPE::KEY_H].key = 'H';
	m_arrKey_1P[(int)KEY_TYPE::KEY_J].key = 'J';
	m_arrKey_1P[(int)KEY_TYPE::KEY_K].key = 'K';
	m_arrKey_1P[(int)KEY_TYPE::KEY_L].key = 'L';
	m_arrKey_1P[(int)KEY_TYPE::KEY_Z].key = 'Z';
	m_arrKey_1P[(int)KEY_TYPE::KEY_X].key = 'X';
	m_arrKey_1P[(int)KEY_TYPE::KEY_C].key = 'C';
	m_arrKey_1P[(int)KEY_TYPE::KEY_V].key = 'V';
	m_arrKey_1P[(int)KEY_TYPE::KEY_B].key = 'B';
	m_arrKey_1P[(int)KEY_TYPE::KEY_N].key = 'N';
	m_arrKey_1P[(int)KEY_TYPE::KEY_M].key = 'M';
	m_arrKey_1P[(int)KEY_TYPE::KEY_UP].key = VK_UP;
	m_arrKey_1P[(int)KEY_TYPE::KEY_DOWN].key = VK_DOWN;
	m_arrKey_1P[(int)KEY_TYPE::KEY_LEFT].key = VK_LEFT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_RIGHT].key = VK_RIGHT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_LSHIFT].key = VK_LSHIFT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_RSHIFT].key = VK_RSHIFT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_SPACE].key = VK_SPACE;
	m_arrKey_1P[(int)KEY_TYPE::KEY_INSERT].key = VK_INSERT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_DEL].key = VK_DELETE;
	m_arrKey_1P[(int)KEY_TYPE::KEY_HOME].key = VK_HOME;
	m_arrKey_1P[(int)KEY_TYPE::KEY_END].key = VK_END;
	m_arrKey_1P[(int)KEY_TYPE::KEY_PAGEUP].key = VK_PRIOR;
	m_arrKey_1P[(int)KEY_TYPE::KEY_PAGEDOWN].key = VK_NEXT;
	m_arrKey_1P[(int)KEY_TYPE::KEY_ESC].key = VK_ESCAPE;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F1].key = VK_F1;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F2].key = VK_F2;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F3].key = VK_F3;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F4].key = VK_F4;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F5].key = VK_F5;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F6].key = VK_F6;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F7].key = VK_F7;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F8].key = VK_F8;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F9].key = VK_F9;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F10].key = VK_F10;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F11].key = VK_F11;
	m_arrKey_1P[(int)KEY_TYPE::KEY_F12].key = VK_F12;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM0].key = VK_NUMPAD0;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM1].key = VK_NUMPAD1;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM2].key = VK_NUMPAD2;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM3].key = VK_NUMPAD3;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM4].key = VK_NUMPAD4;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM5].key = VK_NUMPAD5;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM6].key = VK_NUMPAD6;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM7].key = VK_NUMPAD7;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM8].key = VK_NUMPAD8;
	m_arrKey_1P[(int)KEY_TYPE::KEY_NUM9].key = VK_NUMPAD9;
	m_arrKey_1P[(int)KEY_TYPE::KEY_ENTER].key = VK_RETURN;
	m_arrKey_1P[(int)KEY_TYPE::KEY_CTRL].key = VK_CONTROL;
	m_arrKey_1P[(int)KEY_TYPE::KEY_ALT].key = VK_MENU;
	m_arrKey_1P[(int)KEY_TYPE::KEY_TAB].key = VK_TAB;
	m_arrKey_1P[(int)KEY_TYPE::KEY_CAPS].key = VK_CAPITAL;
	m_arrKey_1P[(int)KEY_TYPE::KEY_LBTN].key = VK_LBUTTON;
	m_arrKey_1P[(int)KEY_TYPE::KEY_RBTN].key = VK_RBUTTON;
	m_arrKey_1P[(int)KEY_TYPE::KEY_MBTN].key = VK_MBUTTON;

	//
	// 2P Key
	//
	m_arrKey_2P[(int)KEY_TYPE::KEY_1].key = '1';
	m_arrKey_2P[(int)KEY_TYPE::KEY_2].key = '2';
	m_arrKey_2P[(int)KEY_TYPE::KEY_3].key = '3';
	m_arrKey_2P[(int)KEY_TYPE::KEY_4].key = '4';
	m_arrKey_2P[(int)KEY_TYPE::KEY_5].key = '5';
	m_arrKey_2P[(int)KEY_TYPE::KEY_6].key = '6';
	m_arrKey_2P[(int)KEY_TYPE::KEY_7].key = '7';
	m_arrKey_2P[(int)KEY_TYPE::KEY_8].key = '8';
	m_arrKey_2P[(int)KEY_TYPE::KEY_9].key = '9';
	m_arrKey_2P[(int)KEY_TYPE::KEY_0].key = '0';
	m_arrKey_2P[(int)KEY_TYPE::KEY_Q].key = 'Q';
	m_arrKey_2P[(int)KEY_TYPE::KEY_W].key = 'W';
	m_arrKey_2P[(int)KEY_TYPE::KEY_E].key = 'E';
	m_arrKey_2P[(int)KEY_TYPE::KEY_R].key = 'R';
	m_arrKey_2P[(int)KEY_TYPE::KEY_T].key = 'T';
	m_arrKey_2P[(int)KEY_TYPE::KEY_Y].key = 'Y';
	m_arrKey_2P[(int)KEY_TYPE::KEY_U].key = 'U';
	m_arrKey_2P[(int)KEY_TYPE::KEY_I].key = 'I';
	m_arrKey_2P[(int)KEY_TYPE::KEY_O].key = 'O';
	m_arrKey_2P[(int)KEY_TYPE::KEY_P].key = 'P';
	m_arrKey_2P[(int)KEY_TYPE::KEY_A].key = 'A';
	m_arrKey_2P[(int)KEY_TYPE::KEY_S].key = 'S';
	m_arrKey_2P[(int)KEY_TYPE::KEY_D].key = 'D';
	m_arrKey_2P[(int)KEY_TYPE::KEY_F].key = 'F';
	m_arrKey_2P[(int)KEY_TYPE::KEY_G].key = 'G';
	m_arrKey_2P[(int)KEY_TYPE::KEY_H].key = 'H';
	m_arrKey_2P[(int)KEY_TYPE::KEY_J].key = 'J';
	m_arrKey_2P[(int)KEY_TYPE::KEY_K].key = 'K';
	m_arrKey_2P[(int)KEY_TYPE::KEY_L].key = 'L';
	m_arrKey_2P[(int)KEY_TYPE::KEY_Z].key = 'Z';
	m_arrKey_2P[(int)KEY_TYPE::KEY_X].key = 'X';
	m_arrKey_2P[(int)KEY_TYPE::KEY_C].key = 'C';
	m_arrKey_2P[(int)KEY_TYPE::KEY_V].key = 'V';
	m_arrKey_2P[(int)KEY_TYPE::KEY_B].key = 'B';
	m_arrKey_2P[(int)KEY_TYPE::KEY_N].key = 'N';
	m_arrKey_2P[(int)KEY_TYPE::KEY_M].key = 'M';
	m_arrKey_2P[(int)KEY_TYPE::KEY_UP].key = VK_UP;
	m_arrKey_2P[(int)KEY_TYPE::KEY_DOWN].key = VK_DOWN;
	m_arrKey_2P[(int)KEY_TYPE::KEY_LEFT].key = VK_LEFT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_RIGHT].key = VK_RIGHT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_LSHIFT].key = VK_LSHIFT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_RSHIFT].key = VK_RSHIFT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_SPACE].key = VK_SPACE;
	m_arrKey_2P[(int)KEY_TYPE::KEY_INSERT].key = VK_INSERT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_DEL].key = VK_DELETE;
	m_arrKey_2P[(int)KEY_TYPE::KEY_HOME].key = VK_HOME;
	m_arrKey_2P[(int)KEY_TYPE::KEY_END].key = VK_END;
	m_arrKey_2P[(int)KEY_TYPE::KEY_PAGEUP].key = VK_PRIOR;
	m_arrKey_2P[(int)KEY_TYPE::KEY_PAGEDOWN].key = VK_NEXT;
	m_arrKey_2P[(int)KEY_TYPE::KEY_ESC].key = VK_ESCAPE;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F1].key = VK_F1;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F2].key = VK_F2;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F3].key = VK_F3;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F4].key = VK_F4;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F5].key = VK_F5;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F6].key = VK_F6;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F7].key = VK_F7;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F8].key = VK_F8;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F9].key = VK_F9;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F10].key = VK_F10;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F11].key = VK_F11;
	m_arrKey_2P[(int)KEY_TYPE::KEY_F12].key = VK_F12;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM0].key = VK_NUMPAD0;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM1].key = VK_NUMPAD1;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM2].key = VK_NUMPAD2;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM3].key = VK_NUMPAD3;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM4].key = VK_NUMPAD4;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM5].key = VK_NUMPAD5;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM6].key = VK_NUMPAD6;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM7].key = VK_NUMPAD7;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM8].key = VK_NUMPAD8;
	m_arrKey_2P[(int)KEY_TYPE::KEY_NUM9].key = VK_NUMPAD9;
	m_arrKey_2P[(int)KEY_TYPE::KEY_ENTER].key = VK_RETURN;
	m_arrKey_2P[(int)KEY_TYPE::KEY_CTRL].key = VK_CONTROL;
	m_arrKey_2P[(int)KEY_TYPE::KEY_ALT].key = VK_MENU;
	m_arrKey_2P[(int)KEY_TYPE::KEY_TAB].key = VK_TAB;
	m_arrKey_2P[(int)KEY_TYPE::KEY_CAPS].key = VK_CAPITAL;
	m_arrKey_2P[(int)KEY_TYPE::KEY_LBTN].key = VK_LBUTTON;
	m_arrKey_2P[(int)KEY_TYPE::KEY_RBTN].key = VK_RBUTTON;
	m_arrKey_2P[(int)KEY_TYPE::KEY_MBTN].key = VK_MBUTTON;
}
