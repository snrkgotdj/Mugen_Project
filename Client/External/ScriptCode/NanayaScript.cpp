#include "stdafx.h"
#include "NanayaScript.h"

#include "KeyManager.h"
#include "TimeManager.h"
#include "SceneManager.h"
#include "SaveLoadManager.h"
#include "EffectManager.h"
#include "GameManager.h"
#include "SoundLoadMgr.h"

#include "GameObject.h"

#include "AnimatorEX.h"
#include "TransForm.h"
#include "Collider2D.h"
#include "AnimationEX.h"

#include "KeyCommand.h"
#include "ScriptCode\EffectScript.h"
#include "ScriptCode\ShadowScript.h"
#include "ScriptCode\EffectManager.h"
#include "ScriptCode\PlayerManager.h"
#include "..\Doom\define_server.h"
#include "NetworkManager.h"
#include "ClientManager.h"

CNanayaScript::CNanayaScript()
	: m_eState(NANAYA_IDLE)
	, m_ePrevState(NANAYA_IDLE)
	, m_fWalkSpeed(300)
	, m_fAccTime(0)
	, m_pSound(NULL)
	, m_bSound(false)
	, m_bEffect(false)
{
	m_iDir = DIR_LEFT;
	m_fJumpSpeed = 0;
	m_fFloorY = -1.f;
	m_fGravAccel = 4000;
	m_fKnockback = 0.f;
	m_bDefeated = false;
}

CNanayaScript::~CNanayaScript()
{
}

void CNanayaScript::Gravity()
{
	if (m_bOnGround == false)
	{
		//m_vecPos = GetTransform()->GetLocalPos();

		m_vecPos.y += m_fJumpSpeed * DT;
		m_fGravity += m_fGravAccel * DT;
		m_vecPos.y -= m_fGravity * DT;

		GetTransform()->SetLocalPos(m_vecPos);
	}

	if (m_fStunHitTime == 0 && m_fStunAttTime == 0)
	{
		GetAnimatorEX()->SetPause(false);
	}

	else
	{
		if (m_fStunHitTime != 0)
		{
			if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
			{
				m_fStunTime += DT;

				int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

				if (iCurFrame >= 7 && m_eState == NANAYA_KNOCKDOWN)
				{
					GetAnimatorEX()->GetCurAnim()->SetFrameIndex(7);
					GetAnimatorEX()->SetPause(true);
				}
				else if (iCurFrame >= 1 && m_eState != NANAYA_KNOCKDOWN)
				{
					GetAnimatorEX()->GetCurAnim()->SetFrameIndex(1);
					GetAnimatorEX()->SetPause(true);
				}
				m_fGravity = 0;
			}
			else
			{
				m_fStunTime = 0;
				m_fStunHitTime = 0;
				GetAnimatorEX()->SetPause(false);
			}
		}

		else if (m_fStunAttTime != 0)
		{
			if (m_fStunTime < m_fStunAttTime)//스턴 걸린 상태
			{
				m_fStunTime += DT;
				GetAnimatorEX()->SetPause(true);
				m_fGravity = 0;
			}
			else
			{
				m_fStunTime = 0;
				m_fStunAttTime = 0;
				GetAnimatorEX()->SetPause(false);
			}
		}
	}
}

void CNanayaScript::SetDir(int _iDir)
{
	if (m_bOnGround == true && m_eState != NANAYA_GUIKSYNANAYA && m_eState != NANAYA_GUIKSYNANAYA_HIT)
		m_iDir = _iDir;

}

void CNanayaScript::SetState(UINT _iState)
{
	m_eState = (NANAYA_STATE)_iState;
	m_iBeforeIdx = -1;

	switch (m_eState)
	{
	case NANAYA_IDLE:
	{
		m_pAnimator->ChangeAnimation(L"Idle");
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
	}
		break;
	case NANAYA_WALK:
		m_pAnimator->ChangeAnimation(L"Walk");
		break;
	case NANAYA_BACKWALK:
		m_pAnimator->ChangeAnimation(L"뒤로 걷기", 1);
		break;
	case NANAYA_SIT:
		m_pAnimator->ChangeAnimation(L"앉기", 4, 4, 0);
		break;
	case NANAYA_ATTACK_Light:
		m_pAnimator->ChangeAnimation(L"약공격");
		break;
	case NANAYA_ATTACK_Medium:
		m_pAnimator->ChangeAnimation(L"중공격", 0, -1, 0);
		break;
	case NANAYA_ATTACK_Medium_Addition:
		m_pAnimator->ChangeAnimation(L"중공격 추가타", 10, -1, 0);
		break;
	case NANAYA_ATTACK_Power:
		m_pAnimator->ChangeAnimation(L"강공격");
		break;
	case NANAYA_ATTACK_SIT_Z:
		m_pAnimator->ChangeAnimation(L"앉아 공격", 5, -1, 0);
		break;
	case NANAYA_ATTACK_SIT_X:
		m_pAnimator->ChangeAnimation(L"앉아 발차기", 10, -1, 0);
		break;
	case NANAYA_ATTACK_JUMP_Z:
		m_pAnimator->ChangeAnimation(L"점프공격Z");
		break;
	case NANAYA_ATTACK_JUMP_X:
		m_pAnimator->ChangeAnimation(L"점프공격X");
		break;
	case NANAYA_ATTACK_JUMP_C:
		m_pAnimator->ChangeAnimation(L"점프공격C");
		break;
	case NANAYA_FRONT_DASH:
		m_pAnimator->ChangeAnimation(L"앞 대쉬", 0, -1, 0);
		break;
	case NANAYA_BACK_DASH:
		m_pAnimator->ChangeAnimation(L"백스탭", 0, -1, 0);
		break;
	case NANAYA_BACK_DUMBLING:
		m_pAnimator->ChangeAnimation(L"백 덤블링", 16, -1, 0);
		break;
	case NANAYA_JUMP_STAY:
		m_pAnimator->ChangeAnimation(L"제자리 점프", 14, -1, 0);
		break;
	case NANAYA_JUMP_FRONT:
		m_pAnimator->ChangeAnimation(L"앞 점프", 12, 13, 0);
		break;
	case NANAYA_JUMP_BACK:
		m_pAnimator->ChangeAnimation(L"백점프", 28, -1, 0);
		break;
	case NANAYA_JUMP_FORNT_DASH:
		m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
		m_fGravity = 0;
		break;
	case NANAYA_JUMP_BACK_DASH:
		m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
		m_fGravity = 0;
		break;
	case NANAYA_HITTED_MIDDLE:
		m_pAnimator->ChangeAnimation(L"중단피격", 7, -1, 0);
		break;
	case NANAYA_HITTED_UP:
		m_pAnimator->ChangeAnimation(L"상단피격", 7, -1, 0);
		break;
	case NANAYA_HITTED_DOWN:
		m_pAnimator->ChangeAnimation(L"하단피격", 7, -1, 0);
		break;
	case NANAYA_KNOCKDOWN:
		m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		break;
	case NANAYA_STANDUP:
		m_pAnimator->ChangeAnimation(L"일어서기2", 5, -1, 0);
		break;
	case NANAYA_PALJUMCHOONG:
		m_pAnimator->ChangeAnimation(L"팔점충", 7, -1, 0);
		break;
	case NANAYA_CHILYA:
		m_pAnimator->ChangeAnimation(L"섬소 칠야", 12, -1, 0);
		break;
	case NANAYA_PALCHUN:
		m_pAnimator->ChangeAnimation(L"섬소 팔천", 14, -1, 0);
		break;
	case NANAYA_YUKTO:
		m_pAnimator->ChangeAnimation(L"섬주 육토", 14, -1, 0);
		break;
	case NANAYA_GUIKSYNANAYA:
		m_pAnimator->ChangeAnimation(L"극사나나야", 19, 21, 0);
		break;
	case NANAYA_GUIKSYNANAYA_FAIL:
		m_pAnimator->ChangeAnimation(L"극사나나야", 41, -1, 34);
		break;
	case NANAYA_GUIKSYNANAYA_HIT:
		m_pAnimator->ChangeAnimation(L"극사나나야", 33, 33, 22);
		break;
	case NANAYA_GUARD:
		m_pAnimator->ChangeAnimation(L"서서막기", 2, -1, 0);
		CEffectManager::GetInst()->ActiveEffect(L"나나야방어", GetTransform()->GetLocalPos());
		break;
	case NANAYA_END:
		break;
	default:
		break;
	}
}

void CNanayaScript::Awake()
{
	CCharactorScript::Awake();

	m_pEffect = GetGameObject()->GetChildByName(L"Nanaya_Effect");
	if (m_pEffect != NULL)
	{
		m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);
		((CEffectScript*)m_pEffect->GetFirstScript())->SetManaged(false);
		((CEffectScript*)m_pEffect->GetFirstScript())->SetActive(true);
	}

	CEffectManager::GetInst()->LoadEffect(L"Nanaya");
	CSoundLoadMgr::GetInst()->LoadSoundFolder(L"Nanaya");
}

void CNanayaScript::Start()
{
	m_eState = NANAYA_IDLE;
	m_pAnimator->ChangeAnimation(L"Idle");

	//그림자
	CreateShadow();
	m_pKeyCommand->init(m_tKey);
	((CShadowScript*)(m_pShadow->GetFirstScript()))->init(GetGameObject(), 0.07f, 0.23f);
	m_vecPos = GetTransform()->GetLocalPos();
}

void CNanayaScript::Update()
{
	if (m_bOnline == false)
		Update_Client();

	else if (m_bOnline == true)
		Update_Server();
}

void CNanayaScript::LateUpdate()
{
	m_vecPos = m_pTransForm->GetLocalPos();

	if (m_iDir == DIR_RIGHT&& m_fStunAttTime == 0)
		m_vecPos.x += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->fSpeed * DT;
	
	else if (m_iDir == DIR_LEFT&& m_fStunAttTime == 0)
		m_vecPos.x -= m_pAnimator->GetCurAnim()->GetCurFrameInfo()->fSpeed * DT;

	if (m_bOnGround == false && m_fStunAttTime == 0)
		m_vecPos.y += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->fJumpSpeed * DT;

	if (m_vecPos.y < m_fFloorY && m_eState != NANAYA_JUMP_STAY)
	{
		m_vecPos.y = m_fFloorY;
		m_fGravity = 0;
		m_bOnGround = true;
	}

	else if (m_vecPos.y <= m_fFloorY && m_eState == NANAYA_JUMP_STAY)
	{
		m_vecPos.y = m_fFloorY;
		m_fGravity = 0;
		m_bOnGround = true;
	}

	m_pTransForm->SetLocalPos(m_vecPos);
}

void CNanayaScript::PushOtherPlayer(CGameObject * _pOther)
{
	if (m_eState == NANAYA_GUIKSYNANAYA || m_eState == NANAYA_GUIKSYNANAYA_HIT)
		return;

	if (m_eState == NANAYA_WALK)
	{
		Vec3 vecPos = _pOther->GetTransform()->GetLocalPos();
		if (m_vecPos.x < _pOther->GetTransform()->GetLocalPos().x)
			vecPos.x = m_vecPos.x + 50;

		else
			vecPos.x = m_vecPos.x - 50;

		_pOther->GetTransform()->SetLocalPos(vecPos);
	}

	else
	{
		if (m_vecPos.x < _pOther->GetTransform()->GetLocalPos().x)
			m_vecPos.x = _pOther->GetTransform()->GetLocalPos().x - 50;

		else
			m_vecPos.x = _pOther->GetTransform()->GetLocalPos().x + 50;

		m_pTransForm->SetLocalPos(m_vecPos);
	}
}

void CNanayaScript::Update_Client()
{
	m_pKeyCommand->Update();
	m_vecPos = m_pTransForm->GetLocalPos();
	m_vecScale = m_pTransForm->GetLocalScale();
	m_bGuard = false;

	AdjustDir();

	switch (m_eState)
	{
	case NANAYA_IDLE:						Idle();						break;
	case NANAYA_WALK:						Walk();						break;
	case NANAYA_BACKWALK:					Walk_Back();				break;
	case NANAYA_SIT:						Sit();						break;
	case NANAYA_ATTACK_Light:				Attack_Light();				break;
	case NANAYA_ATTACK_Medium:				Attack_Midium();			break;
	case NANAYA_ATTACK_Medium_Addition:		Attack_Midium_Addition();	break;
	case NANAYA_ATTACK_Power:				Attack_Power();				break;
	case NANAYA_ATTACK_SIT_Z:				Attack_Sit_Z();				break;
	case NANAYA_ATTACK_SIT_X:				Attack_Sit_X();				break;
	case NANAYA_ATTACK_JUMP_Z:				Attack_Jump_Z();			break;
	case NANAYA_ATTACK_JUMP_X:				Attack_Jump_X();			break;
	case NANAYA_ATTACK_JUMP_C:				Attack_Jump_C();			break;
	case NANAYA_FRONT_DASH:					Front_Dash();				break;
	case NANAYA_BACK_DASH:					Back_Dash();				break;
	case NANAYA_BACK_DUMBLING:				Back_Dumbling();			break;
	case NANAYA_JUMP_STAY:					Jump_Stay();				break;
	case NANAYA_JUMP_FRONT:					Jump_Front();				break;
	case NANAYA_JUMP_BACK:					Jump_Back();				break;
	case NANAYA_JUMP_FORNT_DASH:			Jump_Front_Dash();			break;
	case NANAYA_JUMP_BACK_DASH:				Jump_Back_Dash();			break;
	case NANAYA_HITTED_UP:					Hitted_Up();				break;
	case NANAYA_HITTED_MIDDLE:				Hitted_Middle();			break;
	case NANAYA_HITTED_DOWN:				Hitted_Down();				break;
	case NANAYA_KNOCKDOWN:					KnockDown();				break;
	case NANAYA_STANDUP:					StandUp();					break;
	case NANAYA_PALJUMCHOONG:				PalJumChoong();				break;
	case NANAYA_CHILYA:	Chilya();													break;
	case NANAYA_PALCHUN:		Palchun();												   break;
	case NANAYA_YUKTO:		Yukto();													break;
	case NANAYA_GUIKSYNANAYA:				GuiksyNanaya();				break;
	case NANAYA_GUIKSYNANAYA_FAIL:			GuiksyNanaya_Fail();		break;
	case NANAYA_GUIKSYNANAYA_HIT:			GuiksyNanaya_Hit();			break;
	case NANAYA_GUARD:						Guard();					break;


	case NANAYA_END:
		break;
	default:
		break;
	}

	if (m_eState == NANAYA_SIT || m_eState == NANAYA_ATTACK_SIT_X || m_eState == NANAYA_ATTACK_SIT_Z)
		m_bCrouch = true;

	Gravity();
	EffectedTime();
	if (true == CKeyManager::Manager().isServer())
	{
		SendPos();
		m_vecPrevPos = m_vecPos;
	}


	m_pTransForm->SetLocalPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CNanayaScript::Update_Server()
{
	m_pKeyCommand->Update();

	AdjustDir();

	switch (m_eState)
	{
	case NANAYA_IDLE:						Idle();						break;
	case NANAYA_WALK:						Walk();						break;
	case NANAYA_BACKWALK:					Walk_Back();					break;
	case NANAYA_SIT:						Sit();						break;
	case NANAYA_ATTACK_Light:				Attack_Light();				break;
	case NANAYA_ATTACK_Medium:				Attack_Midium();				break;
	case NANAYA_ATTACK_Medium_Addition:		Attack_Midium_Addition();	break;
	case NANAYA_ATTACK_Power:				Attack_Power();				break;
	case NANAYA_ATTACK_SIT_Z:				Attack_Sit_Z();				break;
	case NANAYA_ATTACK_SIT_X:				Attack_Sit_X();				break;
	case NANAYA_ATTACK_JUMP_Z:				Attack_Jump_Z();				break;
	case NANAYA_ATTACK_JUMP_X:				Attack_Jump_X();				break;
	case NANAYA_ATTACK_JUMP_C:				Attack_Jump_C();				break;
	case NANAYA_FRONT_DASH:					Front_Dash();				break;
	case NANAYA_BACK_DASH:					Back_Dash();					break;
	case NANAYA_BACK_DUMBLING:				Back_Dumbling();				break;
	case NANAYA_JUMP_STAY:					Jump_Stay();					break;
	case NANAYA_JUMP_FRONT:					Jump_Front();				break;
	case NANAYA_JUMP_BACK:					Jump_Back();					break;
	case NANAYA_JUMP_FORNT_DASH:			Jump_Front_Dash();			break;
	case NANAYA_JUMP_BACK_DASH:				Jump_Back_Dash();			break;
	case NANAYA_HITTED_UP:					Hitted_Up();					break;
	case NANAYA_HITTED_MIDDLE:				Hitted_Middle();				break;
	case NANAYA_HITTED_DOWN:				Hitted_Down();				break;
	case NANAYA_KNOCKDOWN:					KnockDown();					break;
	case NANAYA_STANDUP:					StandUp();					break;
	case NANAYA_PALJUMCHOONG:				PalJumChoong();				break;
	case NANAYA_CHILYA:	Chilya();													break;
	case NANAYA_PALCHUN:		Palchun();												   break;
	case NANAYA_YUKTO:		Yukto();													break;
	case NANAYA_GUIKSYNANAYA:				GuiksyNanaya();				break;
	case NANAYA_GUIKSYNANAYA_FAIL:			GuiksyNanaya_Fail();			break;
	case NANAYA_GUIKSYNANAYA_HIT:			GuiksyNanaya_Hit();			break;
	case NANAYA_GUARD:						Guard();						break;


	case NANAYA_END:
		break;
	default:
		break;
	}

	if (m_eState == NANAYA_SIT || m_eState == NANAYA_ATTACK_SIT_X || m_eState == NANAYA_ATTACK_SIT_Z)
		m_bCrouch = true;

	EffectedTime();
	Gravity();
	SendState();
	m_ePrevState = m_eState;
}



void CNanayaScript::SendState()
{
	if (m_ePrevState == m_eState)
		return;

	PACKET_STATE tPacket;
	tPacket.bPlayer = m_b1Player;
	tPacket.eState = (UINT)m_eState;
	ServerEngine::CClientManager::Manager().Write(PH_STATE, (char*)&tPacket, sizeof(tPacket));
}

void CNanayaScript::AdjustDir()
{
	m_vecPos = m_pTransForm->GetLocalPos();
	m_vecScale = m_pTransForm->GetLocalScale();

	if (m_b1Player)
		CEffectManager::GetInst()->FollowPos(L"1P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);
	else
		CEffectManager::GetInst()->FollowPos(L"2P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);

	if (m_iDir == DIR_RIGHT)
	{
		if (m_vecScale.x <= 0)
		{
			m_vecScale.x *= -1;
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		if (m_vecScale.x >= 0)
		{
			m_vecScale.x *= -1;
		}
	}

	m_pTransForm->SetLocalPos(m_vecPos);
	m_pTransForm->SetLocalScale(m_vecScale);
}

void CNanayaScript::Idle()
{
	m_bGuard = false;
	m_fKnockback = 0.f;
	m_fStunTime = 0.f;
	m_fStunHitTime = 0.f;
	m_fJumpSpeed = 0.f;
	m_bSound = false;
	m_bEffect = false;
	// 움직임
	// 오른쪽

	if (m_iDir == DIR_RIGHT)
	{
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_WALK;
			m_pAnimator->ChangeAnimation(L"Walk");

			//커맨드 움직임
			if (true == m_pKeyCommand->GetCommand(L"RR"))
			{
				m_eState = NANAYA_FRONT_DASH;
				m_pAnimator->ChangeAnimation(L"앞 대쉬", 0, -1, 0);
			}

			m_pKeyCommand->SetKey(m_tKey.Right);
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_BACKWALK;
			m_pAnimator->ChangeAnimation(L"뒤로 걷기", 1);

			if (true == m_pKeyCommand->GetCommand(L"LL", false))
			{
				m_eState = NANAYA_BACK_DASH;
				m_pAnimator->ChangeAnimation(L"백스탭", 0, -1, 0);
			}
			m_pKeyCommand->SetKey(m_tKey.Left);
		}
	}

	//왼쪽
	else if (m_iDir == DIR_LEFT)
	{

		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_WALK;
			m_pAnimator->ChangeAnimation(L"Walk");

			//커맨드 움직임
			if (true == m_pKeyCommand->GetCommand(L"LL"))
			{
				m_eState = NANAYA_FRONT_DASH;
				m_pAnimator->ChangeAnimation(L"앞 대쉬", 0, -1, 0);
			}

			m_pKeyCommand->SetKey(m_tKey.Left);
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_BACKWALK;
			m_pAnimator->ChangeAnimation(L"뒤로 걷기", 1);

			if (true == m_pKeyCommand->GetCommand(L"RR", false))
			{
				m_eState = NANAYA_BACK_DASH;
				m_pAnimator->ChangeAnimation(L"백스탭", 0, -1, 0);
			}

			m_pKeyCommand->SetKey(m_tKey.Right);
		}
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_PUSH, m_b1Player))
	{
		m_eState = NANAYA_SIT;
		m_pAnimator->ChangeAnimation(L"앉기", 4, 4, 0);

		//m_pKeyCommand->Clear();
		m_pKeyCommand->SetKey(m_tKey.Down);
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_PUSH, m_b1Player))
	{
		if (m_bOnGround == true)
		{
			m_eState = NANAYA_JUMP_STAY;
			m_pAnimator->ChangeAnimation(L"제자리 점프", 14, -1, 0);
			m_bOnGround = false;
		}
	}

	// 공격
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Light;
		m_pAnimator->ChangeAnimation(L"약공격");
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Medium;
		m_pAnimator->ChangeAnimation(L"중공격", 0, -1, 0);

		//Command
		m_pKeyCommand->SetKey(m_tKey.X);
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Power;
		m_pAnimator->ChangeAnimation(L"강공격");
	}

	if (m_pKeyCommand->GetCommand(L"DRC") || m_pKeyCommand->GetCommand(L"DLC"))
	{
		m_eState = NANAYA_PALJUMCHOONG;
		m_pAnimator->ChangeAnimation(L"팔점충", 7, -1, 0);
		m_fAccTime = 0;
	}

	if (m_pKeyCommand->GetCommand(L"DRZ")&&m_iDir==DIR_RIGHT 
		|| m_pKeyCommand->GetCommand(L"DLZ") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_PALCHUN;
		m_pAnimator->ChangeAnimation(L"섬소 팔천", 14, -1, 0);
		m_fAccTime = 0;
	}
	else if (m_pKeyCommand->GetCommand(L"DRX") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLX") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_CHILYA;
		m_pAnimator->ChangeAnimation(L"섬소 칠야", 12, -1, 0);
		m_fAccTime = 0;
	}
	else if (((m_pKeyCommand->GetCommand(L"DLX") || m_pKeyCommand->GetCommand(L"DLZ")) && m_iDir == DIR_RIGHT)
		|| ((m_pKeyCommand->GetCommand(L"DRX") || m_pKeyCommand->GetCommand(L"DRZ")) && m_iDir == DIR_LEFT))
	{
			m_eState = NANAYA_YUKTO;
			m_pAnimator->ChangeAnimation(L"섬주 육토", 14, -1, 0);
			m_fAccTime = 0;
	}
	else if (m_pKeyCommand->GetCommand(L"DLDRC") || m_pKeyCommand->GetCommand(L"DRDLC"))
	{
		m_eState = NANAYA_GUIKSYNANAYA;
		m_pAnimator->ChangeAnimation(L"극사나나야", 19, 21, 0);
		m_fAccTime = 0;

		CGameObject* pMap = CPlayerManager::GetInst()->GetMap();
		if (pMap != nullptr)
			pMap->GetMeshRender()->SetMaterial(CResManager::Manager().FindMaterial(L"Material_Std2D_Reverse"));

		//CPlayerManager::GetInst()->SetPause(5.f);
	}
}

void CNanayaScript::Walk()
{
	//움직임

	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x += m_fWalkSpeed * DT;
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{

		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_UP, m_b1Player))
		{
			m_eState = NANAYA_IDLE;
			m_pAnimator->ChangeAnimation(L"Idle");
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_BACKWALK;
			m_pAnimator->ChangeAnimation(L"뒤로 걷기");
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x -= m_fWalkSpeed * DT;
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{

		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_UP, m_b1Player))
		{
			m_eState = NANAYA_IDLE;
			m_pAnimator->ChangeAnimation(L"Idle");
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_BACKWALK;
			m_pAnimator->ChangeAnimation(L"뒤로 걷기");
		}
	}

	//공격
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Light;
		m_pAnimator->ChangeAnimation(L"약공격");
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Medium;
		m_pAnimator->ChangeAnimation(L"중공격");
		m_pKeyCommand->SetKey(m_tKey.X);
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Power;
		m_pAnimator->ChangeAnimation(L"강공격");
	}
	if (m_pKeyCommand->GetCommand(L"DRC"))
	{
		m_eState = NANAYA_PALJUMCHOONG;
		m_pAnimator->ChangeAnimation(L"팔점충", 7, -1, 0);
		m_fAccTime = 0;
	}

	if (m_pKeyCommand->GetCommand(L"DRZ") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLZ") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_PALCHUN;
		m_pAnimator->ChangeAnimation(L"섬소 팔천", 14, -1, 0);
		m_fAccTime = 0;
	}
	else if (m_pKeyCommand->GetCommand(L"DRX") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLX") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_CHILYA;
		m_pAnimator->ChangeAnimation(L"섬소 칠야", 12, -1, 0);
		m_fAccTime = 0;
	}
	else if (((m_pKeyCommand->GetCommand(L"LRX") || m_pKeyCommand->GetCommand(L"LRZ")) && m_iDir == DIR_RIGHT)
		|| ((m_pKeyCommand->GetCommand(L"RLX") || m_pKeyCommand->GetCommand(L"RLZ")) && m_iDir == DIR_LEFT))
	{
			m_eState = NANAYA_YUKTO;
			m_pAnimator->ChangeAnimation(L"섬주 육토", 14, -1, 0);
			m_fAccTime = 0;
	}
	//점프
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_PUSH, m_b1Player))
	{
		if (m_bOnGround == true)
		{
			m_eState = NANAYA_JUMP_FRONT;
			m_pAnimator->ChangeAnimation(L"앞 점프", 12, 13, 0);
			m_bOnGround = false;
		}
	}

	else if (m_pKeyCommand->GetCommand(L"DLDRC") || m_pKeyCommand->GetCommand(L"DRDLC"))
	{
		m_eState = NANAYA_GUIKSYNANAYA;
		m_pAnimator->ChangeAnimation(L"극사나나야", 19, 21, 0);
		m_fAccTime = 0;

		CGameObject* pMap = CPlayerManager::GetInst()->GetMap();
		if (pMap != nullptr)
			pMap->GetMeshRender()->SetMaterial(CResManager::Manager().FindMaterial(L"Material_Std2D_Reverse"));
	}
}

void CNanayaScript::Walk_Back()
{
	m_bGuard = true;
	//움직임
	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x -= m_fWalkSpeed * DT;
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{

		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_UP, m_b1Player))
		{
			m_bGuard = false;
			m_eState = NANAYA_IDLE;
			m_pAnimator->ChangeAnimation(L"Idle");
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_WALK;
			m_pAnimator->ChangeAnimation(L"Walk", 1);
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x += m_fWalkSpeed * DT;
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
		{

		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_UP, m_b1Player))
		{
			m_bGuard = false;
			m_eState = NANAYA_IDLE;
			m_pAnimator->ChangeAnimation(L"Idle");
		}
		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
		{
			m_eState = NANAYA_WALK;
			m_pAnimator->ChangeAnimation(L"Walk", 1);
		}
	}

	//공격
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Light;
		m_pAnimator->ChangeAnimation(L"약공격");
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Medium;
		m_pAnimator->ChangeAnimation(L"중공격");
		m_pKeyCommand->SetKey(m_tKey.X);
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Power;
		m_pAnimator->ChangeAnimation(L"강공격");
	}

	//점프
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_PUSH, m_b1Player))
	{
		if (m_bOnGround == true)
		{
			m_eState = NANAYA_JUMP_BACK;
			m_pAnimator->ChangeAnimation(L"백점프", 28, -1, 0);
			m_bOnGround = false;
		}
	}

	if (m_pKeyCommand->GetCommand(L"DRZ") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLZ") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_PALCHUN;
		m_pAnimator->ChangeAnimation(L"섬소 팔천", 14, -1, 0);
		m_fAccTime = 0;
	}
	else if (m_pKeyCommand->GetCommand(L"DRX") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLX") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_CHILYA;
		m_pAnimator->ChangeAnimation(L"섬소 칠야", 12, -1, 0);
		m_fAccTime = 0;
	}
	else if (((m_pKeyCommand->GetCommand(L"DLX") || m_pKeyCommand->GetCommand(L"DLZ")) && m_iDir == DIR_RIGHT)
		||( (m_pKeyCommand->GetCommand(L"DRX") || m_pKeyCommand->GetCommand(L"DRZ")) && m_iDir == DIR_LEFT))
	{
			m_eState = NANAYA_YUKTO;
			m_pAnimator->ChangeAnimation(L"섬주 육토", 14, -1, 0);
			m_fAccTime = 0;
	}
}

void CNanayaScript::Sit()
{
	if (KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
		|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)
	{
		m_bGuard = true;
	}

	//누르고 있을땐 아무작없도 하지 않는다
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_PUSH, m_b1Player))
	{

	}
	// Idle로 돌아간다
	else if (9 == m_pAnimator->GetCurAnim()->GetCurIndex())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
	// 일어난다
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_UP, m_b1Player))
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		//m_pAnimator->ChangeAnimation(L"앉기", 9);
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_SIT_Z;
		m_pAnimator->ChangeAnimation(L"앉아 공격", 5, -1, 0);
	}
	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_SIT_X;
		m_pAnimator->ChangeAnimation(L"앉아 발차기", 10, -1, 0);
	}
	
	if (m_pKeyCommand->GetCommand(L"DRZ") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLZ") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_PALCHUN;
		m_pAnimator->ChangeAnimation(L"섬소 팔천", 14, -1, 0);
		m_fAccTime = 0;
	}
	else if (m_pKeyCommand->GetCommand(L"DRX") && m_iDir == DIR_RIGHT
		|| m_pKeyCommand->GetCommand(L"DLX") && m_iDir == DIR_LEFT)
	{
		m_eState = NANAYA_CHILYA;
		m_pAnimator->ChangeAnimation(L"섬소 칠야", 12, -1, 0);
		m_fAccTime = 0;
	}
	else if (((m_pKeyCommand->GetCommand(L"DLX")|| m_pKeyCommand->GetCommand(L"DLZ")) && m_iDir == DIR_RIGHT)
		|| ((m_pKeyCommand->GetCommand(L"DRX") || m_pKeyCommand->GetCommand(L"DRZ")) && m_iDir == DIR_LEFT))
	{
			m_eState = NANAYA_YUKTO;
			m_pAnimator->ChangeAnimation(L"섬주 육토", 14, -1, 0);
			m_fAccTime = 0;
	}
}

void CNanayaScript::Attack_Light()
{
	m_eHitState = HIT_UP;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA19.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_Light;
		m_pAnimator->ChangeAnimation(L"약공격");
		PACKET_STATE tPacket;
		tPacket.bPlayer = m_b1Player;
		tPacket.eState = (UINT)m_eState;
		if(m_bOnline)
		ServerEngine::CClientManager::Manager().Write(PH_STATE, (char*)&tPacket, sizeof(tPacket));
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Attack_Midium()
{
	m_eHitState = HIT_MIDDLE;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA17.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_bSound = false;
		m_eState = NANAYA_ATTACK_Medium_Addition;
		m_pAnimator->ChangeAnimation(L"중공격 추가타", 10, -1, 0);
	}
}

void CNanayaScript::Attack_Midium_Addition()
{
	m_eHitState = HIT_AIRUP;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA18.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (m_pAnimator->GetCurAnim()->GetCurIndex() == 3)
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"중공격추가타", 8, -1, 0);
}

void CNanayaScript::Attack_Power()
{
	m_eHitState = HIT_MIDDLE;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA21.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (m_pAnimator->GetCurAnim()->GetCurIndex() == 4)
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"강공격", 5, -1, 0);
}

void CNanayaScript::Attack_Sit_Z()
{
	m_eHitState = HIT_DOWN;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA19.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_SIT_Z;
		m_pAnimator->ChangeAnimation(L"앉아 공격", 5, -1, 0);
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_SIT;
		m_pAnimator->ChangeAnimation(L"앉기", 4, 4, 4);
	}
}

void CNanayaScript::Attack_Sit_X()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA17.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	m_eHitState = HIT_DOWN;
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_SIT;
		m_pAnimator->ChangeAnimation(L"앉기", 4, 4, 4);
	}
}

void CNanayaScript::Attack_Jump_Z()
{
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x -= m_fWalkSpeed * DT;
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x += m_fWalkSpeed * DT;
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Attack_Jump_X()
{
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x -= m_fWalkSpeed * DT;
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x += m_fWalkSpeed * DT;
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (m_pAnimator->GetCurAnim()->GetCurIndex() == 3)
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"점프공격2", 7, -1, 0);
}

void CNanayaScript::Attack_Jump_C()
{
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x -= m_fWalkSpeed * DT;
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player))
	{
		m_vecPos.x += m_fWalkSpeed * DT;
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (m_pAnimator->GetCurAnim()->GetCurIndex() == 3)
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"점프공격3", 5, -1, 0);
}

void CNanayaScript::Front_Dash()
{
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Back_Dash()
{
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}

	if (m_iDir == DIR_RIGHT)
	{
		if (true == m_pKeyCommand->GetCommand(L"LLL"))
		{
			m_eState = NANAYA_BACK_DUMBLING;
			m_pAnimator->ChangeAnimation(L"백 덤블링", 16, -1, 0);
		}
	}
	else if (m_iDir == DIR_LEFT)
	{
		if (true == m_pKeyCommand->GetCommand(L"RRR"))
		{
			m_eState = NANAYA_BACK_DUMBLING;
			m_pAnimator->ChangeAnimation(L"백 덤블링", 16, -1, 0);
		}
	}
}

void CNanayaScript::Back_Dumbling()
{
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Jump_Stay()
{
	if (m_iBeforeIdx <= 4)
		m_bOnGround = false;

	m_fJumpSpeed = 0;

	if (m_iDir == DIR_RIGHT)
	{
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}
	}


	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_Z;
		m_pAnimator->ChangeAnimation(L"점프공격Z");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_X;
		m_pAnimator->ChangeAnimation(L"점프공격X");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_C;
		m_pAnimator->ChangeAnimation(L"점프공격C");
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Jump_Front()
{
	if (m_iBeforeIdx <= 4)
		m_bOnGround = false;

	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x += m_fWalkSpeed * DT;

		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}
	}

	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x -= m_fWalkSpeed * DT;

		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}
	}

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_Z;
		m_pAnimator->ChangeAnimation(L"점프공격Z");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_X;
		m_pAnimator->ChangeAnimation(L"점프공격X");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_C;
		m_pAnimator->ChangeAnimation(L"점프공격C");
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Jump_Back()
{
	if (m_iBeforeIdx <= 4)
		m_bOnGround = false;

	if (DIR_RIGHT == m_iDir)
	{
		m_vecPos.x -= m_fWalkSpeed * DT;

		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}
	}

	else if (DIR_LEFT == m_iDir)
	{
		m_vecPos.x += m_fWalkSpeed * DT;

		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_BACK_DASH;
			m_pAnimator->ChangeAnimation(L"백점프", 20, 21, 7);
			m_fGravity = 600;
		}

		else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		{
			m_eState = NANAYA_JUMP_FORNT_DASH;
			m_pAnimator->ChangeAnimation(L"공중대쉬", 4, -1, 0);
			m_fGravity = 0;
		}
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_Z;
		m_pAnimator->ChangeAnimation(L"점프공격Z");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_X;
		m_pAnimator->ChangeAnimation(L"점프공격X");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_C;
		m_pAnimator->ChangeAnimation(L"점프공격C");
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Jump_Front_Dash()
{
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_Z;
		m_pAnimator->ChangeAnimation(L"점프공격Z");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_X;
		m_pAnimator->ChangeAnimation(L"점프공격X");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_C;
		m_pAnimator->ChangeAnimation(L"점프공격C");
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Jump_Back_Dash()
{
	if (m_iDir == DIR_RIGHT)
		m_vecPos.x -= (m_fWalkSpeed + 150) * DT;

	else if (m_iDir == DIR_LEFT)
		m_vecPos.x += (m_fWalkSpeed + 150) * DT;

	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_Z;
		m_pAnimator->ChangeAnimation(L"점프공격Z");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_X;
		m_pAnimator->ChangeAnimation(L"점프공격X");
	}

	else if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
	{
		m_eState = NANAYA_ATTACK_JUMP_C;
		m_pAnimator->ChangeAnimation(L"점프공격C");
	}

	if (true == m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Hitted_Up()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA66.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x -= m_fKnockback * DT;
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x += m_fKnockback * DT;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Hitted_Middle()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA64.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}
	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x -= m_fKnockback * DT;
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x += m_fKnockback * DT;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		m_fKnockback = 0;
	}
}

void CNanayaScript::Hitted_Down()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA62.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}
	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x -= m_fKnockback * DT;
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x += m_fKnockback * DT;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::KnockDown()
{
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA65.OGG");
		m_pSound->Play(1);
		
		m_bSound = true;
	}

	if (m_bOnGround)
	{
		if (!m_bEffect)
		{
			if (iCurFrame > 0)
			{
				CEffectManager::GetInst()->ActiveEffect(L"Circle2", m_vecPos + Vec3(0.f, -110.f, 0.f));

				CSound* pSound = CResManager::Manager().FindSound(L"common_00046.wav");
				pSound->Play(1);
				m_bEffect = true;
			}
		}
	}

	m_vecPos = GetTransform()->GetLocalPos();

	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x -= m_fKnockback * DT;
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x += m_fKnockback * DT;
	}

	GetTransform()->SetLocalPos(m_vecPos);

	if (m_bOnGround)
		m_fKnockback = 0.f;

	if (true == m_pAnimator->IsAnimFinish())
	{
		if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_PUSH, m_b1Player)
			|| true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_PUSH, m_b1Player)
			|| true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_PUSH, m_b1Player)
			|| true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_PUSH, m_b1Player))
		{
			m_bSound = false;
			m_eState = NANAYA_STANDUP;
			m_pAnimator->ChangeAnimation(L"일어서기2", 5, -1, 0);
		}
	}
}

void CNanayaScript::StandUp()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA69.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::PalJumChoong()
{
	m_eHitState = HIT_MIDDLE;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA27.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (m_iDir == DIR_RIGHT)
	{
		m_vecPos.x += (m_fWalkSpeed / 2) * DT;
		if (m_iBeforeIdx >= 6)
		{
			CEffectManager::GetInst()->FollowPos(L"3점", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"4점", Vec3(m_vecPos.x + 150, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"5점", Vec3(m_vecPos.x + 123, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"6점", Vec3(m_vecPos.x + 70, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"7점", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"8점", Vec3(m_vecPos.x + 100, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"10점", Vec3(m_vecPos.x + 50, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"11점", Vec3(m_vecPos.x + 50, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"12점", Vec3(m_vecPos.x + 50, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"13점", Vec3(m_vecPos.x + 50, m_vecPos.y, m_vecPos.z));
		}
	}
	else if (m_iDir == DIR_LEFT)
	{
		m_vecPos.x -= (m_fWalkSpeed / 2) * DT;
		if (m_iBeforeIdx >= 6)
		{
			CEffectManager::GetInst()->FollowPos(L"3점", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"4점", Vec3(m_vecPos.x - 150, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"5점", Vec3(m_vecPos.x - 123, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"6점", Vec3(m_vecPos.x - 70, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"7점", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"8점", Vec3(m_vecPos.x - 100, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"10점", Vec3(m_vecPos.x - 50, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"11점", Vec3(m_vecPos.x - 50, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"12점", Vec3(m_vecPos.x - 50, m_vecPos.y, m_vecPos.z), false);
			CEffectManager::GetInst()->FollowPos(L"13점", Vec3(m_vecPos.x - 50, m_vecPos.y, m_vecPos.z), false);
		}
	}

	m_fAccTime += DT;
	if (m_fAccTime >= 4)
	{
		//bOnce = false;
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
		m_fAccTime = 0;
		CEffectManager::GetInst()->StopEffect(L"3점");
		CEffectManager::GetInst()->StopEffect(L"4점");
		CEffectManager::GetInst()->StopEffect(L"5점");
		CEffectManager::GetInst()->StopEffect(L"6점");
		CEffectManager::GetInst()->StopEffect(L"7점");
		CEffectManager::GetInst()->StopEffect(L"8점");
	}

}

void CNanayaScript::Chilya()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA37.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	m_eHitState = HIT_DOWN;
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (iCurFrame == 7)
	CEffectManager::GetInst()->FollowPos(L"Nanaya_칠야", m_vecPos, m_iDir + 1);
	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Palchun()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA13.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	m_eHitState = HIT_UP;
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if(iCurFrame==1)
	CEffectManager::GetInst()->FollowPos(L"Nanaya_팔천", m_vecPos+Vec3(m_iDir*100.f,0.f,0.f), m_iDir+1);

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::Yukto()
{
	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA41.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	m_eHitState = HIT_MIDDLE;
	if(m_iBeforeIdx<=1)
		m_bOnGround = false;

	m_fJumpSpeed = 1000.f;

	if (true == m_pAnimator->IsAnimFinish()||m_bOnGround)
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::GuiksyNanaya()
{
	if (!m_bSound)
	{
		CEffectManager::GetInst()->ActiveEffect(L"SuperSpark1", m_vecPos);

		CSound* pSound = CResManager::Manager().FindSound(L"NANAYA80.OGG");
		pSound->Play(1);
		m_pSound = CResManager::Manager().FindSound(L"NANAYA74.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	CGameManager::GetInst()->GetSound()->Stop(true);

	m_fAccTime += DT;
	if (m_fAccTime >= 5)
	{
		m_eState = NANAYA_GUIKSYNANAYA_FAIL;
		m_pAnimator->ChangeAnimation(L"극사나나야", 41, -1, 34);
		m_fAccTime = 0;
	}
}

void CNanayaScript::GuiksyNanaya_Fail()
{
	CGameManager::GetInst()->GetSound()->Stop(false);
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	if (iCurFrame == 34&&m_bSound)
		m_bSound = false;

	if (!m_bSound)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA69.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");

		CGameObject* pMap = CPlayerManager::GetInst()->GetMap();
		if (pMap != nullptr)
			pMap->GetMeshRender()->SetMaterial(CResManager::Manager().FindMaterial(L"Material_Std2D"));
	}
}

void CNanayaScript::GuiksyNanaya_Hit()
{
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	if (iCurFrame == 22 && m_bSound)
		m_bSound = false;

	if (!m_bSound&&iCurFrame==22)
	{
		m_pSound = CResManager::Manager().FindSound(L"NANAYA75.OGG");
		m_pSound->Play(1);
		m_bSound = true;
	}

	if (true == m_pAnimator->IsAnimFinish())
	{
		CGameManager::GetInst()->GetSound()->Stop(false);
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");

		CGameObject* pMap = CPlayerManager::GetInst()->GetMap();
		if (pMap != nullptr)
			pMap->GetMeshRender()->SetMaterial(CResManager::Manager().FindMaterial(L"Material_Std2D"));
	}
}

void CNanayaScript::Guard()
{
	m_bGuard = true;

	if (2 == GetAnimatorEX()->GetCurAnim()->GetCurIndex())
	{
		m_bGuard = false;
		m_eState = NANAYA_IDLE;
		m_pAnimator->ChangeAnimation(L"Idle");
	}
}

void CNanayaScript::OnCollisionEnter(CCollider * _pOther)
{
	if (m_bOnline == false)
		OnColisionEnter_Client(_pOther);

	else if (m_bOnline == true)
		OnColisionEnter_Server(_pOther);
}

void CNanayaScript::OnColisionEnter_Client(CCollider * _pOther)
{
	PACKET_STATE tPacket;
	tPacket.bPlayer = m_b1Player;

	CGameObject* m_pOtherPlayer = NULL;
	CCharactorScript* m_pOtherPlayerScript = NULL;
	HIT_STATE eOtherHitState = HIT_MIDDLE;

	if (_pOther->GetGameObject()->GetParent() != NULL)
	{
		m_pOtherPlayer = _pOther->GetGameObject()->GetParent();
		eOtherHitState = ((CCharactorScript*)m_pOtherPlayer->GetFirstScript())->GetHitState();
		m_pOtherPlayerScript = ((CCharactorScript*)m_pOtherPlayer->GetFirstScript());
	}
	else
	{
		m_pOtherPlayer = _pOther->GetGameObject();
	}

	if (m_bGuard == true && (eOtherHitState == HIT_UP || eOtherHitState == HIT_MIDDLE))
	{
		m_eState = NANAYA_GUARD;
		GetAnimatorEX()->ChangeAnimation(L"서서막기", 2, -1, 0);
		Vec3 vecShild = m_vecPos;
		if (m_iDir == DIR_RIGHT)
		{
			vecShild.x += 10;
		}
		else if (m_iDir == DIR_LEFT)
		{
			vecShild.x -= 10;
		}
		CEffectManager::GetInst()->ActiveEffect(L"나나야방어", vecShild);

		if (CKeyManager::Manager().isServer() == true)
		{
			tPacket.eState = (UINT)m_eState;
			ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_STATE, (char*)&tPacket, sizeof(tPacket));
		}
		return;
	}

	if (HIT_UP == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_UP;
		m_pAnimator->ChangeAnimation(L"상단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}
	}
	else if (HIT_MIDDLE == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_MIDDLE;
		m_pAnimator->ChangeAnimation(L"중단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}
	}
	else if (HIT_DOWN == eOtherHitState)
	{
		m_eState = NANAYA_HITTED_DOWN;
		m_pAnimator->ChangeAnimation(L"하단피격", 7, -1, 0);

		if (m_bOnGround == false)
		{
			m_fGravity = 0;
			m_fJumpSpeed = 0;

			m_eState = NANAYA_KNOCKDOWN;
			m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		}
	}
	else if (HIT_AIRUP == eOtherHitState)
	{
		m_eState = NANAYA_KNOCKDOWN;
		m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
		m_bOnGround = false;
	}

	else if (HIT_KNOCKDOWN == eOtherHitState)
	{
		m_eState = NANAYA_KNOCKDOWN;
		m_pAnimator->ChangeAnimation(L"넉다운", 8, -1, 0);
	}

	if (CKeyManager::Manager().isServer() == true) 
	{
		tPacket.eState = (UINT)m_eState;
		ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_STATE, (char*)&tPacket, sizeof(tPacket));
	}
}

void CNanayaScript::OnColisionEnter_Server(CCollider * _pOther)
{

}