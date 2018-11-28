#include "stdafx.h"
#include "ServerManager.h"

#include "ServerEngine.h"
#include "PacketQueue.h"
#include "NetworkManager.h"
#include "KeyManager.h"
#include "Stream.h"

#include "..\External\ScriptCode\PlayerManager.h"
#include "..\External\ScriptCode\GameManager.h"
#include "..\Doom\define_client.h"
#include "..\Doom\define_server.h"
#include "..\External\ScriptCode\\UIManager.h"
#include "..\External\ScriptCode\\UICharacterSel.h"

SERVER_USING

CServerManager::CServerManager()
	: m_b1P(false)
	, m_b2P(false)
{
}


CServerManager::~CServerManager()
{
	FreeConsole();
}


bool CServerManager::init()
{
	AllocConsole();
	freopen("CONOUT$", "wb", stdout);
	printf_s("Start_Server\n\n");

	return true;
}

bool CServerManager::run()
{
	PACKET tPacket = {};
	while (GET_SINGLE(CPacketQueue)->Pop(&tPacket))
	{
		CStream stream;
		stream.SetBuffer(tPacket.cPacket);

		switch (tPacket.iHeader)
		{
		case PH_JOIN:
			Join();

			break;

		case PH_LOGIN:
			break;

		case PH_CHARACTORSEL:
		{
			CStream stream;
			char cPlayer;
			wchar_t cPlayerName[255] = {};
			stream.SetBuffer(tPacket.cPacket);
			stream.ReadStream(&cPlayer, sizeof(char));
			stream.ReadStream(cPlayerName, sizeof(cPlayerName));

			if (cPlayer == 1)
			{
				m_b1P = true;
				CPlayerManager::GetInst()->Set1P(cPlayerName);
				GET_SINGLE(CNetworkManager)->SendPacket(PH_CHARACTORSEL, (char*)tPacket.cPacket, wcslen(cPlayerName) * 2 + 1, false);
			}

			else if (cPlayer == 2)
			{
				m_b2P = true;
				CPlayerManager::GetInst()->Set2P(cPlayerName);
				GET_SINGLE(CNetworkManager)->SendPacket(PH_CHARACTORSEL, (char*)tPacket.cPacket, wcslen(cPlayerName) * 2 + 1, true);
			}

			if (m_b1P == true && m_b2P == true)
			{
				CUIManager::GetInst()->SetTraining(false);
				CGameManager::GetInst()->init();
				MAP_STATE MapName = CPlayerManager::GetInst()->GetMapName();
				GET_SINGLE(CNetworkManager)->SendMessageAll(PH_GAMESTART, (char*)&MapName, 4);
				CSceneManager::Manager().SetPlayMode(true);
				m_b1P = false;
				m_b2P = false;

				CGameObject* pObj = CSceneManager::Manager().GetCurScene()->FindLayer(L"Default")->FindObjectByTag(L"CharacterSel");
				if (nullptr != pObj)
				{
					CUICharacterSel* pScript = ((CUICharacterSel*)pObj->GetFirstScript());
					pScript->SetFinish(true);
				}
			}
		}
		break;

		case PH_STATE:
		{
			PACKET_STATE Packet_State;
			stream.ReadStream(&Packet_State, sizeof(Packet_State));
			CPlayerManager::GetInst()->SetState(Packet_State.eState, Packet_State.bPlayer);
			//GET_SINGLE(CNetworkManager)->SendMessageAll(PH_STATE, (char*)&Packet_State, sizeof(PACKET_STATE));
			GET_SINGLE(CNetworkManager)->SendPacket(PH_STATE, (char*)&Packet_State, sizeof(PACKET_STATE), !Packet_State.bPlayer);
			break;
		}
			
		case PH_KEY_OUTPUT:
			break;

		default:
			break;
		}
	}

	return false;
}

void CServerManager::Join()
{
	char a = 1;
	if (m_b1P == false)
	{
		m_b1P = true;
	}

	else if (m_b2P == false)
		m_b2P = true;

	if (m_b1P == true && m_b2P == true)
	{
		printf_s("Connected Player\n");


		GET_SINGLE(CNetworkManager)->SendPacket(PH_JOIN, &a, 1, true);
		a = 2;
		GET_SINGLE(CNetworkManager)->SendPacket(PH_JOIN, &a, 1, false);

		m_b1P = false;
		m_b2P = false;
	}
}

