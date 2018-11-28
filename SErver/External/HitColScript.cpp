#include "stdafx.h"
#include "HitColScript.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "Collider.h"

#include "CharactorScript.h"
#include "TimeManager.h"
#include "ScriptManager.h"

#include "ScriptCode\UIManager.h"
#include "ScriptCode\UIBar.h"
#include "ScriptCode\UICombo.h"
#include "ScriptCode\UIGuardBar.h"
#include "NetworkManager.h"

#include "..\Doom\define_server.h"
#include "Stream.h"

CHitColScript::CHitColScript()
{
}

CHitColScript::~CHitColScript()
{
}

void CHitColScript::MoveLayer(bool _1P)
{
	CLayer* pLayer;

	if (true == _1P)
	{
		pLayer = CSceneManager::Manager().GetCurScene()->FindLayer(L"Hit_1P");
	}
	else
	{
		pLayer = CSceneManager::Manager().GetCurScene()->FindLayer(L"Hit_2P");
	}

	CLayer* pCurLayer = CSceneManager::Manager().GetCurScene()->FindLayer(GetGameObject()->GetLayerName());
	if (NULL != pCurLayer)
	{
		pCurLayer->RemoveListOnly(GetGameObject());
	}

	pLayer->AddGameObject(GetGameObject());
}

void CHitColScript::Damage(float _Damage)
{
	CGameObject* pHP;
	CGameObject* pCombo;

	if (m_b1P == true)
	{
		pHP = CUIManager::GetInst()->Get2PHP();
		pCombo = CUIManager::GetInst()->GetCombo1P();
	}
	else
	{
		pHP = CUIManager::GetInst()->Get1PHP();
		pCombo = CUIManager::GetInst()->GetCombo2P();
	}

	if (pHP != NULL)
	{
		list<CGameObject*>& listChild = pHP->GetChildList();
		for (auto& pair : listChild)
		{
			if (pair->GetTagName() == L"HPBar2P")
			{
				((CUIBar*)(pair)->GetFirstScript())->SetValue(((CUIBar*)(pair)->GetFirstScript())->GetValue() - (_Damage / 100.f));
			}
		}
	}

	if (pCombo != NULL)
	{
		list<CGameObject*>& listChild = pCombo->GetChildList();
		for (auto& pair : listChild)
		{
			if (pair->GetTagName() == L"01"||pair->GetTagName()==L"10")
			{
				((CUICombo*)(pair)->GetFirstScript())->AddValue();
			}
		}
	}

	if (true == CKeyManager::Manager().isServer())
	{
		PACKET_HP tPacket = {};
		tPacket.bPlayer = m_b1P;
		tPacket.fDamage = _Damage;
		ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_HP, (char*)&tPacket, sizeof(PACKET_HP));
	}
}

void CHitColScript::GuardDamage(float _fDmg)
{
	CGameObject* pHP;

	if (m_b1P == true)
		pHP = CUIManager::GetInst()->Get2PHP();
	else
		pHP = CUIManager::GetInst()->Get1PHP();

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
					if(CScriptMgr::GetScriptName(*iter)== L"UIGuardBar")
					{
						float fVal = ((CUIGuardBar*)(*iter))->GetValue();
						//((CUIGuardBar*)(pair)->GetFirstScript())->GetValue();

						
							((CUIGuardBar*)(*iter))->SetValue(fVal - (_fDmg / 100.f));
					}
				}
			}
			//if (pair->GetTagName() == L"GuardBar2P")
			//{
			//	((CUIGuardBar*)(pair)->GetFirstScript())->
			//		SetValue(((CUIGuardBar*)(pair)->GetFirstScript())->GetValue() - (_fDmg / 100.f));
			//}
		}
	}
}

void CHitColScript::GuardRecover()
{
	CGameObject* pHP;

	if (m_b1P == true)
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
				list<Script*>& listScript = pair->GetScriptList();
				list<Script*>::iterator iter = listScript.begin();
				list<Script*>::iterator iterEnd = listScript.end();

				for (iter; iter != iterEnd; ++iter)
				{
					if (CScriptMgr::GetScriptName(*iter) == L"UIGuardBar")
					{
						float fVal = ((CUIGuardBar*)(*iter))->GetValue();

						if (fVal < 1.f)
							((CUIGuardBar*)(*iter))->SetValue(fVal + DT*0.025f);
					}
				}
			}
		}
	}
}

void CHitColScript::Update()
{
	
}

void CHitColScript::FinalUpdate()
{
	if (m_pAnimator == NULL || m_pCollider == NULL || NULL == m_pTransForm)
		return;

	if (m_pAnimator->GetCurAnim() == NULL)
		return;

	const tFrameInfo* tCurFrameInfo = m_pAnimator->GetCurAnim()->GetCurFrameInfo();

	m_vecPos.x = tCurFrameInfo->tHitCol.vPos.x;
	m_vecPos.y = tCurFrameInfo->tHitCol.vPos.y;

	m_vecScale.x = tCurFrameInfo->tHitCol.vSize.x;
	m_vecScale.y = tCurFrameInfo->tHitCol.vSize.y;

	m_pCollider->SetPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CHitColScript::OnCollisionEnter(CCollider * _pOther)
{
	
}

void CHitColScript::OnCollision(CCollider * _pOther)
{
}

void CHitColScript::OnCollisionExit(CCollider * _pOther)
{
}

