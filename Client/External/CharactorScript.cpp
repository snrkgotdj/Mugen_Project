#include "stdafx.h"
#include "CharactorScript.h"

#include "SaveLoadManager.h"

#include "TransForm.h"
#include "Collider2D.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "ScriptCode\EffectScript.h"
#include "ScriptCode\CameraScript.h"
#include "ScriptCode\PlayerManager.h"
#include "..\Doom\define_server.h"
#include "NetworkManager.h"
#include "ClientManager.h"

CCharactorScript::CCharactorScript()
	: m_pAnimator(NULL)
	, m_pShadow(NULL)
	, m_pKeyCommand(NULL)
	, m_pTransForm(NULL)
	, m_vecPos{0,0,0}
	, m_vecPrevPos{0,0,0}
	, m_iBeforeIdx(-1)
	, m_fFloorY(0)
	, m_bOnline(false)
	, m_bOnGround(true)
	, m_bGuard(false)
	, m_bCatch(false)
	, m_bCrouch(false)
	, m_bEffected(false)
	, m_fGravity(0)
	, m_eHitState(HIT_UP)
	, m_fKnockback(0.f)
	, m_fStunTime(0.f)
	, m_fStunHitTime(0.f)
	, m_fStunAttTime(0.f)
	, m_fAccEffect(0.f)
	, m_iDir(DIR_LEFT)
	, m_bDefeated(false)
	, m_fEffectTimeLimit(0.5f)
	, m_pSound(NULL)
{
	m_pKeyCommand = NULL;
	m_tKey.Right = KEY_RIGHT;
	m_tKey.Left = KEY_LEFT;
	m_tKey.Up = KEY_UP;
	m_tKey.Down = KEY_DOWN;
	m_tKey.Z = KEY_Z;
	m_tKey.X = KEY_X;
	m_tKey.C = KEY_C;
	m_tKey.A = KEY_A;
	m_tKey.S = KEY_S;
	m_tKey.D = KEY_D;
}

CCharactorScript::~CCharactorScript()
{
	SAFE_DELETE(m_pKeyCommand);
}

void CCharactorScript::Awake()
{
	m_pAnimator = GetAnimatorEX();
	m_pTransForm = GetTransform();
	m_pKeyCommand = new CKeyCommand;
}

void CCharactorScript::FinalUpdate()
{
	if (m_pAnimator == NULL)
		return;

	if (m_iBeforeIdx != m_pAnimator->GetCurAnim()->GetCurIndex())
	{
		if (m_iDir == DIR_RIGHT)
		{
			m_vecPos = m_pTransForm->GetLocalPos();
			m_vecPos.x += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}
		else if (m_iDir == DIR_LEFT)
		{
			m_vecPos = m_pTransForm->GetLocalPos();
			m_vecPos.x -= m_pAnimator->GetCurAnim()->GetCurFrameInfo()->tMove.x;
			m_vecPos.y += m_pAnimator->GetCurAnim()->GetCurFrameInfo()->tMove.y;
		}
		
		m_pTransForm->SetLocalPos(m_vecPos);
		m_iBeforeIdx = m_pAnimator->GetCurAnim()->GetCurIndex();
	}

	CCameraScript* CamScript = (CCameraScript*)CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetFirstScript();
	
	if (CamScript == NULL)
		return;

	if (m_vecPos.x >= CamScript->GetMapScale().x / 2 - 50)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			m_vecPos.x = CamScript->GetMapScale().x / 2 - 50;
			m_pTransForm->SetLocalPos(m_vecPos);
		}
	}

	if (m_vecPos.x <= -CamScript->GetMapScale().x / 2 + 50)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			m_vecPos.x = -CamScript->GetMapScale().x / 2 + 50;
			m_pTransForm->SetLocalPos(m_vecPos);
		}
	}
}

void CCharactorScript::CreateShadow()
{
	wstring strShadowPath;
	strShadowPath = CPathManager::GetResPathEX();
	strShadowPath += L"Save\\GameObject\\Shadow.goc";
	m_pShadow = CSaveLoadManager::Manager().LoadGameObjectOnly(strShadowPath);
	CSceneManager::Manager().GetCurScene()->FindLayer(L"Shadow")->AddGameObject(m_pShadow);
}

void CCharactorScript::EffectedTime()
{
	if (m_bEffected == true)
	{
		m_fAccEffect += DT;
		if (m_fAccEffect >= m_fEffectTimeLimit)
		{
			CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
			GetMeshRender()->SetMaterial(pMtrl);
			m_bEffected = false;
			m_fAccEffect = 0;
			m_fEffectTimeLimit = 0.5f;
		}
	}
}

void CCharactorScript::SendPos()
{
	if (m_vecPos == m_vecPrevPos)
		return;

	PACKET_POS tPacket;
	tPacket.bPlayer = m_b1Player;
	tPacket.vecPos = m_vecPos;
	ServerEngine::CNetworkManager::GetInst()->SendMessageAll(PH_POS, (char*)&tPacket, sizeof(tPacket));
}

void CCharactorScript::SetEffected(bool _bTrue, float _fTime)
{
	m_bEffected = _bTrue; 
	m_fEffectTimeLimit = _fTime;
	m_fAccEffect = 0.f;
}

void CCharactorScript::Set1P()
{
	m_b1Player = true;
	if(NULL!=m_pKeyCommand)
	m_pKeyCommand->Set1P(true);
}

void CCharactorScript::Set2P()
{
	m_b1Player = false;
	if (NULL != m_pKeyCommand)
	m_pKeyCommand->Set1P(false);
}

void CCharactorScript::PushPlayerX(float _distance)
{
	Vec3 vecPos = GetTransform()->GetLocalPos();

	if (m_iDir == DIR_RIGHT)
	{
		vecPos.x -= _distance;
	}
	else if (m_iDir == DIR_LEFT)
	{
		vecPos.x += _distance;
	}
	
	GetTransform()->SetLocalPos(vecPos);
}

void CCharactorScript::Gravity()
{
	if (m_bOnGround == true)
		return;

	float delta = DT;

	m_vecPos.y += m_fJumpSpeed * delta;
	m_fGravity += m_fGravAccel * delta;
	m_vecPos.y -= m_fGravity * delta;

	GetTransform()->SetLocalPos(m_vecPos);
}

void CCharactorScript::SetState(UINT _iState)
{
}

void CCharactorScript::SetPos(const Vec3 & _vecPos)
{
	GetTransform()->SetLocalPos(_vecPos);
}

void CCharactorScript::SendKey()
{
	// 방향키 KEY_DOWN 이벤트
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Right, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Left, STATE_DOWN, m_b1Player);
	
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Up, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Down, STATE_DOWN, m_b1Player);

	// 공격키 KEY_DOWN이벤트
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.A, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.A, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.S, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.S, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.D, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.D, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Z, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.X, STATE_DOWN, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_DOWN, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.C, STATE_DOWN, m_b1Player);

	// 방향키 KEY_UP 이벤트
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Right, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Right, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Left, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Left, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Up, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Up, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Down, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Down, STATE_UP, m_b1Player);

	// 공격키 KEY_UP이벤트
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.A, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.A, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.S, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.S, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.D, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.D, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.Z, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.Z, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.X, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.X, STATE_UP, m_b1Player);
	if (true == CKeyManager::Manager().GetKeyState(m_tKey.C, STATE_UP, m_b1Player))
		CKeyManager::Manager().SendKey(m_tKey.C, STATE_UP, m_b1Player);
}

void CCharactorScript::SetOnlineKey()
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

void CCharactorScript::SoundOutput(const wstring & _strSound, int _iBefIdx)
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

void CCharactorScript::SoundOutput(const wstring & _strSound)
{
	if (!m_bSound)
	{
		m_pSound = (CSound*)CResManager::Manager().FindSound(_strSound);
		m_pSound->Play(1);
		m_bSound = true;
	}
}

void CCharactorScript::SoundOutputAnimFin(const wstring & _strSound)
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

