#include "stdafx.h"
#include "UICharacterSel.h"
//#include "ResManager.h"

#include "PlayerManager.h"
#include "GameManager.h"
#include "KeyManager.h"
#include "ModeMgr.h"

#include "Stream.h"
#include "ClientManager.h"
#include "..\Doom\define_server.h"
#include "UIManager.h"

//#include "MeshRender.h"
//#include "Material.h"

CUICharacterSel::CUICharacterSel()
	: m_bIs1P(true)
	, m_bIsFinish(false)
	, m_iNum(0)
	, m_bFirstCall(true)
	, m_cPlayer(0)
{
	//if (m_iPlayMode == 0)
	//	m_strName = L"Iori2.goc";
	//else
		m_strName = L"Iori";
}


CUICharacterSel::~CUICharacterSel()
{
}

void CUICharacterSel::Awake()
{
}

void CUICharacterSel::Start()
{
}

void CUICharacterSel::Update()
{
	if (m_bFirstCall)
	{
		if (CKeyManager::Manager().GetKeyState(KEY_ENTER,STATE_DOWN))
		{
			m_bFirstCall = false;
		}
	}
	else
	{
		if (CKeyManager::Manager().GetKeyState(KEY_LEFT, STATE_DOWN))
			m_iNum--;
		else if (CKeyManager::Manager().GetKeyState(KEY_RIGHT, STATE_DOWN))
			m_iNum++;

		if (m_iNum < 0)m_iNum = 2;
		if (m_iNum > 2)m_iNum = 0;

		switch (m_iNum)
		{
		case 0:
			m_strName = L"Iori";
			break;
		case 1:
			m_strName = L"Nanaya";
			break;
		case 2:
			m_strName = L"Kyo";
			break;
		}

		list<CGameObject*> listChild = GetGameObject()->GetChildList();
		list<CGameObject*>::iterator iter = listChild.begin();
		list<CGameObject*>::iterator iterEnd = listChild.end();
		wstring strTag;

		for (iter; iter != iterEnd; ++iter)
		{
			strTag = (*iter)->GetTagName();

			if (strTag == m_strName)
			{
				CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
				(*iter)->GetMeshRender()->SetMaterial(pMtrl);
			}
			else
			{
				CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Gray");
				(*iter)->GetMeshRender()->SetMaterial(pMtrl);
			}
		}

		if (m_iNum != 1)//nanaya พฦดิ
			m_strName += L"3.goc";
		else
			m_strName += L"5.goc";

		if (!m_bIsFinish)
		{
			if (CKeyManager::Manager().GetKeyState(KEY_ENTER, STATE_DOWN))
			{
				if (CModeMgr::GetInst()->GetMode() == 0)
				{
					if (m_bIs1P)
					{
						CPlayerManager::GetInst()->Set1P(m_strName);
						m_bIs1P = false;
						m_iNum = 0;
						m_iPlayMode = 0;
						CKeyManager::Manager().Active2P(false);
					}
					else
					{
						CPlayerManager::GetInst()->Set2P(m_strName);
						m_bIsFinish = true;
						CGameManager::GetInst()->init();
						CGameManager::GetInst()->SetActive2P(false);
						m_bIs1P = true;
					}
				}

				else if (CModeMgr::GetInst()->GetMode() == 3
					||CModeMgr::GetInst()->GetOnline())
				{
					Mode_Server();
					CModeMgr::GetInst()->SetOnline(true);
				}

				else
				{
					if (m_bIs1P)
					{
						CPlayerManager::GetInst()->Set1P(m_strName);
						m_bIs1P = false;
						m_iNum = 0;
						m_iPlayMode = 1;
						CKeyManager::Manager().Active2P(true);
					}
					else
					{
						CPlayerManager::GetInst()->Set2P(m_strName);
						m_bIsFinish = true;
						CGameManager::GetInst()->init();
						CGameManager::GetInst()->SetActive2P(true);
						m_bIs1P = true;
					}
				}
			}
		}
	}
}

void CUICharacterSel::LateUpdate()
{
	if (m_bIsFinish)
	{
		GetGameObject()->Die();
	}
}

void CUICharacterSel::Mode_Server()
{
	CStream stream;
	
	char PlayerInfo[255] = {};

	if (m_bIs1P == true)
	{
		CPlayerManager::GetInst()->Set1P(m_strName);
		CGameManager::GetInst()->SetActive2P(false);
		m_cPlayer = 1;
		CPlayerManager::GetInst()->SetPlayer(true);
	}
	if (m_bIs1P == false)
	{
		CPlayerManager::GetInst()->Set2P(m_strName);
		CGameManager::GetInst()->SetActive2P(true);
		m_cPlayer = 2;
		CPlayerManager::GetInst()->SetPlayer(false);
	}
	
	CUIManager::GetInst()->SetPlayer(m_cPlayer);

	stream.SetBuffer(PlayerInfo);
	stream.WriteStream(&m_cPlayer, sizeof(char));
	stream.WriteStream(m_strName.c_str(), m_strName.size() * 2);
	ServerEngine::CClientManager::Manager().Write(PH_CHARACTORSEL, PlayerInfo, sizeof(PlayerInfo));

	m_bIsFinish = true;
}

void CUICharacterSel::SetPlayer(char _cPlayer)
{
	m_cPlayer = _cPlayer;
	if (1 == _cPlayer)
	{
		m_bIs1P = true;
	}
	else
	{
		m_bIs1P = false;
	}

}