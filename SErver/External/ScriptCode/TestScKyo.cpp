#include "stdafx.h"
#include "TestScKyo.h"

#include "Collider.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "GameObject.h"

#include "ShaderManager.h"
#include "ResManager.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Layer.h"

#include "Material.h"

#include "CameraScript.h"
#include "ScriptCode\ShadowScript.h"

CTestScKyo::CTestScKyo()
	: m_fTime(0.f)
	, m_bEffect(false)
	, m_bSound(false)
	, m_iBeforeIdx(-1)
{
	m_fJumpSpeed = 1800;
	m_fGravAccel = 3000.f;
	m_iDir = DIR_RIGHT;
	m_vecPos = Vec3Zero;
	m_fFloorY = -20.f;
}


CTestScKyo::~CTestScKyo()
{
}

void CTestScKyo::Awake()
{
}

void CTestScKyo::Start()
{
	CreateShadow();

	((CShadowScript*)(m_pShadow->GetFirstScript()))->init(GetGameObject(), 0.07f, 0.18f);
}

void CTestScKyo::Update()
{
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = GetTransform()->GetLocalScale().x;

	if (GetAnimatorEX() == NULL)
		return;

	if (m_iDir == DIR_LEFT)
		fDir = 300.f;
	else if (m_iDir == DIR_RIGHT)
		fDir = -300.f;

	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	bool bClipEnd = GetAnimatorEX()->GetCurAnim()->IsAnimFinish();
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurFrameCnt();
	int iMaxFrame = GetAnimatorEX()->GetCurAnim()->GetMaxFrameCnt();

	if (bClipEnd)
		ClipEnd();

	Aerial();

	if (strClipName == L"Hit1" || strClipName == L"Hit2")//쳐맞았다면
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
			m_fStunTime = 0.f;
			m_fStunHitTime = 0.f;
		}
	}
	else if (strClipName == L"Knockback")
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
			m_fStunTime = 0.f;
			m_fStunHitTime = 0.f;
		}
	}
	else if (strClipName == L"Knockdown")
	{
		if (m_fStunTime < m_fStunHitTime)//스턴 걸린 상태
		{
			m_fStunTime += DT;
			GetAnimatorEX()->GetCurAnim()->SetFrameIndex(2);
			GetAnimatorEX()->SetPause(true);
		}
		else
		{
			GetAnimatorEX()->SetPause(false);
		}
	}

	if ((INPUT(KEY_LEFT, STATE_UP) || INPUT(KEY_RIGHT, STATE_UP))
		&& (strClipName == L"BackWalk" || strClipName == L"FrontWalk" || strClipName == L"Run"))
		GetAnimatorEX()->ChangeAnimation(L"Idle", 9, -1, 0);

	GetTransform()->SetLocalScale(Vec3(fDir, vScale.y, vScale.z));
	GetTransform()->SetLocalPos(vPos);

	Gravity();
	EffectedTime();

#if defined DEBUG | _DEBUG
	//char str[255];
	//GET_SINGLE(CDebugMgr)->SetResetTime(0.1f);
	//sprintf_s(str, "Cmd : (%d, %f) \n", m_vecCmd.size(), m_fTime);
	//GET_SINGLE(CDebugMgr)->OutputConsole(str);
#endif
}

void CTestScKyo::LateUpdate()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();

	Vec3 vPos = GetTransform()->GetLocalPos();

	if (GetAnimatorEX() == NULL)
		return;

	if (vPos.y <= -20.f)
	{
		vPos.y = -20.f;
		m_fGravity = 0.f;
		m_fJumpSpeed = 0.f;
		m_bOnGround = true;
	}

	GetTransform()->SetLocalPos(vPos);
}

void CTestScKyo::FinalUpdate()
{
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();
	float fDir = vScale.x;

	if (m_iDir == DIR_RIGHT)
	{
		//vPos.x += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x*DT;
		vPos.x -= m_fKnockback*DT;
	}
	else
	{
	//	vPos.x -= GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.x*DT;
		vPos.x += m_fKnockback*DT;
	}

	vPos.y += GetAnimatorEX()->GetCurAnim()->GetCurFrameInfo()->tMove.y*DT;

	GetTransform()->SetLocalPos(vPos);

	m_iBeforeIdx = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	CCameraScript* CamScript = (CCameraScript*)CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetFirstScript();

	if (CamScript == NULL)
		return;

	if (vPos.x >= CamScript->GetMapScale().x / 2 - 70)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			vPos.x = CamScript->GetMapScale().x / 2 - 70;
			GetTransform()->SetLocalPos(vPos);
		}
	}

	if (vPos.x <= -CamScript->GetMapScale().x / 2 + 70)
	{
		if (0 != CamScript->GetMapScale().x)
		{
			vPos.x = -CamScript->GetMapScale().x / 2 + 70;
			GetTransform()->SetLocalPos(vPos);
		}
	}
}

void CTestScKyo::OnCollisionEnter(CCollider * _pOther)
{
	CGameObject* pOtherPlayer = NULL;
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

	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vOtherPos = pOtherPlayer->GetTransform()->GetLocalPos();//역가드 판정용

	if (HIT_UP == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit1", 3, -1, 0);
	}
	else if (HIT_MIDDLE == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Hit2", 4, -1, 0);
	}
	else if (HIT_DOWN == eOtherHitState)
	{
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
	}

	else if (HIT_KNOCKDOWN == eOtherHitState)
	{
		GetAnimatorEX()->ChangeAnimation(L"Knockdown", 2, -1, 0);
	}
}

void CTestScKyo::Aerial()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();
	bool bClipEnd = GetAnimatorEX()->GetCurAnim()->IsAnimFinish();

	if (strClipName == L"AirHit")
	{
		if (m_iBeforeIdx == 0)
		{
			m_fJumpSpeed = 1300.f;
			m_bOnGround = false;
		}
		if (m_bOnGround)
		{			
			if (bClipEnd)
			{
				GetAnimatorEX()->ChangeAnimation(L"StandUp2");
			}
		}	
	}

	if (strClipName == L"JumpDown" || strClipName == L"JumpFront" || strClipName == L"JumpBack")
	{
		if (m_bOnGround == true)
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle");
		}
	}
}

void CTestScKyo::ClipEnd()
{
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	float fDir = GetTransform()->GetLocalScale().x;

	if (strClipName != L"Idle"&&strClipName != L"JumpFront" && strClipName != L"JumpBack")
	{
		if (strClipName != L"Crouch"&&strClipName != L"AirHit")
			GetAnimatorEX()->ChangeAnimation(L"Idle");

		m_fKnockback = 0.f;
		m_fStunTime = 0.f;
		m_fStunHitTime = 0.f;

		CMaterial* pMtrl = CResManager::Manager().FindMaterial(L"Material_Std2D");
		GetMeshRender()->SetMaterial(pMtrl);
	}
	if (strClipName == L"JumpUp")
	{
		GetAnimatorEX()->ChangeAnimation(L"JumpDown");
	}
	if (strClipName == L"JumpDown"&&m_bOnGround)
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
	
	if (strClipName == L"Knockdown")
	{
		GetAnimatorEX()->ChangeAnimation(L"Idle");
	}
}

void CTestScKyo::Gravity()
{
	if (m_bOnGround == true)
		return;

	Vec3 vPos = GetTransform()->GetLocalPos();
	Vec3 vScale = GetTransform()->GetLocalScale();

	vPos.y += m_fJumpSpeed * DT;
	m_fGravity += m_fGravAccel * DT;
	vPos.y -= m_fGravity * DT;

	GetTransform()->SetLocalPos(vPos);
}
