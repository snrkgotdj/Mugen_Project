#include "stdafx.h"

#include "PlayerScKyo.h"

#include "Collider.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "GameObject.h"

#include "ShaderManager.h"
#include "ResManager.h"
#include "SceneManager.h"

#include "EffectManager.h"
#include "PlayerManager.h"
#include "UIManager.h"
#include "SoundLoadMgr.h"

#include "Scene.h"
#include "Layer.h"

#include "Material.h"

#include "CameraScript.h"
#include "KeyCommand.h"
#include "ScriptCode\ShadowScript.h"
#include "..\Doom\define_server.h"
#include "NetworkManager.h"
#include "ClientManager.h"

CPlayerScKyo::CPlayerScKyo()
	: m_fTime(0.f)
	, m_iCmdLinkCnt(0)
	, m_bEffect(false)
	, m_bSound(false)
	, m_bPracticeMode(false)
	, m_eState(KYO_IDLE)
	, m_ePreInput(KYO_IDLE)
	, m_ePrevState(KYO_IDLE)
	, m_pSound(NULL)
	, m_fDashCooldown(0.f)
	, m_fCharge(0.f)
{
	m_fJumpSpeed = 1500;
	m_fGravAccel = 4000;
	m_iDir = DIR_RIGHT;
	m_iBeforeIdx = -1;
	m_vecPos = Vec3Zero;
	m_bDefeated = false;
}


CPlayerScKyo::~CPlayerScKyo()
{
}

void CPlayerScKyo::Awake()
{
	m_fFloorY = -20.f;
	GetTransform()->SetLocalScale(Vec3(300.f, 300.f, 1.f));

	m_pEffect = GetGameObject()->GetChildByName(L"KyoEffect");
	m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);
}

void CPlayerScKyo::Start()
{
	m_fFloorY = -20.f;
	GetTransform()->SetLocalScale(Vec3(300.f, 300.f, 1.f));
	m_vecPos = GetTransform()->GetLocalPos();

	if (CUIManager::GetInst()->GetTraining()||m_bOnline)
	{
		m_tKey.Up = KEY_UP;
		m_tKey.Down = KEY_DOWN;
		m_tKey.Left = KEY_LEFT;
		m_tKey.Right = KEY_RIGHT;
		m_tKey.Z = KEY_Z;
		m_tKey.X = KEY_X;
		m_tKey.C = KEY_C;
		m_tKey.A = KEY_A;
		m_tKey.S = KEY_S;
		m_tKey.D = KEY_D;
	}
	else
	{
		if (CPlayerManager::GetInst()->Get1P() == GetGameObject())
		{
			m_tKey.Up = KEY_T;
			m_tKey.Down = KEY_G;
			m_tKey.Left = KEY_F;
			m_tKey.Right = KEY_H;
			m_tKey.Z = KEY_A;
			m_tKey.X = KEY_S;
			m_tKey.C = KEY_D;
			m_tKey.A = KEY_Q;
			m_tKey.S = KEY_W;
			m_tKey.D = KEY_E;
		}
		else
		{
			m_tKey.Up = KEY_UP;
			m_tKey.Down = KEY_DOWN;
			m_tKey.Left = KEY_LEFT;
			m_tKey.Right = KEY_RIGHT;
			m_tKey.Z = KEY_J;
			m_tKey.X = KEY_K;
			m_tKey.C = KEY_L;
			m_tKey.A = KEY_U;
			m_tKey.S = KEY_I;
			m_tKey.D = KEY_O;
		}
	}
}

void CPlayerScKyo::Update_Client()
{
	m_fFloorY = -20.f;
	m_vecPos = GetTransform()->GetLocalPos();

	if (m_b1Player)
		CEffectManager::GetInst()->FollowPos(L"1P_UI", m_vecPos + Vec3(0.f, 70.f, 0.f), m_iDir);
	else
		CEffectManager::GetInst()->FollowPos(L"2P_UI", m_vecPos + Vec3(0.f, 70.f, 0.f), m_iDir);

	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = GetTransform()->GetLocalScale().x;

	if (GetAnimatorEX() == NULL)
		return;

	m_fDashCooldown += DT;

	if (m_iDir == DIR_LEFT)
		fDir = 300.f;
	else if (m_iDir == DIR_RIGHT)
		fDir = -300.f;

	CommandInput();//커맨드 입력;

	if (!m_vecCmd.empty())
	{
		m_fTime += DT;
		float fLimit = 0.5f;

		if ((m_eState == KYO_IDLE || m_eState == KYO_CROUCH ||
			m_eState == KYO_BACKWALK || m_eState == KYO_FRONTWALK) && m_fTime < 0.25f)
		{
			fLimit = 0.25f;
		}
		if (m_fTime > fLimit)
		{
			m_iCmdLinkCnt = 0;
			m_vecCmd.clear();
			m_fTime = 0.f;
			m_ePreInput = KYO_IDLE;
		}
	}

	if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN) ||
		INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		CommandDecode();//커맨드 판독

	if (!m_bDefeated)
	{
		switch (m_eState)
		{
		case KYO_IDLE:					Idle();					break;
		case KYO_FRONTWALK:				FrontWalk();			break;
		case KYO_BACKWALK:				BackWalk();				break;
		case KYO_FRONTDASH:				FrontDash();			break;
		case KYO_BACKDASH:				BackDash();				break;
		case KYO_CROUCH:				Crouch();				break;
		case KYO_JUMP_UP:				JumpUp();				break;
		case KYO_JUMP_DOWN:				JumpDown();				break;
		case KYO_JUMP_FRONT:			JumpFront();			break;
		case KYO_JUMP_BACK:				JumpBack();				break;
		case KYO_ROLL:					Roll();					break;
		case KYO_LP:					GroundLP();				break;
		case KYO_SP:					GroundSP();				break;
		case KYO_LK:					GroundLK();				break;
		case KYO_SK:					GroundSK();				break;
		case KYO_TYPE75_KAI:			Type75_Kai();			break;
		case KYO_REDKICK:				RedKicK();				break;
		case KYO_TYPE100_ONIYAKI:		Type100_Oniyaki();		break;
		case KYO_UTYPE108_OROCHINAGI:	TypeU108_Orochinagi();	break;
		case KYO_TYPE114_ARAGAMI:		Type114_Aragami();		break;
		case KYO_TYPE115_DOKUGAMI:		Type115_Dokugami();		break;
		case KYO_TYPE125_NANASE:		Type125_Nanase();		break;
		case KYO_TYPE127_YANOSABI:		Type127_Yanosabi();		break;
		case KYO_TYPE128_KONOKIZU:		Type128_Konokizu();		break;
		case KYO_TYPE182:				Type182();				break;
		case KYO_TYPE401_TSUMIYOMI:		Type401_Tsumiyomi();	break;
		case KYO_TYPE402_BATSUYOMI:		Type402_Batsuyomi();	break;
		case KYO_TYPE427_HIKIGANE_L:	Type427_Hikigane_L();	break;
		case KYO_TYPE427_HIKIGANE_S:	Type427_Hikigane_S();	break;
		case KYO_OUTTYPE_GOUFU:			OutType_Goufu();		break;
		case KYO_OUTTYPE_MIGIRI:		OutType_Migiri();		break;
		case KYO_NOTYPE:				NoType();				break;
		case KYO_CROUCH_LP:				CrouchLP();				break;
		case KYO_CROUCH_SP:				CrouchSP();				break;
		case KYO_CROUCH_LK:				CrouchLK();				break;
		case KYO_CROUCH_SK:				CrouchSK();				break;
		case KYO_TYPE88:				Type88();				break;
		case KYO_JUMP_LP:				JumpLP();				break;
		case KYO_JUMP_SP:				JumpSP();				break;
		case KYO_JUMP_LK:				JumpLK();				break;
		case KYO_JUMP_SK:				JumpSK();				break;
		case KYO_OUTTYPE_NARAKU:		OutType_Naraku();		break;
		case KYO_GUARD:					Guard();				break;
		case KYO_HIT_DOWN:				HitDown();				break;
		case KYO_HIT_MID:				HitMid();				break;
		case KYO_HIT_UP:				HitUp();				break;
		case KYO_HIT_AERIAL:			HitAerial();			break;
		case KYO_HIT_KNOCKDOWN:			HitKnockdown();			break;
		case KYO_HIT_STANDUP:			StandUp();				break;
		case KYO_HIT_STANDUP_QUICK:		StandUpQuick();			break;
		}
	}
	else
	{
		m_fStunHitTime = 0.f;
		m_bOnGround = true;
		//m_fJumpSpeed = 0.f;
	}

	if (m_eState == KYO_BACKWALK || (m_eState == KYO_CROUCH &&
		(KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
			|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)))
	{
		m_bGuard = true;
	}
	else
		m_bGuard = false;

	if (m_eState == KYO_CROUCH || m_eState == KYO_CROUCH_LK || m_eState == KYO_CROUCH_LP
		|| m_eState == KYO_CROUCH_SK || m_eState == KYO_CROUCH_SP)
		m_bCrouch = true;
	else
		m_bCrouch = false;

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));
	GetTransform()->SetLocalPos(m_vecPos);

	Gravity();
	EffectedTime();
	if (true == CKeyManager::Manager().isServer())
	{
		SendPos();
		m_vecPrevPos = m_vecPos;
	}

#if defined DEBUG | _DEBUG
	//char str[255];
	//GET_SINGLE(CDebugMgr)->SetResetTime(0.1f);
	//sprintf_s(str, "Cmd : (%d, %f) \n", m_vecCmd.size(), m_fTime);
	//GET_SINGLE(CDebugMgr)->OutputConsole(str);
#endif
}

void CPlayerScKyo::Update_Server()
{
	m_fFloorY = -20.f;

	if (m_b1Player)
		CEffectManager::GetInst()->FollowPos(L"1P_UI", m_vecPos + Vec3(0.f, 120.f, 0.f), m_iDir);
	else
		CEffectManager::GetInst()->FollowPos(L"2P_UI", m_vecPos + Vec3(0.f, 120.f, 0.f), m_iDir);

	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = GetTransform()->GetLocalScale().x;

	if (GetAnimatorEX() == NULL)
		return;

	m_fDashCooldown += DT;

	if (m_iDir == DIR_LEFT)
		fDir = 300.f;
	else if (m_iDir == DIR_RIGHT)
		fDir = -300.f;

	CommandInput();//커맨드 입력;

	if (!m_vecCmd.empty())
	{
		m_fTime += DT;
		float fLimit = 0.5f;

		if ((m_eState == KYO_IDLE || m_eState == KYO_CROUCH ||
			m_eState == KYO_BACKWALK || m_eState == KYO_FRONTWALK) && m_fTime < 0.25f)
		{
			fLimit = 0.25f;
		}
		if (m_fTime > fLimit)
		{
			m_iCmdLinkCnt = 0;
			m_vecCmd.clear();
			m_fTime = 0.f;
			m_ePreInput = KYO_IDLE;
		}
	}

	if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN) ||
		INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		CommandDecode();//커맨드 판독

	if (!m_bDefeated)
	{
		switch (m_eState)
		{
		case KYO_IDLE:					Idle();					break;
		case KYO_FRONTWALK:				FrontWalk();			break;
		case KYO_BACKWALK:				BackWalk();				break;
		case KYO_FRONTDASH:				FrontDash();			break;
		case KYO_BACKDASH:				BackDash();				break;
		case KYO_CROUCH:				Crouch();				break;
		case KYO_JUMP_UP:				JumpUp();				break;
		case KYO_JUMP_DOWN:				JumpDown();				break;
		case KYO_JUMP_FRONT:			JumpFront();			break;
		case KYO_JUMP_BACK:				JumpBack();				break;
		case KYO_ROLL:					Roll();					break;
		case KYO_LP:					GroundLP();				break;
		case KYO_SP:					GroundSP();				break;
		case KYO_LK:					GroundLK();				break;
		case KYO_SK:					GroundSK();				break;
		case KYO_TYPE75_KAI:			Type75_Kai();			break;
		case KYO_REDKICK:				RedKicK();				break;
		case KYO_TYPE100_ONIYAKI:		Type100_Oniyaki();		break;
		case KYO_UTYPE108_OROCHINAGI:	TypeU108_Orochinagi();	break;
		case KYO_TYPE114_ARAGAMI:		Type114_Aragami();		break;
		case KYO_TYPE115_DOKUGAMI:		Type115_Dokugami();		break;
		case KYO_TYPE125_NANASE:		Type125_Nanase();		break;
		case KYO_TYPE127_YANOSABI:		Type127_Yanosabi();		break;
		case KYO_TYPE128_KONOKIZU:		Type128_Konokizu();		break;
		case KYO_TYPE182:				Type182();				break;
		case KYO_TYPE401_TSUMIYOMI:		Type401_Tsumiyomi();	break;
		case KYO_TYPE402_BATSUYOMI:		Type402_Batsuyomi();	break;
		case KYO_TYPE427_HIKIGANE_L:	Type427_Hikigane_L();	break;
		case KYO_TYPE427_HIKIGANE_S:	Type427_Hikigane_S();	break;
		case KYO_OUTTYPE_GOUFU:			OutType_Goufu();		break;
		case KYO_OUTTYPE_MIGIRI:		OutType_Migiri();		break;
		case KYO_NOTYPE:				NoType();				break;
		case KYO_CROUCH_LP:				CrouchLP();				break;
		case KYO_CROUCH_SP:				CrouchSP();				break;
		case KYO_CROUCH_LK:				CrouchLK();				break;
		case KYO_CROUCH_SK:				CrouchSK();				break;
		case KYO_TYPE88:				Type88();				break;
		case KYO_JUMP_LP:				JumpLP();				break;
		case KYO_JUMP_SP:				JumpSP();				break;
		case KYO_JUMP_LK:				JumpLK();				break;
		case KYO_JUMP_SK:				JumpSK();				break;
		case KYO_OUTTYPE_NARAKU:		OutType_Naraku();		break;
		case KYO_GUARD:					Guard();				break;
		case KYO_HIT_DOWN:				HitDown();				break;
		case KYO_HIT_MID:				HitMid();				break;
		case KYO_HIT_UP:				HitUp();				break;
		case KYO_HIT_AERIAL:			HitAerial();			break;
		case KYO_HIT_KNOCKDOWN:			HitKnockdown();			break;
		case KYO_HIT_STANDUP:			StandUp();				break;
		case KYO_HIT_STANDUP_QUICK:		StandUpQuick();			break;
		}
	}
	else
	{
		m_fStunHitTime = 0.f;
		m_bOnGround = true;
		//m_fJumpSpeed = 0.f;
	}

	if (m_eState == KYO_BACKWALK || (m_eState == KYO_CROUCH &&
		(KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
			|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)))
	{
		m_bGuard = true;
	}
	else
		m_bGuard = false;

	if (m_eState == KYO_CROUCH || m_eState == KYO_CROUCH_LK || m_eState == KYO_CROUCH_LP
		|| m_eState == KYO_CROUCH_SK || m_eState == KYO_CROUCH_SP)
		m_bCrouch = true;
	else
		m_bCrouch = false;

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));

	Gravity();
	EffectedTime();
	SendState();

	m_ePrevState = m_eState;
}

void CPlayerScKyo::SendState()
{
	if (m_ePrevState == m_eState)
		return;

	PACKET_STATE tPacket;
	tPacket.bPlayer = m_b1Player;
	tPacket.eState = (UINT)m_eState;
	ServerEngine::CClientManager::Manager().Write(PH_STATE, (char*)&tPacket, sizeof(tPacket));
}

void CPlayerScKyo::Update()
{
	if (m_bOnline == false)
		Update_Client();

	else if (m_bOnline == true)
		Update_Server();
}

void CPlayerScKyo::LateUpdate()
{
	m_vecPos =GetTransform()->GetLocalPos();

	if (GetAnimatorEX() == NULL)
		return;

	if (m_vecPos.y <= m_fFloorY)
	{
		m_vecPos.y = m_fFloorY;
		m_fGravity = 0.f;
		m_fJumpSpeed = 0.f;
		m_bOnGround = true;
	}

	else
		m_bOnGround = false;

	GetTransform()->SetLocalPos(m_vecPos);
}

void CPlayerScKyo::FinalUpdate()
{
	m_vecPos = GetTransform()->GetLocalPos();

	m_vecPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->fSpeed
		*m_iDir*((2.f + m_fCharge) / 2.f)* DT;
	m_vecPos.x -= m_fKnockback*m_iDir*DT;

	if (m_iBeforeIdx != GetAnimatorEX()->GetCurAnim()->GetCurIndex())
	{
		if (m_iDir == DIR_RIGHT)
		{
			m_vecPos.x -= GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}
		else if (m_iDir == DIR_LEFT)
		{
			m_vecPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}

		m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	}

	GetTransform()->SetLocalPos(m_vecPos);

	m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	CCameraScript* CamScript = (CCameraScript*)CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetFirstScript();

	if (CamScript == NULL)
		return;

	if (m_vecPos.x >= CamScript->GetMapScale().x / 2 - 50)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			m_vecPos.x = CamScript->GetMapScale().x / 2 - 50;
			GetTransform()->SetLocalPos(m_vecPos);
		}
	}

	if (m_vecPos.x <= -CamScript->GetMapScale().x / 2 + 50)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			m_vecPos.x = -CamScript->GetMapScale().x / 2 + 50;
			GetTransform()->SetLocalPos(m_vecPos);
		}
	}
}

void CPlayerScKyo::OnCollisionEnter(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = NULL;
	HIT_STATE eOtherHitState = HIT_UP;
	if (_pOther->GetGameObject()->GetParent() != NULL)
	{
		pOtherPlayer = _pOther->GetGameObject()->GetParent();
		eOtherHitState = ((CCharactorScript*)pOtherPlayer->GetFirstScript())->GetHitState();
	}
	else
	{
		pOtherPlayer = _pOther->GetGameObject();
	}

	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();//역가드 판정용

	float fOtherColPosY = _pOther->GetCollider()->GetWorldPos().y;

	if (m_bGuard == true)
	{
		if (((m_eState == KYO_BACKWALK||m_eState==KYO_IDLE) 
			&& ((fOtherColPosY > -45.f&& _pOther->GetGameObject()->HasParent()==true)
				||(_pOther->GetGameObject()->HasParent()==false)))
			|| m_eState == KYO_CROUCH || m_eState == KYO_GUARD)
		{
			m_eState = KYO_GUARD;
			GetAnimatorEX()->ChangeAnimation(L"Guard", 2, -1, 0);
			Vec3 vecShild = GetTransform()->GetLocalPos();
			if (m_iDir == DIR_RIGHT)
			{
				vecShild.x += 10;
			}
			else if (m_iDir == DIR_LEFT)
			{
				vecShild.x -= 10;
			}
			CEffectManager::GetInst()->ActiveEffect(L"나나야방어", vecShild);
			return;
		}
		else
			m_bGuard = false;
	}

	m_bGuard = false;

	if (m_eState == KYO_NOTYPE)
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
	}

	if (HIT_UP == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit1", 3, -1, 0);
		m_eState = KYO_HIT_UP;
	}
	else if (HIT_MIDDLE == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
		m_eState = KYO_HIT_MID;
	}
	else if (HIT_DOWN == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
		m_eState = KYO_HIT_DOWN;
	}
	else if (HIT_CATCH == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
	}
	else if (HIT_AIRUP == eOtherHitState)
	{
		GetAnimatorEX()->SetPause(false);
		m_fStunTime = 0.f;
		m_fStunHitTime = 0.f;
		GetAnimatorEX()->ChangeAnimation(L"AirHit", 2, -1, 0);
		m_eState = KYO_HIT_AERIAL;
	}
	else if (HIT_KNOCKDOWN == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Knockdown", 2, -1, 0);
		m_eState = KYO_HIT_KNOCKDOWN;
	}
}

void CPlayerScKyo::PushOtherPlayer(CGameObject * _pOther)
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	m_vecPos = GetTransform()->GetLocalPos();

	if (strClipName==L"FrontWalk")
	{
		Vec3 vecPos = _pOther->GetTransform()->GetLocalPos();
		if (m_vecPos.x < _pOther->GetTransform()->GetLocalPos().x)
		{
			vecPos.x = m_vecPos.x + 50;
		}
		else
		{
			vecPos.x = m_vecPos.x - 50;
		}
		_pOther->GetTransform()->SetLocalPos(vecPos);
	}

	else
	{
		if (m_vecPos.x < _pOther->GetTransform()->GetLocalPos().x)
		{
			m_vecPos.x = _pOther->GetTransform()->GetLocalPos().x - 50;
		}
		else
		{
			m_vecPos.x = _pOther->GetTransform()->GetLocalPos().x + 50;
		}
		GetTransform()->SetLocalPos(m_vecPos);
	}
}

void CPlayerScKyo::CommandInput()
{
	if (KEYDOWN_PLAYER(m_tKey.Z)|| KEYDOWN_PLAYER(m_tKey.X)|| KEYDOWN_PLAYER(m_tKey.A)|| KEYDOWN_PLAYER(m_tKey.S))
		m_fTime -= 0.15f;

	if (m_bOnGround)
	{
		if (INPUT_PLAYER(m_tKey.Left, STATE_DOWN))
		{
			if (m_iDir == DIR_LEFT)
				m_vecCmd.push_back(m_tKey.Right);
			else if (m_iDir == DIR_RIGHT)
				m_vecCmd.push_back(m_tKey.Left);

			m_fTime = 0.05f;
		}
		if (INPUT_PLAYER(m_tKey.Right, STATE_DOWN))
		{
			if (m_iDir == DIR_LEFT)
				m_vecCmd.push_back(m_tKey.Left);
			else if (m_iDir == DIR_RIGHT)
				m_vecCmd.push_back(m_tKey.Right);

			m_fTime = 0.05f;
		}
	}
	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
	{
		m_fTime = 0.05f;
	}
	if (INPUT_PLAYER(m_tKey.Down, STATE_DOWN))
	{
		m_vecCmd.push_back(m_tKey.Down);
		m_fTime = 0.05f;
	}

	if (m_fTime < 0.f)
		m_fTime = 0.f;
}

void CPlayerScKyo::CommandDecode()
{
	int iSize = m_vecCmd.size();

	if (CommandDecode_Size4() == RET_FAIL)
	{
		if (CommandDecode_Size3() == RET_FAIL)
		{
			if (CommandDecode_Size2() == RET_FAIL)
			{
				if (CommandDecode_Size1() == RET_FAIL)
				{
					CommandDecode_Size0();
				}
			}
		}
	}

	m_fTime = 0.f;
	m_vecCmd.clear();
}

int CPlayerScKyo::CommandDecode_Size4()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 4)
		return RET_FAIL;

	if (m_eState == KYO_IDLE || m_eState == KYO_CROUCH
		|| m_eState == KYO_FRONTWALK || m_eState == KYO_BACKWALK)
	{
		if (m_vecCmd[iSize - 4] == m_tKey.Down
			&&m_vecCmd[iSize - 3] == m_tKey.Left
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"리 108식 대사치", 9, -1, 0);
			m_pEffect->GetAnimatorEX()->ChangeAnimation(L"108식_1", 4, 4, 0);

			m_eState = KYO_UTYPE108_OROCHINAGI;
			m_bEffect = false;
			iRet = RET_SUCCESS;
		}
		else if (m_vecCmd[iSize - 4] == m_tKey.Down
			&&m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Right)
		{
			if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"182식", 25, -1, 0);
				m_eState = KYO_TYPE182;
				m_bEffect = false;
				iRet = RET_SUCCESS;
			}
			else if (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"최종결전오의 무식", 49, -1, 0);
				m_eState = KYO_NOTYPE;
				m_bEffect = false;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 4] == m_tKey.Left
			&&m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Left
			&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 7, -1, 0);
			m_eState = KYO_TYPE100_ONIYAKI;
			m_bEffect = false;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.Z))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.X))
				m_iCmdLinkCnt = 1;
		}
		else if (m_vecCmd[iSize - 4] == m_tKey.Right
			&&m_vecCmd[iSize - 3] == m_tKey.Left
			&&m_vecCmd[iSize - 2] == m_tKey.Right
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"REDKicK", 10, -1, 0);
			m_eState = KYO_REDKICK;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.A))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.S))
				m_iCmdLinkCnt = 1;
		}
		else
			int iRet = RET_FAIL;
	}

	else if (m_eState == KYO_TYPE401_TSUMIYOMI)
	{
		if (m_vecCmd[iSize - 4] == m_tKey.Left
			&&m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Left
			&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE100_ONIYAKI;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.Z))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.X))
				m_iCmdLinkCnt = 1;
		}
	}

	return iRet;
}

int CPlayerScKyo::CommandDecode_Size3()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 3)
		return RET_FAIL;

	if (m_eState == KYO_IDLE || m_eState == KYO_FRONTWALK || m_eState == KYO_BACKWALK)
	{
		if (m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 7, -1, 0);
			m_eState = KYO_TYPE100_ONIYAKI;
			m_bEffect = false;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.Z))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.X))
				m_iCmdLinkCnt = 1;
		}
		else if (m_vecCmd[iSize - 3] == m_tKey.Left
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"REDKicK", 10, -1, 0);
			m_eState = KYO_REDKICK;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.A))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.S))
				m_iCmdLinkCnt = 1;
		}
	}
	else if (m_eState == KYO_TYPE114_ARAGAMI)
	{
		if (m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Left)
		{
			if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			{
				m_ePreInput = KYO_TYPE127_YANOSABI;
				m_iCmdLinkCnt = 2;
				iRet = RET_SUCCESS;
			}
			else if (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			{
				m_ePreInput = KYO_TYPE427_HIKIGANE_S;
				iRet = RET_SUCCESS;
			}
		}
	}
	else if (m_eState == KYO_TYPE128_KONOKIZU && 2 == m_iCmdLinkCnt)
	{
		if (m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE427_HIKIGANE_L;
			iRet = RET_SUCCESS;
		}
	}
	else if (m_eState == KYO_TYPE115_DOKUGAMI)
	{
		if (m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE401_TSUMIYOMI;
			iRet = RET_SUCCESS;
		}
	}
	else if (m_eState == KYO_TYPE401_TSUMIYOMI)
	{
		if (m_vecCmd[iSize - 3] == m_tKey.Right
			&&m_vecCmd[iSize - 2] == m_tKey.Down
			&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE100_ONIYAKI;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.Z))
				m_iCmdLinkCnt = 0;
			else if (KEYDOWN(m_tKey.X))
				m_iCmdLinkCnt = 1;
		}
	}

	return iRet;
}

int CPlayerScKyo::CommandDecode_Size2()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 2)
		return RET_FAIL;

	if (m_eState == KYO_IDLE || m_eState == KYO_CROUCH
		|| m_eState == KYO_FRONTWALK || m_eState == KYO_BACKWALK)
	{
		if (m_vecCmd[iSize - 2] == m_tKey.Down&&m_vecCmd[iSize - 1] == m_tKey.Right)
		{
			if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"114식 황물기", 17, -1, 0);
				m_iCmdLinkCnt = 1;
				m_eState = KYO_TYPE114_ARAGAMI;
			}
			if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"115식 독물기", 11, -1, 0);
				m_eState = KYO_TYPE115_DOKUGAMI;
			}
			if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"75식 개", 10, -1, 0);
				m_eState = KYO_TYPE75_KAI;
			}
			if (KEYDOWN_PLAYER(m_tKey.A))
			{
				GetAnimatorEX()->ChangeAnimation(L"88식", 6, -1, 0);
				m_eState = KYO_TYPE88;
				m_vecPos.x += 50.f*m_iDir;
			}

			iRet = RET_SUCCESS;
		}

		if (m_eState == KYO_BACKWALK)
		{
			if (m_vecCmd[iSize - 2] == m_tKey.Down&&m_vecCmd[iSize - 1] == m_tKey.Left)
			{
				if (KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S))
				{
					GetAnimatorEX()->ChangeAnimation(L"REDKicK", 10, -1, 0);
					m_eState = KYO_REDKICK;
					iRet = RET_SUCCESS;
				}
			}
		}
	}

	else if (m_eState == KYO_TYPE114_ARAGAMI)
	{
		if (m_vecCmd[iSize - 2] == m_tKey.Down&&m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE128_KONOKIZU;
			iRet = RET_SUCCESS;
		}
		if (m_vecCmd[iSize - 2] == m_tKey.Right&&m_vecCmd[iSize - 1] == m_tKey.Left)
		{
			if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			{
				m_ePreInput = KYO_TYPE127_YANOSABI;
				m_iCmdLinkCnt = 2;
				iRet = RET_SUCCESS;
			}
			else if (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			{
				m_ePreInput = KYO_TYPE427_HIKIGANE_S;
				iRet = RET_SUCCESS;
			}
		}
	}

	else if (m_eState == KYO_TYPE128_KONOKIZU && 2 == m_iCmdLinkCnt)
	{
		if (m_vecCmd[iSize - 2] == m_tKey.Right
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE427_HIKIGANE_L;
			iRet = RET_SUCCESS;
		}
	}
	else if (m_eState == KYO_TYPE115_DOKUGAMI)
	{
		if (m_vecCmd[iSize - 2] == m_tKey.Right
			&&m_vecCmd[iSize - 1] == m_tKey.Left
			&& (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE401_TSUMIYOMI;
			iRet = RET_SUCCESS;
		}
	}
	return iRet;
}

int CPlayerScKyo::CommandDecode_Size1()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 1)
		return RET_FAIL;

	if (m_eState == KYO_IDLE || m_eState == KYO_FRONTWALK || m_eState == KYO_BACKWALK)
	{
		if (m_vecCmd[iSize - 1] == KEY_TYPE::KEY_RIGHT)
		{
			if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"외식 굉부 양", 9, -1, 0);
				m_eState = KYO_OUTTYPE_GOUFU;
				iRet = RET_SUCCESS;
			}
		}
	}
	if (m_eState == KYO_CROUCH)
	{
		if (m_vecCmd[iSize - 1] == m_tKey.Right)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z))
			{
				GetAnimatorEX()->ChangeAnimation(L"114식 황물기", 17, -1, 0);
				m_iCmdLinkCnt = 1;
				m_eState = KYO_TYPE114_ARAGAMI;
			}
			if (KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"115식 독물기", 11, -1, 0);
				m_eState = KYO_TYPE115_DOKUGAMI;
			}
			if (KEYDOWN_PLAYER(m_tKey.S))
			{
				GetAnimatorEX()->ChangeAnimation(L"75식 개", 10, -1, 0);
				m_eState = KYO_TYPE75_KAI;
			}
			if (KEYDOWN_PLAYER(m_tKey.A))
			{
				GetAnimatorEX()->ChangeAnimation(L"88식", 6, -1, 0);
				m_vecPos.x += 50.f*m_iDir;
				m_eState = KYO_TYPE88;
			}
			iRet = RET_SUCCESS;
		}

		if (m_vecCmd[iSize - 1] == m_tKey.Down)
		{
			if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"CrouchLP", 1, -1, 0);
				m_eState = KYO_CROUCH_LP;
			}
			if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"CrouchSP", 4, -1, 0);
				m_eState = KYO_CROUCH_SP;
			}
			if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"CrouchLK", 1, -1, 0);
				m_eState = KYO_CROUCH_LK;
			}
			if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			{
				GetAnimatorEX()->ChangeAnimation(L"CrouchSK", 8, -1, 0);
				m_eState = KYO_CROUCH_SK;
			}
		}
	}
	if (m_eState == KYO_JUMP_UP || m_eState == KYO_JUMP_DOWN
		|| m_eState == KYO_JUMP_FRONT || m_eState == KYO_JUMP_BACK)
	{
		if (m_vecCmd[iSize - 1] == m_tKey.Down
			&& (INPUT_PLAYER(m_tKey.X, STATE_DOWN) || INPUT_PLAYER(m_tKey.Z, STATE_DOWN)))
		{
			GetAnimatorEX()->ChangeAnimation(L"나락떨구기", 1, -1, 0);
			m_eState = KYO_OUTTYPE_NARAKU;
			iRet = RET_SUCCESS;

			if (KEYDOWN(m_tKey.X))
				m_iCmdLinkCnt = 1;
		}
	}
	if (m_eState == KYO_TYPE401_TSUMIYOMI)
	{
		if (m_vecCmd[iSize - 1] == m_tKey.Right
			&& (INPUT_PLAYER(m_tKey.X, STATE_DOWN) || INPUT_PLAYER(m_tKey.Z, STATE_DOWN)))
		{
			m_ePreInput = KYO_TYPE402_BATSUYOMI;
			iRet = RET_SUCCESS;
		}
	}

	return iRet;
}

int CPlayerScKyo::CommandDecode_Size0()
{
	if (m_eState == KYO_IDLE || m_eState == KYO_FRONTWALK || m_eState == KYO_BACKWALK)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundLP", 2, -1, 0);
			m_eState = KYO_LP;
		}
		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSP", 4, -1, 0);
			m_eState = KYO_SP;
		}
		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSK", 5, -1, 0);
			m_eState = KYO_SK;
		}
		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			if (m_eState == KYO_FRONTWALK)
			{
				GetAnimatorEX()->ChangeAnimation(L"외식 굉부 양", 9, -1, 0);
				m_eState = KYO_OUTTYPE_GOUFU;
			}
			else
			{
				GetAnimatorEX()->ChangeAnimation(L"GroundLK", 4, -1, 0);
				m_eState = KYO_LK;
			}
		}
	}

	else if (m_eState == KYO_CROUCH)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchLP", 1, -1, 0);
			m_eState = KYO_CROUCH_LP;
		}
		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchSP", 4, -1, 0);
			m_eState = KYO_CROUCH_SP;
		}
		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchLK", 1, -1, 0);
			m_eState = KYO_CROUCH_LK;
		}
		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchSK", 8, -1, 0);
			m_eState = KYO_CROUCH_SK;
		}
	}
	else if (m_eState == KYO_JUMP_UP || m_eState == KYO_JUMP_DOWN
		|| m_eState == KYO_JUMP_FRONT || m_eState == KYO_JUMP_BACK)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpLP", 3, -1, 0);
			m_eState = KYO_JUMP_LP;
		}
		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpSP", 1, -1, 0);
			m_eState = KYO_JUMP_SP;
		}
		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpLK", 2, -1, 0);
			m_eState = KYO_JUMP_LK;
		}
		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpSK", 3, -1, 0);
			m_eState = KYO_JUMP_SK;
		}
	}
	else if (m_eState == KYO_TYPE128_KONOKIZU&&m_iCmdLinkCnt == 2)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			m_ePreInput = KYO_TYPE127_YANOSABI;
			m_iCmdLinkCnt = 0;
		}
		else if (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			m_ePreInput = KYO_TYPE125_NANASE;
			m_iCmdLinkCnt = 0;
		}
	}
	else if (m_eState == KYO_TYPE127_YANOSABI&&m_iCmdLinkCnt == 2)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN))
			m_ePreInput = KYO_OUTTYPE_MIGIRI;
		else if (INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
			m_ePreInput = KYO_TYPE125_NANASE;
	}

	return 0;
}

void CPlayerScKyo::Gravity()
{
	if (m_bOnGround == false)
	{
		m_vecPos = GetTransform()->GetLocalPos();

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
				int iMaxFrame = GetAnimatorEX()->GetCurAnim()->GetMaxFrameCnt();
				int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
				m_fStunTime += DT;
				
					if (iCurFrame > 1 && iMaxFrame > 2)
					{
						GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
						GetAnimatorEX()->SetPause(true);
					}
					else
						GetAnimatorEX()->SetPause(false);
				
				m_fJumpSpeed = 0.f;
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
				m_fJumpSpeed = 0.f;
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

void CPlayerScKyo::SetState(UINT _iState)
{
	m_eState = (KYO_STATE)_iState;
	m_iBeforeIdx = -1;

	switch (m_eState)
	{
	case KYO_IDLE:
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		break;
	case KYO_FRONTWALK:
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 0);
		break;
	case KYO_BACKWALK:
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		break;
	case KYO_FRONTDASH:
		GetAnimatorEX()->ChangeAnimation(L"FrontDash");
		break;
	case KYO_BACKDASH:
		GetAnimatorEX()->ChangeAnimation(L"BackDash");
		break;
	case KYO_CROUCH:
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 0);
		break;
	case KYO_JUMP_UP:
		GetAnimatorEX()->ChangeAnimation(L"JumpUp");
		break;
	case KYO_JUMP_DOWN:
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		break;
	case KYO_JUMP_FRONT:
		GetAnimatorEX()->ChangeAnimation(L"JumpFront");
		m_bOnGround = false;
		break;
	case KYO_JUMP_BACK:
		GetAnimatorEX()->ChangeAnimation(L"JumpBack");
		m_bOnGround = false;
		break;
	case KYO_ROLL:
		break;
	case KYO_GUARD:
		break;
	case KYO_LP:
		GetAnimatorEX()->ChangeAnimation(L"GroundLP", 2, -1, 0);
		break;
	case KYO_SP:
		GetAnimatorEX()->ChangeAnimation(L"GroundSP", 4, -1, 0);
		break;
	case KYO_LK:
		GetAnimatorEX()->ChangeAnimation(L"GroundLK", 4, -1, 0);
		break;
	case KYO_SK:
		GetAnimatorEX()->ChangeAnimation(L"GroundSK", 5, -1, 0);
		break;
	case KYO_TYPE75_KAI:
		GetAnimatorEX()->ChangeAnimation(L"75식 개", 10, -1, 0);
		break;
	case KYO_REDKICK:
		GetAnimatorEX()->ChangeAnimation(L"REDKicK", 10, -1, 0);
		break;
	case KYO_TYPE100_ONIYAKI:
		GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 7, -1, 0);
		break;
	case KYO_UTYPE108_OROCHINAGI:
		GetAnimatorEX()->ChangeAnimation(L"리 108식 대사치", 9, -1, 0);
		break;
	case KYO_TYPE114_ARAGAMI:
		GetAnimatorEX()->ChangeAnimation(L"114식 황물기", 17, -1, 0);
		break;
	case KYO_TYPE115_DOKUGAMI:
		GetAnimatorEX()->ChangeAnimation(L"115식 독물기", 11, -1, 0);
		break;
	case KYO_TYPE125_NANASE:
		GetAnimatorEX()->ChangeAnimation(L"125식 칠뢰", 13, -1, 0);
		break;
	case KYO_TYPE127_YANOSABI:
		GetAnimatorEX()->ChangeAnimation(L"127식 팔청", 6, -1, 0);
		break;
	case KYO_TYPE128_KONOKIZU:
		GetAnimatorEX()->ChangeAnimation(L"128식 구상", 8, -1, 0);
		break;
	case KYO_TYPE182:
		GetAnimatorEX()->ChangeAnimation(L"182식", 25, -1, 0);
		break;
	case KYO_TYPE401_TSUMIYOMI:
		GetAnimatorEX()->ChangeAnimation(L"401식 죄읊기", 5, -1, 0);
		break;
	case KYO_TYPE402_BATSUYOMI:
		GetAnimatorEX()->ChangeAnimation(L"402식 벌읊기", 4, -1, 0);
		break;
	case KYO_TYPE427_HIKIGANE_L:
		GetAnimatorEX()->ChangeAnimation(L"427식 역철 약", 13, -1, 0);
		break;
	case KYO_TYPE427_HIKIGANE_S:
		GetAnimatorEX()->ChangeAnimation(L"427식 역철 강", 12, -1, 0);
		break;
	case KYO_OUTTYPE_GOUFU:
		GetAnimatorEX()->ChangeAnimation(L"외식 굉부 양", 9, -1, 0);
		break;
	case KYO_OUTTYPE_MIGIRI:
		GetAnimatorEX()->ChangeAnimation(L"외식 섬돌뚫기", 8, -1, 0);
		break;
	case KYO_NOTYPE:
		GetAnimatorEX()->ChangeAnimation(L"최종결전오의 무식", 49, -1, 0);
		break;
	case KYO_CROUCH_LP:
		GetAnimatorEX()->ChangeAnimation(L"CrouchLP", 1, -1, 0);
		break;
	case KYO_CROUCH_SP:
		GetAnimatorEX()->ChangeAnimation(L"CrouchSP", 4, -1, 0);
		break;
	case KYO_CROUCH_LK:
		GetAnimatorEX()->ChangeAnimation(L"CrouchLK", 1, -1, 0);
		break;
	case KYO_CROUCH_SK:
		GetAnimatorEX()->ChangeAnimation(L"CrouchSK", 8, -1, 0);
		break;
	case KYO_TYPE88:
		GetAnimatorEX()->ChangeAnimation(L"88식", 6, -1, 0);
		break;
	case KYO_JUMP_LP:
		GetAnimatorEX()->ChangeAnimation(L"JumpLP", 3, -1, 0);
		break;
	case KYO_JUMP_SP:
		GetAnimatorEX()->ChangeAnimation(L"JumpSP", 1, -1, 0);
		break;
	case KYO_JUMP_LK:
		GetAnimatorEX()->ChangeAnimation(L"JumpLK", 2, -1, 0);
		break;
	case KYO_JUMP_SK:
		GetAnimatorEX()->ChangeAnimation(L"JumpSK", 3, -1, 0);
		break;
	case KYO_OUTTYPE_NARAKU:
		GetAnimatorEX()->ChangeAnimation(L"나락떨구기", 1, -1, 0);
		break;
	case KYO_HIT_DOWN:
		break;
	case KYO_HIT_MID:
		break;
	case KYO_HIT_UP:
		break;
	case KYO_HIT_AERIAL:
		break;
	case KYO_HIT_KNOCKDOWN:
		GetAnimatorEX()->ChangeAnimation(L"Knockdown", 2, -1, 0);
		break;
	case KYO_HIT_STANDUP:
		break;
	case KYO_HIT_STANDUP_QUICK:
		GetAnimatorEX()->ChangeAnimation(L"StandUp2");
		break;
	case KYO_END:
		break;
	default:
		break;
	}


}

void CPlayerScKyo::SoundOutput(const wstring & _strSound, int _iBefIdx)
{
	/*if (m_iBeforeIdx == _iBefIdx)
	{
		if (!m_bSound)
		{
			m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
			m_pSound->Play(1);
			m_bSound = true;
		}
	}*/
}

void CPlayerScKyo::SoundOutput(const wstring & _strSound)
{
	/*if (!m_bSound)
	{
		m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
		m_pSound->Play(1);
		m_bSound = true;
	}*/
}

void CPlayerScKyo::SoundOutputAnimFin(const wstring & _strSound)
{
	/*if (GetAnimatorEX()->IsAnimFinish())
	{
		if (!m_bSound)
		{
			m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
			m_pSound->Play(1);
			m_bSound = true;
		}
	}*/
}

void CPlayerScKyo::Idle()
{
	m_bEffect = false;
	m_bGuard = false;
	m_fKnockback = 0.f;
	m_fStunTime = 0.f;
	m_fStunHitTime = 0.f;
	m_fStunAttTime = 0.f;
	m_iCmdLinkCnt = 0;
	m_bSound = true;
	m_fCharge = 0.f;

	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_RIGHT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_LEFT))
	{
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		m_eState = KYO_BACKWALK;
	}
	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_LEFT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_RIGHT))
	{
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 0);
		m_eState = KYO_FRONTWALK;
	}
	else if (INPUT_PLAYER(m_tKey.Down, STATE_PUSH))
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 0);
		m_eState = KYO_CROUCH;
	}
	else if (KEYDOWN_PLAYER(m_tKey.Up))
	{
		CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
		pSound->Play(1);
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;

		if (!KEYDOWN_PLAYER(m_tKey.Left) && !KEYDOWN_PLAYER(m_tKey.Right))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpUp");
			m_eState = KYO_JUMP_UP;
		}
	}

	if (KEYDOWN_PLAYER(m_tKey.Up)&&((KEYDOWN_PLAYER(m_tKey.Left) && m_iDir == DIR_LEFT)
		|| (KEYDOWN_PLAYER(m_tKey.Right) && m_iDir == DIR_RIGHT)))
	{
		//CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
		//pSound->Play(1);
		m_eState = KYO_JUMP_FRONT;
		GetAnimatorEX()->ChangeAnimation(L"JumpFront");
	}
	else if (KEYDOWN_PLAYER(m_tKey.Up) &&( (KEYDOWN_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)
		|| (KEYDOWN_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT)))
	{
		//CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
		//pSound->Play(1);
		m_eState = KYO_JUMP_BACK;
		GetAnimatorEX()->ChangeAnimation(L"JumpBack");
	}

	if (KEYDOWN_PLAYER(m_tKey.Left) || KEYDOWN_PLAYER(m_tKey.Right))
	{
		if (m_vecCmd.size() >= 2)
		{
			int iSize = m_vecCmd.size();

			if (m_fDashCooldown > 0.5f)
			{
				m_fDashCooldown = 0.f;

				if (m_vecCmd[iSize - 2] == m_tKey.Left&&m_vecCmd[iSize - 1] == m_tKey.Left)
				{
					GetAnimatorEX()->ChangeAnimation(L"BackDash");
					m_eState = KYO_BACKDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
				}
				else if (m_vecCmd[iSize - 2] == m_tKey.Right&&m_vecCmd[iSize - 1] == m_tKey.Right)
				{
					GetAnimatorEX()->ChangeAnimation(L"FrontDash");
					m_eState = KYO_FRONTDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
				}
			}
		}
	}
}

void CPlayerScKyo::FrontWalk()
{
	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_RIGHT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_LEFT))
	{
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		m_eState = KYO_BACKWALK;
	}

	if (KEYUP_PLAYER(m_tKey.Left) || KEYUP_PLAYER(m_tKey.Right))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		m_eState = KYO_IDLE;
	}

	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
	{
		//CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
		//pSound->Play(1);
		m_eState = KYO_JUMP_FRONT;
		GetAnimatorEX()->ChangeAnimation(L"JumpFront");
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;
	}
}

void CPlayerScKyo::BackWalk()
{
	m_bGuard = true;

	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_LEFT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_RIGHT))
	{
		m_bGuard = false;
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 4, -1, 0);
		m_eState = KYO_FRONTWALK;
	}

	if (KEYUP_PLAYER(m_tKey.Left) || KEYUP_PLAYER(m_tKey.Right))
	{
		m_bGuard = false;
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		m_eState = KYO_IDLE;
	}

	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
	{
		//CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
		//pSound->Play(1);
		m_bGuard = false;
		m_eState = KYO_JUMP_BACK;
		GetAnimatorEX()->ChangeAnimation(L"JumpBack");
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;
	}
}

void CPlayerScKyo::FrontDash()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::BackDash()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::Crouch()
{
	if (KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
		|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)
	{
		m_bGuard = true;
	}
	if (INPUT_PLAYER(m_tKey.Down, STATE_PUSH))
	{
		//GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 1);
	}

	if (true == INPUT_PLAYER(m_tKey.Down, STATE_UP))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;	
	}
}

void CPlayerScKyo::JumpUp()
{
	m_bOnGround = false;
	m_fJumpSpeed = 1300.f;

	if (INPUT_PLAYER(m_tKey.Left, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = KYO_JUMP_FRONT;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = KYO_JUMP_BACK;
		}
	}
	if (INPUT_PLAYER(m_tKey.Right, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = KYO_JUMP_BACK;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = KYO_JUMP_FRONT;
		}
	}

	if (m_iBeforeIdx == 4)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
	}

	if (m_bOnGround == true)
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::JumpDown()
{
	if (m_bOnGround == true)
	{
		m_fJumpSpeed = 0.f;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::JumpFront()
{
	m_fJumpSpeed = 1300.f;
	if (m_bOnGround == true)
	{
		if (3 <= GetAnimatorEX()->GetCurAnim()->GetCurIndex())
		{
			m_fJumpSpeed = 0.f;
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_eState = KYO_IDLE;
		}
	}
}

void CPlayerScKyo::JumpBack()
{
	m_fJumpSpeed = 1300.f;
	if (m_bOnGround == true)
	{
		if (3 <= GetAnimatorEX()->GetCurAnim()->GetCurIndex())
		{
			m_fJumpSpeed = 0.f;
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_eState = KYO_IDLE;
		}
	}
}

void CPlayerScKyo::Roll()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::Guard()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
		m_bGuard = false;
	}
}

void CPlayerScKyo::GroundLP()
{
	if (m_iBeforeIdx != 0)
		m_bSound = false;
	SoundOutput(L"Kyo_Att1.wav", 0);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::GroundSP()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	SoundOutput(L"Kyo_Att3.wav", 3);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::GroundLK()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	SoundOutput(L"Kyo_Att2.wav", 3);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::GroundSK()
{
	if (m_iBeforeIdx != 4)
		m_bSound = false;

	SoundOutput(L"Kyo_Att4.wav", 4);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::Type75_Kai()
{
	if (m_iBeforeIdx != 4&& m_iBeforeIdx != 8)
		m_bSound = false;

	SoundOutput(L"Kyo_Att6.wav", 4);
	SoundOutput(L"Kyo_Att5.wav", 8);

	if (m_iBeforeIdx < 6)
		m_eHitState = HIT_UP;
	else
		m_eHitState = HIT_AIRUP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::RedKicK()
{
	if (m_iCmdLinkCnt == 0)
		m_eHitState = HIT_UP;
	else
		m_eHitState = HIT_KNOCKDOWN;

	if (m_iBeforeIdx == 2)
	{
		m_bOnGround = false;
		m_fJumpSpeed = 1000.f+200.f*m_iCmdLinkCnt;
	}

	if (m_iBeforeIdx != 2)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Kyo_REDKicK1.wav", 2);
	else if (1 == iRand)
		SoundOutput(L"Kyo_REDKicK2.wav", 2);

	if (GetAnimatorEX()->IsAnimFinish()||(m_bOnGround&&m_iBeforeIdx>2))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::Type100_Oniyaki()
{
	m_eHitState = HIT_AIRUP;

	if (m_iBeforeIdx != 2)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Kyo_Oniyaki1.wav", 2);
	else if (1 == iRand)
		SoundOutput(L"Kyo_Oniyaki2.wav", 2);

	if (m_iBeforeIdx == 1)
	{
		m_pEffect->GetTransform()->SetLocalScale(Vec3(1.5f, 1.5f, 1.5f));
		m_bOnGround = false;
		m_fJumpSpeed = 600.f+m_iCmdLinkCnt*300.f;
	}

	if (GetAnimatorEX()->IsAnimFinish()||(m_bOnGround&&m_iBeforeIdx>2))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
		m_pEffect->GetTransform()->SetLocalScale(Vec3One);
	}

	if (!m_bEffect)
	{
		m_bEffect = true;
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"100식", 19, -1, 0);
	}
}

void CPlayerScKyo::TypeU108_Orochinagi()
{
	m_pEffect->GetTransform()->SetLocalPos(Vec3Zero);
	m_eHitState = HIT_AIRUP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_pEffect->GetTransform()->SetLocalPos(Vec3(0.f,-0.05f,0.f));

		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}

	if (!m_bEffect&&m_iBeforeIdx==0)
	{
		Vec3 vPos = GetTransform()->GetLocalPos();
		m_bEffect = true;
	}

	if (3 == m_iBeforeIdx||2==m_iBeforeIdx)
	{
		m_bEffect = true;

		if ((INPUT_PLAYER(m_tKey.Z, STATE_PUSH) || INPUT_PLAYER(m_tKey.X, STATE_PUSH))&&m_fCharge<=2.1f)
		{
			m_fCharge += DT;

			if (m_iBeforeIdx == 3)
				GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);

			if (m_pEffect->GetAnimatorEX()->GetCurAnim() != NULL)
			{
				if (m_pEffect->GetAnimatorEX()->GetCurAnim()->GetKey() == L"108식_1")
					m_pEffect->GetAnimatorEX()->ChangeAnimation(L"108식_2", 3, -1, 0);
			}
		}
		if ((INPUT_PLAYER(m_tKey.Z, STATE_UP) || INPUT_PLAYER(m_tKey.X, STATE_UP))||m_fCharge>2.1f)
		{
			m_pSound->Stop(true);
			GetAnimatorEX()->SetPause(false);
			m_pEffect->GetAnimatorEX()->ChangeAnimation(L"108식_3", 5, -1, 0);
		}
	}

	else if (4 == m_iBeforeIdx || 5 == m_iBeforeIdx)
	{
		m_pSound->Stop(true);
		m_bEffect = false;
	}

	else if (7 == m_iBeforeIdx)
	{
		if (!m_bEffect)
		{
			m_bEffect = true;
		}
	}

	if (m_iBeforeIdx < 7 && m_iBeforeIdx != 2 && m_iBeforeIdx != 3)
	{
		if(m_pSound!=NULL)
			m_pSound->Stop(true);
		m_bSound = false;
	}
	SoundOutput(L"Kyo_Orochinagi2.wav", 2);
	SoundOutput(L"Kyo_Orochinagi4.wav", 7);
}

void CPlayerScKyo::Type114_Aragami()
{
	m_eHitState = HIT_MIDDLE;

	if (m_iBeforeIdx == 7)
	{
		if (!m_bEffect)
		{
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_황독", Vec3(m_vecPos.x + 134.f*m_iDir, m_vecPos.y, m_vecPos.z));
		}
	}

	if(m_iBeforeIdx!=2&& m_iBeforeIdx != 14&&m_iBeforeIdx!=15)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	
	if (m_iBeforeIdx == 14)
	{
		if (KYO_TYPE128_KONOKIZU == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"128식 구상", 8, -1, 0);
			m_iCmdLinkCnt = 2;
			m_bEffect = false;
			m_eState = KYO_TYPE128_KONOKIZU;
		}
		else if (KYO_TYPE127_YANOSABI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"127식 팔청", 6, -1, 0);
			m_bEffect = false;
			m_eState = KYO_TYPE127_YANOSABI;
		}
		else if (KYO_TYPE427_HIKIGANE_S == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"427식 역철 강", 12, -1, 0);
			m_bEffect = false;
			m_eState = KYO_TYPE427_HIKIGANE_S;
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{	
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type115_Dokugami()
{
	m_eHitState = HIT_MIDDLE;

	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 7)
		{
			
		}
	}

	if (m_iBeforeIdx != 7)
		m_bSound = false;
	

	if (iCurFrame >= 10)
	{
		if (KYO_TYPE401_TSUMIYOMI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"401식 죄읊기", 5, -1, 0);
			m_bEffect = false;
			m_iCmdLinkCnt = 0;
			m_eState = KYO_TYPE401_TSUMIYOMI;
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		m_bEffect = false;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type125_Nanase()
{
	m_iCmdLinkCnt = 0;
	m_eHitState = HIT_UP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type127_Yanosabi()
{
	int iRand = getRandomNumber_int(0, 1);
	

	m_eHitState = HIT_MIDDLE;

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 3)
		{
			m_bEffect = true;
		}	
	}
	
	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_bSound = false;

		if (KYO_OUTTYPE_MIGIRI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"외식 섬돌뚫기", 8, -1, 0);
			m_bEffect = false;
			m_eState = KYO_OUTTYPE_MIGIRI;
		}
		else if (KYO_TYPE125_NANASE == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"125식 칠뢰", 13, -1, 0);
			m_eState = KYO_TYPE125_NANASE;
		}
		else
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_bEffect = false;
			m_eState = KYO_IDLE;
			m_ePreInput = KYO_IDLE;
		}
	}
}

void CPlayerScKyo::Type128_Konokizu()
{
	int iRand = getRandomNumber_int(0, 1);

	m_eHitState = HIT_UP;

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 3)
		{
			m_bEffect = true;
		}
	}
	
	if (m_iBeforeIdx >= 6)
	{
		if (KYO_TYPE427_HIKIGANE_L == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"427식 역철 약", 13, -1, 0);
			m_bEffect = false;
			m_iCmdLinkCnt = 0;
			m_eState = KYO_TYPE427_HIKIGANE_L;
		}
		else if (KYO_TYPE127_YANOSABI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"127식 팔청", 6, -1, 0);
			m_bEffect = false;
			m_eState = KYO_TYPE127_YANOSABI;
		}
		else if (KYO_TYPE125_NANASE == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"125식 칠뢰", 13, -1, 0);
			m_eState = KYO_TYPE125_NANASE;
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type182()
{
	if (m_iBeforeIdx !=1 && m_iBeforeIdx != 15)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);

	SoundOutput(L"Kyo_Notype2.wav", 1);

	if(iRand==0)
	SoundOutput(L"Kyo_Orochinagi5.wav", 15);
	else
	SoundOutput(L"Kyo_Type182_3.wav", 15);

	m_eHitState = HIT_MIDDLE;
	
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}

	if (1 == m_iBeforeIdx)
		m_bEffect = false;

	if (!m_bEffect)
	{
		float fDir = GetTransform()->GetLocalScale().x;

		if (0 == m_iBeforeIdx)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00049.wav");
			pSound->Play(1);
			Vec3 vPos = GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"SuperSpark1", vPos);
			m_bEffect = true;
		}

		else if (6 == m_iBeforeIdx)
		{
			if(m_pEffect->GetAnimatorEX()->GetCurAnim()==NULL)
				m_pEffect->GetAnimatorEX()->ChangeAnimation(L"182식_1", 5, -1, 0);
		
			m_bSound = false;

			if ((!KEYPUSH_PLAYER(m_tKey.Z)&& !KEYPUSH_PLAYER(m_tKey.X))||m_fCharge>2.1f)
			{
				GetAnimatorEX()->GetCurAnim()->SetFrameIndex(11);
				m_bEffect = false;
			}
		}

		if(11==m_iBeforeIdx)
			m_pSound->Stop(true);

		if (15 == m_iBeforeIdx)
		{
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_182식_2", m_vecPos);
			CEffectManager::GetInst()->FollowPos(L"Kyo_182식_2", m_vecPos, m_iDir - 1);
		}
		else if (19 == m_iBeforeIdx)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00013.wav");
			pSound->Play(1);
			//CEffectManager::GetInst()->ActiveEffect(L"Explosion_04", Vec3(m_vecPos.x - fDir*0.5f, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"Explosion_04", Vec3(m_vecPos.x - fDir*0.5f, m_vecPos.y, m_vecPos.z), m_iDir - 1);
		
			m_bEffect = true;
		}
	}

	
	if ((INPUT_PLAYER(m_tKey.Z, STATE_PUSH) || INPUT_PLAYER(m_tKey.X, STATE_PUSH))&&m_fCharge<=2.1f)
	{
		m_fCharge += DT;

		if (m_iBeforeIdx >= 10)
		{
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(5);
		}
	}
}

void CPlayerScKyo::Type401_Tsumiyomi()
{
	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 3)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
			pSound->Play(1);
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z), m_iDir - 1);
		}
	}
	
	m_eHitState = HIT_MIDDLE;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		if (KYO_TYPE402_BATSUYOMI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"402식 벌읊기", 4, -1, 0);
			m_eState = KYO_TYPE402_BATSUYOMI;
		}
		else if (KYO_TYPE100_ONIYAKI == m_ePreInput)
		{
			GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 7, -1, 0);
			m_eState = KYO_TYPE100_ONIYAKI;
			m_bEffect = false;
		}
		else
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_bEffect = false;
			m_eState = KYO_IDLE;
			m_ePreInput = KYO_IDLE;
		}
	}
}

void CPlayerScKyo::Type402_Batsuyomi()
{
	m_eHitState = HIT_MIDDLE;

	if (m_iBeforeIdx != 1)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Kyo_Batsuyomi1.wav",1);
	else if (1 == iRand)
		SoundOutput(L"Kyo_Batsuyomi2.wav",1);

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 1)
		{
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z), m_iDir - 1);
		}
	}
	
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_fJumpSpeed = 0.f;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type427_Hikigane_L()
{
	m_eHitState = HIT_MIDDLE;
	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 7)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
			pSound->Play(1);
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"Kyo_황독", Vec3(m_vecPos.x + 100.f*m_iDir, m_vecPos.y, m_vecPos.z), m_iDir - 1);
		}
	}
	
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::Type427_Hikigane_S()
{
	m_eHitState = HIT_AIRUP;

	if (m_iBeforeIdx != 0)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Kyo_Migiri1.wav", 0);
	else if (1 == iRand)
		SoundOutput(L"Kyo_Migiri2.wav", 0);

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 5)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00008.wav");
			pSound->Play(1);
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_섬돌", Vec3(m_vecPos.x + 50.f*m_iDir, m_vecPos.y, m_vecPos.z));
			CEffectManager::GetInst()->FollowPos(L"Kyo_섬돌", Vec3(m_vecPos.x + 50.f*m_iDir, m_vecPos.y, m_vecPos.z), m_iDir + 1);
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_eState = KYO_IDLE;
		m_ePreInput = KYO_IDLE;
	}
}

void CPlayerScKyo::OutType_Goufu()
{
	m_eHitState = HIT_UP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::OutType_Migiri()
{
	m_iCmdLinkCnt = 0;
	m_eHitState = HIT_KNOCKDOWN;

	if (!m_bEffect)
	{
		if (m_iBeforeIdx == 2)
		{
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_섬돌", m_vecPos+Vec3(100.f*m_iDir, -100.f, 0.f) , Vec3(400.f*m_iDir, 400.f, 1.f));
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_bEffect = false;
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::NoType()
{
	if (49 == m_iBeforeIdx)
		m_eHitState = HIT_AIRUP;
	else
		m_eHitState = HIT_MIDDLE;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
	else
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_AllBlack");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
	}

	if (!m_bEffect)
	{
		if (1==m_iBeforeIdx)
		{
			Vec3 vPos = GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"SuperSpark2", vPos);
			m_bEffect = true;
		}

		if (49 == m_iBeforeIdx)
		{
			m_bEffect = true;
			CEffectManager::GetInst()->ActiveEffect(L"Kyo_무식", m_vecPos);
		}
	}

	if (m_iBeforeIdx != 1&&m_iBeforeIdx!=3 && m_iBeforeIdx != 49)
	{
		m_bSound = false;
		m_bEffect = false;
	}

	SoundOutput(L"Kyo_Notype4.wav", 1);
	SoundOutput(L"common_00049.wav", 3);
	SoundOutput(L"Kyo_Notype3.wav", 49);
}
	
void CPlayerScKyo::CrouchLP()
{
	if (m_iBeforeIdx != 0)
		m_bSound = false;
	SoundOutput(L"Kyo_Att1.wav", 0);

	m_bGuard = false;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch",1,-1,1);
		m_eState = KYO_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScKyo::CrouchSP()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Att3.wav", 2);

	m_bGuard = false;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, -1, 1);
		m_eState = KYO_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScKyo::CrouchLK()
{
	if (m_iBeforeIdx != 0)
		m_bSound = false;
	SoundOutput(L"Kyo_Att2.wav", 0);

	m_bGuard = false;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, -1, 1);
		m_eState = KYO_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScKyo::CrouchSK()
{
	if (m_iBeforeIdx != 4)
		m_bSound = false;
	SoundOutput(L"Kyo_Att4.wav", 4);

	m_bGuard = false;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, -1, 1);
		m_eState = KYO_CROUCH;
	}
	m_eHitState = HIT_AIRUP;
}

void CPlayerScKyo::Type88()
{
	if (m_iBeforeIdx != 1&&m_iBeforeIdx!=5)
		m_bSound = false;
	SoundOutput(L"Kyo_Att8.wav", 1);
	SoundOutput(L"Kyo_Att7.wav", 5);

	m_bGuard = false;
	m_eHitState = HIT_DOWN;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = KYO_IDLE;
	}
}

void CPlayerScKyo::JumpLP()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Att1.wav", 2);

	m_bGuard = false;

	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;
	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScKyo::JumpSP()
{
	if (m_iBeforeIdx != 0)
		m_bSound = false;
	SoundOutput(L"Kyo_Att3.wav", 0);

	m_bGuard = false;
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;
	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScKyo::JumpLK()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Kyo_Att2.wav", 1);

	m_bGuard = false;
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;
	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScKyo::JumpSK()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Att4.wav", 2);

	m_bGuard = false;
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;
	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish()||m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScKyo::OutType_Naraku()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Kyo_Batsuyomi1.wav", 1);
	else if (1 == iRand)
		SoundOutput(L"Kyo_Batsuyomi2.wav", 1);
	m_eHitState = HIT_UP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = KYO_JUMP_DOWN;
		if (m_bOnGround&&m_iCmdLinkCnt==1)
		{
			GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 0);
			m_eState = KYO_CROUCH;
		}
	}
}

void CPlayerScKyo::HitDown()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Hit1.wav", 2);
	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = KYO_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScKyo::HitMid()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Hit2.wav", 2);
	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = KYO_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScKyo::HitUp()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Kyo_Hit3.wav", 2);
	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = KYO_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScKyo::HitAerial()
{
	if (m_bOnGround)
	{
		if (m_iBeforeIdx == 0)
			m_bOnGround = false;
		else
			SoundOutput(L"common_00046.wav");
	}
	else
		m_bSound = false;

	if (GetAnimatorEX()->IsAnimFinish() && m_bOnGround)
	{
		m_eState = KYO_HIT_KNOCKDOWN;
		GetAnimatorEX()->ChangeAnimation(L"Knockdown2");
		CEffectManager::GetInst()->ActiveEffect(L"Circle2", m_vecPos + Vec3(0.f, -110.f, 0.f));
	}
}

void CPlayerScKyo::HitKnockdown()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Kyo_Hit5.wav", 1);

	if(m_bOnGround)
		m_fKnockback = 0.f;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		if (!m_bDefeated)
		{
			m_eState = KYO_HIT_STANDUP_QUICK;
			GetAnimatorEX()->ChangeAnimation(L"StandUp2");
		}
	}
	if (m_iBeforeIdx == 2)
	{
		if (m_bDefeated)
		{
			GetAnimatorEX()->SetPause(true);
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
		}
	}
}

void CPlayerScKyo::StandUp()
{
}

void CPlayerScKyo::StandUpQuick()
{
	m_fKnockback = 0.f;

	if (KEYPUSH_PLAYER(m_tKey.Down) || KEYPUSH_PLAYER(m_tKey.Up) 
		|| KEYPUSH_PLAYER(m_tKey.Right) || KEYPUSH_PLAYER(m_tKey.Left))
		GetAnimatorEX()->SetPause(false);
	
	if (KEYDOWN_PLAYER(m_tKey.Left) || KEYDOWN_PLAYER(m_tKey.Right))
	{
		if (m_vecCmd.size() >= 2)
		{
			int iSize = m_vecCmd.size();

			if (m_fDashCooldown > 0.5f)
			{
				m_fDashCooldown = 0.f;

				if (m_vecCmd[iSize - 2] == m_tKey.Left&&m_vecCmd[iSize - 1] == m_tKey.Left)
				{
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
					GetAnimatorEX()->ChangeAnimation(L"BackDash");
					m_eState = KYO_BACKDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
				else if (m_vecCmd[iSize - 2] == m_tKey.Right&&m_vecCmd[iSize - 1] == m_tKey.Right)
				{
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
					GetAnimatorEX()->ChangeAnimation(L"FrontDash");
					m_eState = KYO_FRONTDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
			}
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = KYO_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}
