#include "stdafx.h"
#include "ClientMgr.h"
#include "define_server.h"
#include "define_client.h"
#include "ClientManager.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "KeyManager.h"

#include "PathManager.h"
#include "GameObject.h"
#include "SaveLoadManager.h"
#include "ScriptCode\UICharacterSel.h"
#include "ScriptCode\PlayerManager.h"
#include "ScriptCode\GameManager.h"
#include "ScriptCode\UIManager.h"
#include "ScriptCode\UIBar.h"
#include "ScriptCode\UICombo.h"
#include "ScriptManager.h"
#include "ScriptCode\UIGuardBar.h"


//서버관련 헤더

SERVER_USING

ClientMgr::ClientMgr()
	:m_bServer(false)
{
}

ClientMgr::~ClientMgr()
{
}

bool ClientMgr::run()
{
	if (m_bServer == false)
		return false;

	PACKET tPacket = {};
	CStream stream;
	while (GET_SINGLE(CPacketQueue)->Pop(&tPacket))
	{
		stream.SetBuffer(tPacket.cPacket);
		switch (tPacket.iHeader)
		{
		case PH_JOIN:
		{
			char a;
			stream.ReadStream(&a, sizeof(char));

			wstring strFullPath = CPathManager::GetResPathEX();
			strFullPath += L"Save\\UI\\UICharacterSel.goc";
			CGameObject* pCharacterSel = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath);
			((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetPlayMode(1);
			((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetTrainMode(3);

			if (1 == a)
			{
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->Set1P(true);
				CPlayerManager::GetInst()->SetPlayer(true);
			}

			else if (2 == a)
			{
				((CUICharacterSel*)pCharacterSel->GetFirstScript())->Set1P(false);
				CPlayerManager::GetInst()->SetPlayer(false);
			}
			break;
		}

		case PH_CHARACTORSEL:
		{
			char cPlayer;
			wchar_t strName[255] = {};
			stream.ReadStream(&cPlayer, sizeof(char));
			stream.ReadStream(strName, tPacket.iSize - 9);

			CPlayerManager::GetInst()->SetName(strName ,cPlayer);	

		}
		break;

		case PH_GAMESTART:
		{
			MAP_STATE eMapName = MAP_END;
			stream.ReadStream(&eMapName, sizeof(MAP_STATE));

			CPlayerManager::GetInst()->SetMapName(eMapName);
			CGameManager::GetInst()->init();
			CPlayerManager::GetInst()->SetNetwork();

			break;
		}

		case PH_HP:
		{
			PACKET_HP Packet_HP;
			stream.ReadStream(&Packet_HP, sizeof(Packet_HP));
			Damage(Packet_HP.fDamage, Packet_HP.bPlayer);
		}
			break;

		case PH_HP_EX:
		{
			PACKET_HP Packet_HP;
			stream.ReadStream(&Packet_HP, sizeof(Packet_HP));
			SetHP(Packet_HP.fDamage, Packet_HP.bPlayer);
		}
		break;
		case PH_STATE:
		{
			PACKET_STATE Packet_State;
			stream.ReadStream(&Packet_State, sizeof(Packet_State));
			CPlayerManager::GetInst()->SetState(Packet_State.eState, Packet_State.bPlayer);
		}
			break;
		
		case PH_POS:
		{
			PACKET_POS Packet_Pos;
			stream.ReadStream(&Packet_Pos, sizeof(Packet_Pos));
			CPlayerManager::GetInst()->SetPos(Packet_Pos.vecPos, Packet_Pos.bPlayer);
			//CPlayerManager::GetInst()->SetState(Packet_State.eState, Packet_State.bPlayer);
		}
		break;

		case PH_GUARD_HP:
		{
			PACKET_HP Packet_HP;
			stream.ReadStream(&Packet_HP, sizeof(Packet_HP));
			GuardDamage(Packet_HP.fDamage, Packet_HP.bPlayer);
		}

		default:
			break;
		}
	}

	return false;
}

void ClientMgr::Join()
{
	wstring strFullPath = CPathManager::GetResPathEX();
	strFullPath += L"Save\\UI\\UICharacterSel.goc";
	CGameObject* pCharacterSel = CSaveLoadManager::Manager().LoadGameObjectOnly(strFullPath);
	((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetPlayMode(1);
	((CUICharacterSel*)pCharacterSel->GetFirstScript())->SetTrainMode(3);
}

void ClientMgr::Damage(float _iDamage, bool _bool)
{
	CGameObject* pHP;
	CGameObject* pCombo;

	if (_bool == true)
	{
		pHP = CUIManager::GetInst()->Get2PHP();
		pCombo = CUIManager::GetInst()->GetCombo1P();
	}
	else
	{
		pHP = CUIManager::GetInst()->Get1PHP();
		pCombo = CUIManager::GetInst()->GetCombo2P();
	}

	if (pCombo != NULL)
	{
		list<CGameObject*>& listChild = pCombo->GetChildList();
		for (auto& pair : listChild)
		{
			if (pair->GetTagName() == L"01" || pair->GetTagName() == L"10")
			{
				((CUICombo*)(pair)->GetFirstScript())->AddValue();
			}
		}
	}
}

void ClientMgr::GuardDamage(float _fDamage, bool _bool)
{
	CGameObject* pHP;

	if (_bool == true)
		pHP = CUIManager::GetInst()->Get1PHP();		
	else
		pHP = CUIManager::GetInst()->Get2PHP();

	if (pHP != NULL)
	{
		list<CGameObject*>& listChild = pHP->GetChildList();
		for (auto& pair : listChild)
		{
			if (pair->GetTagName() == L"GuardBar2P")
			{
				list<Script*> listScript = pair->GetScriptList();
				list<Script*>::iterator iter = listScript.begin();
				list<Script*>::iterator iterEnd = listScript.end();

				for (iter; iter != iterEnd; ++iter)
				{
					if (CScriptMgr::GetScriptName(*iter) == L"UIGuardBar")
					{
						float fVal = ((CUIGuardBar*)(*iter))->GetValue();
						//((CUIGuardBar*)(pair)->GetFirstScript())->GetValue();


						((CUIGuardBar*)(*iter))->SetValue(fVal - (_fDamage / 100.f));
					}
				}
			}
		}
	}

}

void ClientMgr::SetHP(float _iDamage, bool _bool)
{
	CGameObject* pHP;
	CGameObject* pCombo;

	if (_bool == true)
	{
		pHP = CUIManager::GetInst()->Get1PHP();
		pCombo = CUIManager::GetInst()->GetCombo2P();
	}
	else
	{
		pHP = CUIManager::GetInst()->Get2PHP();
		pCombo = CUIManager::GetInst()->GetCombo1P();
	}

	if (pHP != NULL)
	{
		list<CGameObject*>& listChild = pHP->GetChildList();
		for (auto& pair : listChild)
		{
			if (pair->GetTagName() == L"HPBar2P")
			{
				((CUIBar*)(pair)->GetFirstScript())->SetValue( _iDamage);
			}
		}
	}
}

void ClientMgr::SetState(UINT _iState, bool _bool)
{
	
}
