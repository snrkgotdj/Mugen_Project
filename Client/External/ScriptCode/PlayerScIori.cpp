#include "stdafx.h"
#include "PlayerScIori.h"

#include "Collider.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "GameObject.h"

#include "ShaderManager.h"
#include "ResManager.h"
#include "SceneManager.h"

#include "EffectManager.h"
#include "PlayerManager.h"
#include "SoundLoadMgr.h"
#include "GameManager.h"
#include "UIManager.h"
#include "BulletMgr.h"

#include "Scene.h"
#include "Layer.h"

#include "Material.h"

#include "CameraScript.h"
#include "ScriptCode\ShadowScript.h"
#include "..\Doom\define_server.h"
#include "NetworkManager.h"
#include "ClientManager.h"

CPlayerScIori::CPlayerScIori()
	: m_fTime(0.f)
	, m_iCmdLinkCnt(0)
	, m_bEffect(false)
	, m_bSound(false)
	, m_iSkillNum(0)
	, m_bPracticeMode(false)
	, m_eState(IORI_IDLE)
	, m_ePrevState(IORI_IDLE)
	, m_pSound(NULL)
	, m_fDashCooldown(0.f)
{
	m_fJumpSpeed = 0.f;
	m_fGravAccel = 4000.f;
	m_iDir = DIR_RIGHT;
	m_vecPos = Vec3(0.f,-1.f,0.f);
	m_fFloorY = 0.f;
	m_bDefeated = false;
}

CPlayerScIori::~CPlayerScIori()
{
}

void CPlayerScIori::Awake()
{
	GetTransform()->SetLocalScale(Vec3(300.f, 300.f, 1.f));

	CEffectManager::GetInst()->LoadEffect(L"Iori");

	m_pEffect = GetGameObject()->GetChildByName(L"IoriEffect");
	m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);

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

void CPlayerScIori::Start()
{
	GetAnimatorEX()->ChangeAnimation(L"Idle");

	m_vecPos = GetTransform()->GetLocalPos();

	CreateShadow();

	CSoundLoadMgr::GetInst()->LoadSoundFolder(L"Iori");

	((CShadowScript*)(m_pShadow->GetFirstScript()))->init(GetGameObject(), 0.03f, 0.1f);
}

void CPlayerScIori::Update()
{
	if (m_bOnline == false)
		Update_Client();

	else if (m_bOnline == true)
		Update_Server();
}

void CPlayerScIori::LateUpdate()
{
	m_vecPos= GetTransform()->GetLocalPos();

	if (GetAnimatorEX() == NULL)
		return;

	if (m_vecPos.y <= m_fFloorY)
	{
		m_vecPos.y = m_fFloorY;
		m_fGravity = 0;
		//m_fJumpSpeed = 0.f;
		m_bOnGround = true;
	}

	else
		m_bOnGround = false;

	GetTransform()->SetLocalPos(m_vecPos);
}

void CPlayerScIori::FinalUpdate()
{
	m_vecPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = vScale.x;
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	m_vecPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->fSpeed *m_iDir* DT;
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

	if(!m_bDefeated)
		GetTransform()->SetLocalPos(m_vecPos);

	CCameraScript* CamScript = (CCameraScript*)CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetFirstScript();

	if (CamScript == NULL)
		return;

	m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

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

void CPlayerScIori::PushOtherPlayer(CGameObject * _pOther)
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	m_vecPos = GetTransform()->GetLocalPos();

	if (strClipName == L"FrontWalk")
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

void CPlayerScIori::Update_Client()
{
	m_vecPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = GetTransform()->GetLocalScale().x;

	if (m_b1Player)
		CEffectManager::GetInst()->FollowPos(L"1P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);
	else
		CEffectManager::GetInst()->FollowPos(L"2P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);

	if (GetAnimatorEX() == NULL)
		return;

	if (m_iDir == DIR_LEFT)
		fDir = 300.f;
	else if (m_iDir == DIR_RIGHT)
		fDir = -300.f;

	m_fDashCooldown += DT;

	CommandInput();//커맨드 입력;

	if (!m_vecCmd.empty())
	{
		m_fTime += DT;
		float fLimit = 0.5f;

		if ((m_eState == IORI_IDLE || m_eState == IORI_CROUCH ||
			m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK) && m_fTime < 0.25f)
		{
			fLimit = 0.25f;
		}
		if (m_fTime > fLimit)
		{
			m_iCmdLinkCnt = 0;
			m_vecCmd.clear();
			m_fTime = 0.f;
		}
	}

	if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN) ||
		INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		CommandDecode();//커맨드 판독

	if (!m_bDefeated)
	{
		switch (m_eState)
		{
		case IORI_IDLE:							Idle();								break;
		case IORI_FRONTWALK:					FrontWalk();						break;
		case IORI_BACKWALK:						BackWalk();							break;
		case IORI_FRONTDASH:					FrontDash();						break;
		case IORI_BACKDASH:						BackDash();							break;
		case IORI_CROUCH:						Crouch();							break;
		case IORI_JUMP_UP:						JumpUp();							break;
		case IORI_JUMP_DOWN:					JumpDown();							break;
		case IORI_JUMP_FRONT:					JumpFront();						break;
		case IORI_JUMP_BACK:					JumpBack();							break;
		case IORI_ROLL:							Roll();								break;
		case IORI_LP:							GroundLP();							break;
		case IORI_SP:							GroundSP();							break;
		case IORI_LK:							GroundLK();							break;
		case IORI_SK:							GroundSK();							break;
		case IORI_OUTTYPE_YUMEBIKI_A:			OutType_Yumebiki_A();				break;
		case IORI_OUTTYPE_YUMEBIKI_B:			OutType_Yumebiki_B();				break;
		case IORI_OUTTYPE_GOUFU:				OutType_Goufu();					break;
		case IORI_TYPE100_ONIYAKI:				Type100_Oniyaki();					break;
		case IORI_TYPE108_YAMIBARAI:			Type108_Yamibarai();				break;
		case IORI_TYPE127_AOIHANA_A:			Type127_Aoihana_A();				break;
		case IORI_TYPE127_AOIHANA_B:			Type127_Aoihana_B();				break;
		case IORI_TYPE127_AOIHANA_C:			Type127_Aoihana_C();				break;
		case IORI_TYPE212_KOTOTSUKI_START:		Type212_Kototsuki();				break;
		case IORI_TYPE212_KOTOTSUKI_FAIL:		Type212_Kototsuki_Fail();			break;
		case IORI_TYPE212_KOTOTSUKI_SUCCESS:	Type212_Kototsuki_Success();		break;
		case IORI_KUZUKAZE_START:				Kuzukaze();							break;
		case IORI_KUZUKAZE_FAIL:				KuzukazeFail();						break;
		case IORI_KUZUKAZE_SUCCESS:				KuzukazeSuccess();					break;
		case IORI_KTYPE1211_YAOTOME_START:		KType1211_Yaotome_Start();			break;
		case IORI_KTYPE1211_YAOTOME:			KType1211_Yaotome();				break;
		case IORI_UTYPE316_SAIKA:				UType316_Saika();					break;
		case IORI_UTYPE1029_HOMURABOTOGI_START:				UType1029_Homurabotogi_Start();					break;
		case IORI_UTYPE1029_HOMURABOTOGI:				UType1029_Homurabotogi();					break;
		case IORI_CROUCH_LP:					CrouchLP();							break;
		case IORI_CROUCH_SP:					CrouchSP();							break;
		case IORI_CROUCH_LK:					CrouchLK();							break;
		case IORI_CROUCH_SK:					CrouchSK();							break;
		case IORI_JUMP_LP:						JumpLP();							break;
		case IORI_JUMP_SP:						JumpSP();							break;
		case IORI_JUMP_LK:						JumpLK();							break;
		case IORI_JUMP_SK:						JumpSK();							break;
		case IORI_OUTTYPE_YURIORI:				OutType_Yuriori();					break;
		case IORI_GUARD:						Guard();							break;
		case IORI_HIT_DOWN:						HitDown(); break;
		case IORI_HIT_MID:							HitMid(); break;
		case IORI_HIT_UP:								HitUp(); break;
		case IORI_HIT_AERIAL:						HitAerial(); break;
		case IORI_HIT_KNOCKDOWN:				HitKnockdown(); break;
		case IORI_HIT_STANDUP:					StandUp(); break;
		case IORI_HIT_STANDUP_QUICK:		StandUpQuick(); break;
		}
	}
	else
	{
		m_bOnGround = true;
	}

	if (m_eState == KYO_BACKWALK || (m_eState == KYO_CROUCH &&
		(KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
			|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)))
	{
		m_bGuard = true;
	}
	else
		m_bGuard = false;

	if (m_eState == IORI_CROUCH || m_eState == IORI_CROUCH_LP || m_eState == IORI_CROUCH_SP ||
		m_eState == IORI_CROUCH_LK || m_eState == IORI_CROUCH_SK)
		m_bCrouch = false;
	else
		m_bCrouch = true;

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));
	GetTransform()->SetLocalPos(m_vecPos);

	Gravity();
	EffectedTime();
	if (true == CKeyManager::Manager().isServer())
	{
		SendPos();
		m_vecPrevPos = m_vecPos;
	}

	//TODO : DebugMgr 사용
#if defined DEBUG | _DEBUG
	//char str[255];
	//GET_SINGLE(CDebugMgr)->SetResetTime(0.1f);
	//sprintf_s(str, "Cmd : (%d, %f) \n", m_vecCmd.size(), m_fTime);
	//GET_SINGLE(CDebugMgr)->OutputConsole(str);
#endif
}

void CPlayerScIori::Update_Server()
{
	if (m_b1Player)
		CEffectManager::GetInst()->FollowPos(L"1P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);
	else
		CEffectManager::GetInst()->FollowPos(L"2P_UI", m_vecPos + Vec3(0.f, 50.f, 0.f), m_iDir);

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

		if ((m_eState == IORI_IDLE || m_eState == IORI_CROUCH ||
			m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK) && m_fTime < 0.25f)
		{
			fLimit = 0.25f;
		}
		if (m_fTime > fLimit)
		{
			m_iCmdLinkCnt = 0;
			m_vecCmd.clear();
			m_fTime = 0.f;
		}
	}

	if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN) || INPUT_PLAYER(m_tKey.X, STATE_DOWN) ||
		INPUT_PLAYER(m_tKey.A, STATE_DOWN) || INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		CommandDecode();//커맨드 판독

	if (!m_bDefeated)
	{
		switch (m_eState)
		{
		case IORI_IDLE:							Idle();								break;
		case IORI_FRONTWALK:					FrontWalk();						break;
		case IORI_BACKWALK:						BackWalk();							break;
		case IORI_FRONTDASH:					FrontDash();						break;
		case IORI_BACKDASH:						BackDash();							break;
		case IORI_CROUCH:						Crouch();							break;
		case IORI_JUMP_UP:						JumpUp();							break;
		case IORI_JUMP_DOWN:					JumpDown();							break;
		case IORI_JUMP_FRONT:					JumpFront();						break;
		case IORI_JUMP_BACK:					JumpBack();							break;
		case IORI_ROLL:							Roll();								break;
		case IORI_LP:							GroundLP();							break;
		case IORI_SP:							GroundSP();							break;
		case IORI_LK:							GroundLK();							break;
		case IORI_SK:							GroundSK();							break;
		case IORI_OUTTYPE_YUMEBIKI_A:			OutType_Yumebiki_A();				break;
		case IORI_OUTTYPE_YUMEBIKI_B:			OutType_Yumebiki_B();				break;
		case IORI_OUTTYPE_GOUFU:				OutType_Goufu();					break;
		case IORI_TYPE100_ONIYAKI:				Type100_Oniyaki();					break;
		case IORI_TYPE108_YAMIBARAI:			Type108_Yamibarai();				break;
		case IORI_TYPE127_AOIHANA_A:			Type127_Aoihana_A();				break;
		case IORI_TYPE127_AOIHANA_B:			Type127_Aoihana_B();				break;
		case IORI_TYPE127_AOIHANA_C:			Type127_Aoihana_C();				break;
		case IORI_TYPE212_KOTOTSUKI_START:		Type212_Kototsuki();				break;
		case IORI_TYPE212_KOTOTSUKI_FAIL:		Type212_Kototsuki_Fail();			break;
		case IORI_TYPE212_KOTOTSUKI_SUCCESS:	Type212_Kototsuki_Success();		break;
		case IORI_KUZUKAZE_START:				Kuzukaze();							break;
		case IORI_KUZUKAZE_FAIL:				KuzukazeFail();						break;
		case IORI_KUZUKAZE_SUCCESS:				KuzukazeSuccess();					break;
		case IORI_KTYPE1211_YAOTOME_START:		KType1211_Yaotome_Start();			break;
		case IORI_KTYPE1211_YAOTOME:			KType1211_Yaotome();				break;
		case IORI_UTYPE316_SAIKA:				UType316_Saika();					break;
		case IORI_UTYPE1029_HOMURABOTOGI_START:				UType1029_Homurabotogi_Start();					break;
		case IORI_UTYPE1029_HOMURABOTOGI:				UType1029_Homurabotogi();					break;
		case IORI_CROUCH_LP:					CrouchLP();							break;
		case IORI_CROUCH_SP:					CrouchSP();							break;
		case IORI_CROUCH_LK:					CrouchLK();							break;
		case IORI_CROUCH_SK:					CrouchSK();							break;
		case IORI_JUMP_LP:						JumpLP();							break;
		case IORI_JUMP_SP:						JumpSP();							break;
		case IORI_JUMP_LK:						JumpLK();							break;
		case IORI_JUMP_SK:						JumpSK();							break;
		case IORI_OUTTYPE_YURIORI:				OutType_Yuriori();					break;
		case IORI_GUARD:						Guard();							break;
		case IORI_HIT_DOWN:						HitDown(); break;
		case IORI_HIT_MID:							HitMid(); break;
		case IORI_HIT_UP:								HitUp(); break;
		case IORI_HIT_AERIAL:						HitAerial(); break;
		case IORI_HIT_KNOCKDOWN:				HitKnockdown(); break;
		case IORI_HIT_STANDUP:					StandUp(); break;
		case IORI_HIT_STANDUP_QUICK:		StandUpQuick(); break;
		}
	}
	else
	{
		m_bOnGround = true;
	}

	if (m_eState == KYO_BACKWALK || (m_eState == KYO_CROUCH &&
		(KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
			|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)))
	{
		m_bGuard = true;
	}
	else
		m_bGuard = false;

	if (m_eState == IORI_CROUCH || m_eState == IORI_CROUCH_LP || m_eState == IORI_CROUCH_SP ||
		m_eState == IORI_CROUCH_LK || m_eState == IORI_CROUCH_SK)
		m_bCrouch = false;
	else
		m_bCrouch = true;

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));

	Gravity();
	EffectedTime();
	SendState();

	m_ePrevState = m_eState;
}

void CPlayerScIori::SendState()
{
	if (m_ePrevState == m_eState)
		return;

	PACKET_STATE tPacket;
	tPacket.bPlayer = m_b1Player;
	tPacket.eState = (UINT)m_eState;
	ServerEngine::CClientManager::Manager().Write(PH_STATE, (char*)&tPacket, sizeof(tPacket));
}

void CPlayerScIori::SetState(UINT _iState)
{
	m_eState = (IORI_STATE)_iState;
	m_iBeforeIdx = -1;

	switch (m_eState)
	{
	case IORI_IDLE:
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
	}
		break;
	case IORI_FRONTWALK:
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 0);
		break;
	case IORI_BACKWALK:
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		break;
	case IORI_FRONTDASH:
		GetAnimatorEX()->ChangeAnimation(L"FrontDash");
		break;
	case IORI_BACKDASH:
		GetAnimatorEX()->ChangeAnimation(L"BackDash");
		break;
	case IORI_CROUCH:
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 1);
		break;
	case IORI_JUMP_UP:
		GetAnimatorEX()->ChangeAnimation(L"JumpUp",1,-1,0);
		m_fJumpSpeed = 1300.f;
		m_bOnGround = false;
		break;
	case IORI_JUMP_DOWN:
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		break;
	case IORI_JUMP_FRONT:
		GetAnimatorEX()->ChangeAnimation(L"JumpFront");
		m_fJumpSpeed = 1300.f;
		m_bOnGround = false;
		break;
	case IORI_JUMP_BACK:
		GetAnimatorEX()->ChangeAnimation(L"JumpBack");
		m_fJumpSpeed = 1300.f;
		m_bOnGround = false;
		break;
	case IORI_GUARD:
		GetAnimatorEX()->ChangeAnimation(L"Guard", 0, -1, 0);
		break;
	case IORI_LP:
		GetAnimatorEX()->ChangeAnimation(L"GroundLP", 5, -1, 0);
		break;
	case IORI_SP:
		GetAnimatorEX()->ChangeAnimation(L"GroundSP", 8, -1, 0);
		break;
	case IORI_LK:
		GetAnimatorEX()->ChangeAnimation(L"GroundLK", 3, -1, 0);
		break;
	case IORI_SK:
		GetAnimatorEX()->ChangeAnimation(L"GroundSK", 8, -1, 0);
		break;
	case IORI_OUTTYPE_YUMEBIKI_A:
		GetAnimatorEX()->ChangeAnimation(L"외식 몽탄 1", 5, -1, 0);
		break;
	case IORI_OUTTYPE_YUMEBIKI_B:
		GetAnimatorEX()->ChangeAnimation(L"외식 몽탄 2", 5, -1, 0);
		break;
	case IORI_OUTTYPE_GOUFU:
		GetAnimatorEX()->ChangeAnimation(L"외식 굉부 음", 12, -1, 0);
		break;
	case IORI_TYPE100_ONIYAKI:
		GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 12, -1, 0);
		break;
	case IORI_TYPE108_YAMIBARAI:
		GetAnimatorEX()->ChangeAnimation(L"108식 어둠쫒기", 7, -1, 0);
		break;
	case IORI_TYPE127_AOIHANA_A:
		GetAnimatorEX()->ChangeAnimation(L"127식 규화 1", 4, -1, 0);
		break;
	case IORI_TYPE127_AOIHANA_B:
		GetAnimatorEX()->ChangeAnimation(L"127식 규화 2", 4, -1, 0);
		break;
	case IORI_TYPE127_AOIHANA_C:
		GetAnimatorEX()->ChangeAnimation(L"127식 규화 3", 4, -1, 0);
		break;
	case IORI_TYPE212_KOTOTSUKI_START:
		GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_start", 7, -1, 0);
		break;
	case IORI_TYPE212_KOTOTSUKI_FAIL:
		GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_fail");
		break;
	case IORI_TYPE212_KOTOTSUKI_SUCCESS:
		GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_success", 3, -1, 0);
		break;
	case IORI_KUZUKAZE_START:
		GetAnimatorEX()->ChangeAnimation(L"설풍_start", 3, -1, 0);
		break;
	case IORI_KUZUKAZE_FAIL:
		GetAnimatorEX()->ChangeAnimation(L"설풍_fail");
		break;
	case IORI_KUZUKAZE_SUCCESS:
		GetAnimatorEX()->ChangeAnimation(L"설풍_success", 4, -1, 0);
		break;
	case IORI_KTYPE1211_YAOTOME_START:
		GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀_start", 5, -1, 0);
		break;
	case IORI_KTYPE1211_YAOTOME:
		GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀", 46, -1, 0);
		break;
	case IORI_UTYPE316_SAIKA:
		GetAnimatorEX()->ChangeAnimation(L"리 316식 시화");
		break;
	case IORI_CROUCH_LP:
		GetAnimatorEX()->ChangeAnimation(L"CrouchLP", 4, -1, 0);
		break;
	case IORI_CROUCH_SP:
		GetAnimatorEX()->ChangeAnimation(L"CrouchSP", 8, -1, 0);
		break;
	case IORI_CROUCH_LK:
		GetAnimatorEX()->ChangeAnimation(L"GroundLK", 3, -1, 0);
		break;
	case IORI_CROUCH_SK:
		GetAnimatorEX()->ChangeAnimation(L"CrouchSK", 6, -1, 0);
		break;
	case IORI_JUMP_LP:
		GetAnimatorEX()->ChangeAnimation(L"JumpLP", 2, -1, 0);
		break;
	case IORI_JUMP_SP:
		GetAnimatorEX()->ChangeAnimation(L"JumpSP", 7, -1, 0);
		break;
	case IORI_JUMP_LK:
		GetAnimatorEX()->ChangeAnimation(L"JumpLK", 1, -1, 0);
		break;
	case IORI_JUMP_SK:
		GetAnimatorEX()->ChangeAnimation(L"JumpSK", 4, -1, 0);
		break;
	case IORI_OUTTYPE_YURIORI:
		GetAnimatorEX()->ChangeAnimation(L"외식 백합꺾기", 3, -1, 0);
		break;
	case IORI_HIT_DOWN:
		break;
	case IORI_HIT_MID:
		break;
	case IORI_HIT_UP:
		break;
	case IORI_HIT_AERIAL:
		break;
	case IORI_HIT_KNOCKDOWN:
		break;
	case IORI_HIT_STANDUP:
		break;
	case IORI_HIT_STANDUP_QUICK:
		GetAnimatorEX()->ChangeAnimation(L"StandUp2", 3, -1, 0);
		break;
	case IORI_END:
		break;
	default:
		break;
	}
}

void CPlayerScIori::SoundOutput(const wstring & _strSound, int _iBefIdx)
{
	if (m_iBeforeIdx == _iBefIdx)
	{
		if (!m_bSound)
		{
			m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
			m_pSound->Play(1);
			m_bSound = true;
		}
	}
}

void CPlayerScIori::SoundOutput(const wstring & _strSound)
{
	if (!m_bSound)
	{
		m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
		m_pSound->Play(1);
		m_bSound = true;
	}
}

void CPlayerScIori::SoundOutputAnimFin(const wstring & _strSound)
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		if (!m_bSound)
		{
			m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
			m_pSound->Play(1);
			m_bSound = true;
		}
	}
}

void CPlayerScIori::OnCollisionEnter(CCollider * _pOther)
{
	if (m_bGuard == true)
	{
		m_eState = IORI_GUARD;
		GetAnimatorEX()->ChangeAnimation(L"Guard", 0, -1, 0);
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

	CGameObject* pOtherPlayer = _pOther->GetGameObject()->GetParent();
	HIT_STATE eOtherHitState = HIT_DOWN;

	if (_pOther->GetGameObject()->GetParent() != NULL)
	{
		pOtherPlayer = _pOther->GetGameObject()->GetParent();
		eOtherHitState = ((CCharactorScript*)pOtherPlayer->GetFirstScript())->GetHitState();
	}
	else
	{
		pOtherPlayer = _pOther->GetGameObject();
	}

	if (m_eState == IORI_KTYPE1211_YAOTOME || m_eState == IORI_KTYPE1211_YAOTOME_START
		|| m_eState == IORI_UTYPE316_SAIKA)
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
	}

	if (HIT_UP == eOtherHitState)
	{
		m_eState = IORI_HIT_UP;
		GetAnimatorEX()->ChangeAnimation(L"Hit1", 3, -1, 0);
	}
	else if (HIT_MIDDLE == eOtherHitState)
	{
		m_eState = IORI_HIT_MID;
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
	}
	else if (HIT_DOWN == eOtherHitState)
	{
		m_eState = IORI_HIT_DOWN;
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
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
		m_eState = IORI_HIT_AERIAL;
	}
	else if (HIT_KNOCKDOWN == eOtherHitState)
	{
		CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00003.wav");
		pSound->Play(1);
		CEffectManager::GetInst()->ActiveEffect(L"Circle2", m_vecPos + Vec3(0.f, -110.f, 0.f));
		m_eState = IORI_HIT_KNOCKDOWN;
		GetAnimatorEX()->ChangeAnimation(L"Knockdown", 2, -1, 0);
	}
}

void CPlayerScIori::CommandInput()
{
	if (KEYDOWN_PLAYER(m_tKey.Z)|| KEYDOWN_PLAYER(m_tKey.X) 
		|| KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S))
		m_fTime -= 0.15f;
	
	if (INPUT_PLAYER(m_tKey.Left, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
			m_vecCmd.push_back(KEY_TYPE::KEY_RIGHT);
		else if (m_iDir == DIR_RIGHT)
			m_vecCmd.push_back(KEY_TYPE::KEY_LEFT);
		m_fTime -= 0.15f;
	
	}
	if (INPUT_PLAYER(m_tKey.Right, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
			m_vecCmd.push_back(KEY_TYPE::KEY_LEFT);
		else if (m_iDir == DIR_RIGHT)
			m_vecCmd.push_back(KEY_TYPE::KEY_RIGHT);		
		m_fTime -= 0.15f;
	}
	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
		m_fTime -= 0.15f;
	if (INPUT_PLAYER(m_tKey.Down, STATE_DOWN))
	{
		m_vecCmd.push_back(KEY_TYPE::KEY_DOWN);
		m_fTime -= 0.15f;
	}

	if (m_fTime < 0.f)
		m_fTime = 0.f;
}

void CPlayerScIori::CommandDecode()
{
	if (CommandDecode_Size8() == RET_FAIL)
	{
		if (CommandDecode_Size6() == RET_FAIL)
		{
			if (CommandDecode_Size5() == RET_FAIL)
			{
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
			}
		}
	}

	m_fTime = 0.f;
	m_vecCmd.clear();
}

int CPlayerScIori::CommandDecode_Size8()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 8)
		return RET_FAIL;

	if (m_eState == IORI_KTYPE1211_YAOTOME)
	{
		if (m_vecCmd[iSize - 8] == KEY_DOWN&& m_vecCmd[iSize - 7] == KEY_RIGHT
			&&m_vecCmd[iSize-6] == KEY_DOWN&& m_vecCmd[iSize-5] == KEY_RIGHT
			&&m_vecCmd[iSize-4] == KEY_DOWN&& m_vecCmd[iSize-3] == KEY_RIGHT
			&&m_vecCmd[iSize - 2] == KEY_DOWN&& m_vecCmd[iSize-1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				m_iCmdLinkCnt = 1;
				iRet = RET_SUCCESS;
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size6()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 6)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_CROUCH
		|| m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if (m_vecCmd[iSize - 6] == KEY_DOWN&& m_vecCmd[iSize - 5] == KEY_LEFT
			&&m_vecCmd[iSize-4] == KEY_DOWN&&m_vecCmd[iSize - 3] == KEY_RIGHT
			&&m_vecCmd[iSize-2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			//GetAnimatorEX()->ChangeAnimation(L"리 1029식 염구", 21, -1, 0);
			//m_eState = IORI_UTYPE1029_HOMURABOTOGI_START;
			//iRet = RET_SUCCESS;
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size5()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 5)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_CROUCH
		|| m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if (m_vecCmd[iSize - 5] == KEY_DOWN&&
			m_vecCmd[iSize - 4] == KEY_LEFT&& m_vecCmd[iSize - 3] == KEY_DOWN
			&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀_start", 5, -1, 0);
				m_eState = IORI_KTYPE1211_YAOTOME_START;
				iRet = RET_SUCCESS;
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size4()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 4)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_CROUCH
		|| m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if (m_vecCmd[iSize - 4] == KEY_DOWN&& m_vecCmd[iSize - 3] == KEY_RIGHT
			&&m_vecCmd[iSize - 2] == KEY_DOWN&&m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀_start", 5, -1, 0);
				m_eState = IORI_KTYPE1211_YAOTOME_START;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 4] == KEY_RIGHT&& m_vecCmd[iSize - 3] == KEY_DOWN
			&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"설풍_start", 3, -1, 0);
				m_iSkillNum = 2;
				m_eState = IORI_KUZUKAZE_START;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 4] == KEY_LEFT&& m_vecCmd[iSize - 3] == KEY_RIGHT
			&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 12, -1, 0);
				m_eState = IORI_TYPE100_ONIYAKI;
				iRet = RET_SUCCESS;

				if (KEYDOWN_PLAYER(m_tKey.Z))
					m_iCmdLinkCnt = 0;
				else if (KEYDOWN_PLAYER(m_tKey.X))
					m_iCmdLinkCnt = 1;
			}
		}
		else if (m_vecCmd[iSize - 4] == KEY_DOWN&& m_vecCmd[iSize - 3] == KEY_RIGHT
			&&m_vecCmd[iSize - 2] == KEY_DOWN&&m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"리 316식 시화");
				m_eState = IORI_UTYPE316_SAIKA;
				iRet = RET_SUCCESS;
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size3()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize <3)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_CROUCH
		|| m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if ((m_eState == IORI_CROUCH&&m_vecCmd[iSize - 3] == KEY_RIGHT
			&&m_vecCmd[iSize - 2] == KEY_DOWN&&m_vecCmd[iSize - 1] == KEY_LEFT)||
			//단축 커맨드
			(m_vecCmd[iSize - 3] == KEY_DOWN
				&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT))
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀_start", 5, -1, 0);
				m_eState = IORI_KTYPE1211_YAOTOME_START;
				iRet = RET_SUCCESS;
			}
		}
		else if ((m_eState == IORI_FRONTWALK&& m_vecCmd[iSize - 3] == KEY_DOWN
			&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT)||
			(m_vecCmd[iSize - 3] == KEY_RIGHT//단축 커맨드
				&&m_vecCmd[iSize - 2] == KEY_LEFT&&m_vecCmd[iSize - 1] == KEY_RIGHT))
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"설풍_start", 3, -1, 0);
				m_iSkillNum = 2;
				m_eState = IORI_KUZUKAZE_START;
				iRet = RET_SUCCESS;
			}
		}
		else
		{
			if (m_vecCmd[iSize - 3] == KEY_RIGHT&& m_vecCmd[iSize - 2] == KEY_DOWN
				&&m_vecCmd[iSize - 1] == KEY_RIGHT)
			{
				if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
				{
					GetAnimatorEX()->ChangeAnimation(L"100식 귀신태우기", 12, -1, 0);
					m_eState = IORI_TYPE100_ONIYAKI;
					iRet = RET_SUCCESS;

					if (KEYDOWN_PLAYER(m_tKey.Z))
						m_iCmdLinkCnt = 0;
					else if (KEYDOWN_PLAYER(m_tKey.X))
						m_iCmdLinkCnt = 1;
				}
			}
			else if (m_vecCmd[iSize - 3] == KEY_RIGHT&& m_vecCmd[iSize - 2] == KEY_DOWN
				&&m_vecCmd[iSize - 1] == KEY_LEFT)
			{
				if (KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S))
				{
					GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_start", 7, -1, 0);
					m_iSkillNum = 1;
					m_eState = IORI_TYPE212_KOTOTSUKI_START;
					iRet = RET_SUCCESS;
				}
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size2()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 2)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_CROUCH
		|| m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if (m_vecCmd[iSize - 2] == KEY_DOWN&& m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"108식 어둠쫒기", 7, -1, 0);
				m_bEffect = false;
				m_eState = IORI_TYPE108_YAMIBARAI;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 2] == KEY_RIGHT&& m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S))
			{
				GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_start", 7, -1, 0);
				m_iSkillNum = 1;
				m_eState = IORI_TYPE212_KOTOTSUKI_START;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 2] == KEY_DOWN&& m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (m_eState == IORI_FRONTWALK)
			{
				if (KEYDOWN_PLAYER(m_tKey.A) || KEYDOWN_PLAYER(m_tKey.S))
				{
					GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_start", 7, -1, 0);
					m_iSkillNum = 1;
					m_eState = IORI_TYPE212_KOTOTSUKI_START;
					iRet = RET_SUCCESS;
				}
			}
			else
			{
				if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
				{
					GetAnimatorEX()->ChangeAnimation(L"127식 규화 1", 4, -1, 0);
					m_eState = IORI_TYPE127_AOIHANA_A;
					iRet = RET_SUCCESS;
				}
			}
		}
	}
	else if (m_eState == IORI_TYPE127_AOIHANA_A)
	{
		if (m_vecCmd[iSize - 2] == KEY_DOWN&& m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"127식 규화 2", 4, -1, 0);
				m_eState = IORI_TYPE127_AOIHANA_B;
				iRet = RET_SUCCESS;
			}
		}
	}
	else if (m_eState == IORI_TYPE127_AOIHANA_B)
	{
		if (m_vecCmd[iSize - 2] == KEY_DOWN&& m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"127식 규화 3", 4, -1, 0);
				m_eState = IORI_TYPE127_AOIHANA_C;
				iRet = RET_SUCCESS;
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size1()
{
	int iSize = m_vecCmd.size();
	int iRet = RET_FAIL;

	if (iSize < 1)
		return RET_FAIL;

	if (m_eState == IORI_IDLE || m_eState == IORI_BACKWALK || m_eState == IORI_FRONTWALK)
	{
		if (KEYDOWN_PLAYER(m_tKey.Z))
		{
			if (m_vecCmd[iSize - 1] == KEY_RIGHT)
			{
				GetAnimatorEX()->ChangeAnimation(L"외식 몽탄 1", 5, -1, 0);
				m_eState = IORI_OUTTYPE_YUMEBIKI_A;
				iRet = RET_SUCCESS;
			}
		}

		if (KEYDOWN_PLAYER(m_tKey.A))
		{
			if (m_vecCmd[iSize - 1] == KEY_RIGHT)
			{
				GetAnimatorEX()->ChangeAnimation(L"외식 굉부 음", 12, -1, 0);
				m_eState = IORI_OUTTYPE_GOUFU;
				iRet = RET_SUCCESS;
			}
		}
	}
	else if (m_eState == IORI_CROUCH)
	{
		if (m_vecCmd[iSize - 1] == KEY_RIGHT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"108식 어둠쫒기", 7, -1, 0);
				m_bEffect = false;
				m_eState = IORI_TYPE108_YAMIBARAI;
				iRet = RET_SUCCESS;
			}
		}
		else if (m_vecCmd[iSize - 1] == KEY_LEFT)
		{
			if (KEYDOWN_PLAYER(m_tKey.Z) || KEYDOWN_PLAYER(m_tKey.X))
			{
				GetAnimatorEX()->ChangeAnimation(L"127식 규화 1", 4, -1, 0);
				m_eState = IORI_TYPE127_AOIHANA_A;
				iRet = RET_SUCCESS;
			}
		}
	}
	if (m_eState == IORI_JUMP_BACK || m_eState == IORI_JUMP_UP ||
		m_eState == IORI_JUMP_FRONT || m_eState == IORI_JUMP_DOWN)
	{
		if (KEYDOWN_PLAYER(m_tKey.A))
		{
			if (m_vecCmd[iSize - 1] == KEY_LEFT)
			{
				GetAnimatorEX()->ChangeAnimation(L"외식 백합꺾기", 3, -1, 0);
				m_eState = IORI_OUTTYPE_YURIORI;
				iRet = RET_SUCCESS;
			}
		}
	}

	return iRet;
}

int CPlayerScIori::CommandDecode_Size0()
{
	if (m_eState == IORI_IDLE || m_eState == IORI_BACKWALK)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundLP", 5, -1, 0);
			m_eState = IORI_LP;
		}
		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSP", 8, -1, 0);
			m_eState = IORI_SP;
		}
		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundLK", 3, -1, 0);
			m_eState = IORI_LK;
		}
		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSK", 8, -1, 0);
			m_eState = IORI_SK;
		}

		m_iCmdLinkCnt = 0;
	}

	if (m_eState == IORI_FRONTWALK)
	{
		if (KEYDOWN_PLAYER(m_tKey.Z))
		{
			GetAnimatorEX()->ChangeAnimation(L"외식 몽탄 1", 5, -1, 0);
			m_eState = IORI_OUTTYPE_YUMEBIKI_A;
		}
		if (KEYDOWN_PLAYER(m_tKey.X))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSP", 8, -1, 0);
			m_eState = IORI_SP;
		}
		if (KEYDOWN_PLAYER(m_tKey.A))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundLK", 3, -1, 0);
			m_eState = IORI_LK;
		}
		if (KEYDOWN_PLAYER(m_tKey.S))
		{
			GetAnimatorEX()->ChangeAnimation(L"GroundSK", 8, -1, 0);
			m_eState = IORI_SK;
		}
	}

	else if (m_eState == IORI_CROUCH)
	{
		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchLP", 4, -1, 0);
			m_eState = IORI_CROUCH_LP;
		}
		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchSP", 8, -1, 0);
			m_eState = IORI_CROUCH_SP;
		}

		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchLK", 3, -1, 0);
			m_eState = IORI_CROUCH_LK;
		}

		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"CrouchSK", 6, -1, 0);
			m_eState = IORI_CROUCH_SK;
		}
	}
	else if (m_eState == IORI_JUMP_BACK || m_eState == IORI_JUMP_DOWN
		|| m_eState == IORI_JUMP_FRONT || m_eState == IORI_JUMP_UP)
	{

		if (INPUT_PLAYER(m_tKey.Z, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpLP", 2, -1, 0);
			m_eState = IORI_JUMP_LP;
		}

		if (INPUT_PLAYER(m_tKey.X, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpSP", 7, -1, 0);
			m_eState = IORI_JUMP_SP;
		}

		if (INPUT_PLAYER(m_tKey.A, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpLK", 1, -1, 0);
			m_eState = IORI_JUMP_LK;
		}

		if (INPUT_PLAYER(m_tKey.S, STATE_DOWN))
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpSK", 4, -1, 0);
			m_eState = IORI_JUMP_SK;
		}
	}
	else if (m_eState == IORI_OUTTYPE_YUMEBIKI_A)
	{
		if (KEYDOWN_PLAYER(m_tKey.Z))
		{
			GetAnimatorEX()->ChangeAnimation(L"외식 몽탄 2", 5, -1, 0);
			m_eState = IORI_OUTTYPE_YUMEBIKI_B;
		}
	}

	return 0;
}

void CPlayerScIori::Gravity()
{
	if (m_fStunHitTime == 0 && m_fStunAttTime == 0)
	{
		if (m_bOnGround == true)
			return;

		m_vecPos = GetTransform()->GetLocalPos();
		Vec3 vScale = GetTransform()->GetLocalScale();

		m_vecPos.y += m_fJumpSpeed * DT;
		m_fGravity += m_fGravAccel * DT;
		m_vecPos.y -= m_fGravity * DT;

		GetTransform()->SetLocalPos(m_vecPos);
	}

	else
	{
		if (m_fStunHitTime != 0)
		{
			if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
			{
				int iMaxFrame = GetAnimatorEX()->GetCurAnim()->GetMaxFrameCnt();

				m_fStunTime += DT;
				if (m_iBeforeIdx > 2&&iMaxFrame>3)
				{
					GetAnimatorEX()->GetCurAnim()->SetFrameIndex(3);
					GetAnimatorEX()->SetPause(true);
				}
				else
				{
					GetAnimatorEX()->GetCurAnim()->SetFrameIndex(1);
					GetAnimatorEX()->SetPause(true);
				}
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

void CPlayerScIori::Idle()
{
	m_iSkillNum = 0;
	m_eHitState = HIT_UP;
	m_fKnockback = 0.f;
	m_bEffect = false;
	m_bGuard = false;
	m_fStunTime = 0.f;
	m_fStunHitTime = 0.f;
	m_bSound = true;
	m_iCmdLinkCnt = 0;

	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_RIGHT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_LEFT))
	{
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		m_eState = IORI_BACKWALK;
	}

	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_LEFT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_RIGHT))
	{
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 0);
		m_eState = IORI_FRONTWALK;
	}

	else if (INPUT_PLAYER(m_tKey.Down, STATE_PUSH))
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch", 1, 1, 1);
		m_eState = IORI_CROUCH;
	}

	else if (KEYDOWN_PLAYER(m_tKey.Up))
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpUp", 1, -1, 0);
		m_eState = IORI_JUMP_UP;
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
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
					GetAnimatorEX()->ChangeAnimation(L"BackDash");
					m_eState = IORI_BACKDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
				else if (m_vecCmd[iSize - 2] == m_tKey.Right&&m_vecCmd[iSize - 1] == m_tKey.Right)
				{
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
					GetAnimatorEX()->ChangeAnimation(L"FrontDash");
					m_eState = IORI_FRONTDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
			}
		}
	}
}

void CPlayerScIori::FrontWalk()
{
	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_RIGHT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_LEFT))
	{
		GetAnimatorEX()->ChangeAnimation(L"BackWalk", 0);
		m_eState = IORI_BACKWALK;
	}

	if (KEYUP_PLAYER(m_tKey.Left) || KEYUP_PLAYER(m_tKey.Right))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle", 0);
		m_eState = IORI_IDLE;
	}

	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
	{
		m_eState = IORI_JUMP_FRONT;
		GetAnimatorEX()->ChangeAnimation(L"JumpFront");
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;
	}
}

void CPlayerScIori::BackWalk()
{
	m_bGuard = true;

	if ((INPUT_PLAYER(m_tKey.Left, STATE_PUSH) && m_iDir == DIR_LEFT)
		|| (INPUT_PLAYER(m_tKey.Right, STATE_PUSH) && m_iDir == DIR_RIGHT))
	{
		m_bGuard = false;
		GetAnimatorEX()->ChangeAnimation(L"FrontWalk", 4, -1, 0);
		m_eState = IORI_FRONTWALK;
	}

	if (KEYUP_PLAYER(m_tKey.Left) || KEYUP_PLAYER(m_tKey.Right))
	{
		m_bGuard = false;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}

	if (INPUT_PLAYER(m_tKey.Up, STATE_DOWN))
	{
		m_bGuard = false;
		m_eState = IORI_JUMP_BACK;
		GetAnimatorEX()->ChangeAnimation(L"JumpBack");
		m_bOnGround = false;
		m_fJumpSpeed = 1300.f;
	}
}

void CPlayerScIori::FrontDash()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::BackDash()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Crouch()
{
	m_fKnockback = 0.f;
	if (INPUT_PLAYER(m_tKey.Down, STATE_PUSH))
	{
	}
	
	if (KEYPUSH_PLAYER(m_tKey.Left) && m_iDir == DIR_RIGHT
		|| KEYPUSH_PLAYER(m_tKey.Right) && m_iDir == DIR_LEFT)
	{
		m_bGuard = true;
	}

	if (CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_UP, m_b1Player))
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::JumpUp()
{
	m_bOnGround = false;
	m_fJumpSpeed = 1300.f;

	if (INPUT_PLAYER(m_tKey.Left, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = IORI_JUMP_FRONT;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = IORI_JUMP_BACK;
		}
	}

	if (INPUT_PLAYER(m_tKey.Right, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = IORI_JUMP_BACK;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = IORI_JUMP_FRONT;
		}
	}

	if (m_iBeforeIdx == 2)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}

	if (m_bOnGround == true)
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::JumpDown()
{
	if (INPUT_PLAYER(m_tKey.Left, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = IORI_JUMP_FRONT;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = IORI_JUMP_BACK;
		}
	}

	if (INPUT_PLAYER(m_tKey.Right, STATE_DOWN))
	{
		if (m_iDir == DIR_LEFT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpBack");
			m_eState = IORI_JUMP_BACK;
		}
		if (m_iDir == DIR_RIGHT)
		{
			GetAnimatorEX()->ChangeAnimation(L"JumpFront");
			m_eState = IORI_JUMP_FRONT;
		}
	}

	if (m_bOnGround == true)
	{
		m_fJumpSpeed = 0.f;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::JumpFront()
{
	m_fJumpSpeed = 1300.f;

	if (m_bOnGround == true)
	{
		m_fJumpSpeed = 0;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
	
}

void CPlayerScIori::JumpBack()
{
	m_fJumpSpeed = 1300.f;

	if (m_bOnGround == true)
	{
		m_fJumpSpeed = 0.f;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Roll()
{
	if (m_bOnGround == true)
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Guard()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
		m_bGuard = false;
	}
}

void CPlayerScIori::GroundLP()
{
	if (m_iBeforeIdx != 4)
		m_bSound = false;
	SoundOutput(L"Iori_Att1.wav",4);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::GroundSP()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Att3.wav", 2);

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::GroundLK()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	else
	SoundOutput(L"Iori_Att2.wav");

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::GroundSK()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	else
		SoundOutput(L"Iori_Att4.wav");

	m_eHitState = HIT_MIDDLE;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::OutType_Yumebiki_A()
{
	if (m_iBeforeIdx != 4)
		m_bSound = false;
	SoundOutput(L"Iori_Yumebiki1.wav", 4);

	m_eHitState = HIT_UP;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::OutType_Yumebiki_B()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Yumebiki2.wav",2);

	m_eHitState = HIT_UP;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::OutType_Goufu()
{
	m_eHitState = HIT_UP;
	m_iCmdLinkCnt = 0;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Type100_Oniyaki()
{
	if (m_iBeforeIdx != 4)
		m_bSound = false;

	int iRand = getRandomNumber_int(0, 1);
	if (0 == iRand)
		SoundOutput(L"Iori_Oniyaki1.wav", 4);
	else
		SoundOutput(L"Iori_Oniyaki2.wav", 4);

	m_eHitState = HIT_AIRUP;

	if (m_iBeforeIdx == 5)
	{
		m_bOnGround = false;
		m_fJumpSpeed = 800.f+500.f*m_iCmdLinkCnt;
	}

	if (GetAnimatorEX()->IsAnimFinish() ||(m_bOnGround&&m_iBeforeIdx>5))
	{
		m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);
		m_pEffect->GetTransform()->SetLocalScale(Vec3One);

		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState =IORI_IDLE;
	}

	if (false == m_bEffect)
	{
		if (m_iBeforeIdx == 4)
		{
			//CEffectManager::GetInst()->ActiveEffect(L"Iori_100식", m_vecPos);
			//CEffectManager::GetInst()->FollowPos(L"Iori_100식", m_vecPos, m_iDir - 1);
			m_pEffect->GetAnimatorEX()->ChangeAnimation(L"100식");
			m_pEffect->GetTransform()->SetLocalScale(Vec3(1.5f, 1.5f, 1.f));
			m_bEffect = true;
		}
	}
}

void CPlayerScIori::Type108_Yamibarai()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	SoundOutput(L"Iori_Yamibarai.wav", 3);

	m_eHitState = HIT_DOWN;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}

	if (false == m_bEffect)
	{
		if (m_iBeforeIdx == 2)
		{
			CEffectManager::GetInst()->ActiveEffect(L"Iori_108식", m_vecPos);
			CEffectManager::GetInst()->FollowPos(L"Iori_108식", m_vecPos, m_iDir - 1);

			//CEffectManager::GetInst()->FollowPos(L"IoriBullet_108식", m_vecPos, m_iDir - 1);

			m_bEffect = true;
		}
	}
}

void CPlayerScIori::Type127_Aoihana_A()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Aoihana1.wav", 2);

	m_eHitState = HIT_UP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Type127_Aoihana_B()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Aoihana2.wav", 1);

	m_eHitState = HIT_UP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState =IORI_IDLE;
	}
}

void CPlayerScIori::Type127_Aoihana_C()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Aoihana3.wav", 2);

	m_eHitState = HIT_KNOCKDOWN;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Type212_Kototsuki()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Kototsuki1.wav", 1);

	m_eHitState = HIT_KNOCKDOWN;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_fail");
		m_eState = IORI_TYPE212_KOTOTSUKI_FAIL;
	}
}

void CPlayerScIori::Type212_Kototsuki_Fail()
{
	if (m_iBeforeIdx !=1)
		m_bSound = false;

	SoundOutput(L"Iori_CatchFailed.wav",1);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Type212_Kototsuki_Success()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	SoundOutput(L"Iori_Kototsuki2.wav", 3);

	m_eHitState = HIT_KNOCKDOWN;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::Kuzukaze()
{
	m_eHitState = HIT_MIDDLE;
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"설풍_fail");
		m_eState = IORI_KUZUKAZE_FAIL;
	}
}

void CPlayerScIori::KuzukazeFail()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::KuzukazeSuccess()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::KType1211_Yaotome_Start()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Yaotome1.wav", 1);

	if (!m_bEffect)
	{
		Vec3 vPos = GetTransform()->GetLocalPos();
		CEffectManager::GetInst()->ActiveEffect(L"SuperSpark1", vPos);
		CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00049.wav");
		pSound->Play(1);
		m_bEffect = true;
	}
	
	if (m_iBeforeIdx == 1)
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Gray");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(true);
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(false);
		GetAnimatorEX()->ChangeAnimation(L"Idle");

		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::KType1211_Yaotome()
{
	if(m_iBeforeIdx==0)
		m_pSound->Stop(true);

	if (m_iBeforeIdx == 1)
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Gray");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(true);
	}

	if (m_iBeforeIdx ==4)
		m_eHitState = HIT_UP;
	else if (m_iBeforeIdx == 31)
		m_eHitState = HIT_DOWN;
	else if (m_iBeforeIdx == 39)
		m_eHitState = HIT_MIDDLE;
	
	if (m_iBeforeIdx != 1&&m_iBeforeIdx!=39)
		m_bSound = false;

	SoundOutput(L"Iori_Yaotome2.wav", 1);
	SoundOutput(L"Iori_Saika4.wav", 39);

	m_bEffect = false;

	if (m_iBeforeIdx == 39)
	{
		m_pEffect->GetAnimatorEX()->ChangeAnimation(L"금 1211식 팔치녀");
	}
	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_pEffect->GetAnimatorEX()->SetCurAnim(NULL);

		if (m_iCmdLinkCnt == 1)
		{
			CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_Reverse");
			CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(true);
			CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
			Vec3 vPos = GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"SuperSpark2", vPos);
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00049.wav");
			pSound->Play(1);
			GetAnimatorEX()->ChangeAnimation(L"리 316식 시화");
			m_eState = IORI_UTYPE316_SAIKA;
		}

		else
		{
			CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
			CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(false);
			CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
			CGameManager::GetInst()->GetSound()->Stop(false);

			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_eState = IORI_IDLE;
		}
	}
}

void CPlayerScIori::UType316_Saika()
{
	if (m_iBeforeIdx != 2 && m_iBeforeIdx != 9)
		m_bSound = false;
	
	SoundOutput(L"Iori_Saika1.wav", 2);
	SoundOutput(L"Iori_Saika3.wav", 9);

	m_eHitState = HIT_AIRUP;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetAnimatorEX()->SetPause(false);
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		//CGameManager::GetInst()->GetSound()->Stop(false);
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
		m_eHitState = HIT_UP;
	}

	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (false == m_bEffect)
	{
		if (iCurFrame ==1&&m_iCmdLinkCnt!=1)
		{
			Vec3 vPos = GetTransform()->GetLocalPos();
			CEffectManager::GetInst()->ActiveEffect(L"SuperSpark1", vPos);
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00049.wav");
			pSound->Play(1);
			m_bEffect = true;
		}

		if (iCurFrame == 8)
		{
			CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00013.wav");
			pSound->Play(1);
			CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D_AllRed");
			CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
			CEffectManager::GetInst()->ActiveEffect(L"Iori_316식", m_vecPos+Vec3(150.f*m_iDir,135.f,0.f));
			CEffectManager::GetInst()->FollowPos(L"Iori_316식", m_vecPos+Vec3(150.f*m_iDir, 135.f, 0.f), m_iDir - 1);
			m_bEffect = true;
		}
	}

	if (iCurFrame != 1&& iCurFrame != 8)
		m_bEffect = false;
}

void CPlayerScIori::UType1029_Homurabotogi_Start()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		GetAnimatorEX()->ChangeAnimation(L"212식 금월 음_fail");
		m_eState = IORI_TYPE212_KOTOTSUKI_FAIL;
	}
}

void CPlayerScIori::UType1029_Homurabotogi()
{
	if (GetAnimatorEX()->IsAnimFinish())
	{
		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		CPlayerManager::GetInst()->GetMap()->GetMeshRender()->SetMaterial(pMtrl);
		GetAnimatorEX()->ChangeAnimation(L"Idle");
		m_eState = IORI_IDLE;
	}
}

void CPlayerScIori::CrouchLP()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Att1.wav", 2);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch");
		m_eState = IORI_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScIori::CrouchSP()
{
	if (m_iBeforeIdx != 3)
		m_bSound = false;
	SoundOutput(L"Iori_Att3.wav", 3);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch");
		m_eState = IORI_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScIori::CrouchLK()
{
	if (m_iBeforeIdx !=1)
		m_bSound = false;
	SoundOutput(L"Iori_Att2.wav",1);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch");
		m_eState = IORI_CROUCH;
	}
	m_eHitState = HIT_DOWN;
}

void CPlayerScIori::CrouchSK()
{
	if (m_iBeforeIdx != 2)
		m_bSound = false;
	SoundOutput(L"Iori_Att4.wav", 2);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		GetAnimatorEX()->ChangeAnimation(L"Crouch");
		m_eState = IORI_CROUCH;
	}
	m_eHitState = HIT_AIRUP;
}

void CPlayerScIori::JumpLP()
{

	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;

	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScIori::JumpSP()
{
	
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;

	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScIori::JumpLK()
{

	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;

	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScIori::JumpSK()
{
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;

	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}
	m_eHitState = HIT_UP;
}

void CPlayerScIori::OutType_Yuriori()
{
	m_eHitState = HIT_UP;
	
	if (KEYPUSH_PLAYER(m_tKey.Left))
		m_vecPos.x -= 600.f*DT;
	
	if (KEYPUSH_PLAYER(m_tKey.Right))
		m_vecPos.x += 600.f*DT;

	if (GetAnimatorEX()->IsAnimFinish() || m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
		m_eState = IORI_JUMP_DOWN;
	}
}

void CPlayerScIori::HitDown()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Hit1.wav", 1);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = IORI_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScIori::HitMid()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Hit2.wav", 1);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = IORI_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScIori::HitUp()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Hit3.wav", 1);

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = IORI_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CPlayerScIori::HitAerial()
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

	if (GetAnimatorEX()->IsAnimFinish()&&m_bOnGround)
	{
		m_eState = IORI_HIT_KNOCKDOWN;
		GetAnimatorEX()->ChangeAnimation(L"Knockdown",2,-1,0);
		CEffectManager::GetInst()->ActiveEffect(L"Circle2", m_vecPos + Vec3(0.f, -110.f, 0.f));
	}
}

void CPlayerScIori::HitKnockdown()
{
	if (m_iBeforeIdx != 1)
		m_bSound = false;
	SoundOutput(L"Iori_Hit5.wav", 1);

	if (m_bOnGround)
		m_fKnockback = 0.f;

	if (GetAnimatorEX()->IsAnimFinish())
	{
		if (!m_bDefeated)
		{
			m_fKnockback = 0.f;
			m_eState = IORI_HIT_STANDUP_QUICK;
			GetAnimatorEX()->ChangeAnimation(L"StandUp2",3,-1,0);
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

void CPlayerScIori::StandUp()
{
}

void CPlayerScIori::StandUpQuick()
{
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
					m_eState = IORI_BACKDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
				else if (m_vecCmd[iSize - 2] == m_tKey.Right&&m_vecCmd[iSize - 1] == m_tKey.Right)
				{
					CSound* pSound = (CSound*)CResManager::Manager().FindSound(L"common_00029.wav");
					pSound->Play(1);
					GetAnimatorEX()->ChangeAnimation(L"FrontDash");
					m_eState = IORI_FRONTDASH;
					m_bOnGround = false;
					m_fJumpSpeed = 500.f;
					m_vecCmd.clear();
				}
			}
		}
	}

	if (GetAnimatorEX()->IsAnimFinish())
	{
		m_eState = IORI_IDLE;
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}