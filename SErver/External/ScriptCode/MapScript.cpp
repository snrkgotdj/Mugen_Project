#include "stdafx.h"
#include "AnimatorEX.h"
#include "SceneManager.h"
#include "MapScript.h"
#include "CameraScript.h"
#include "..\DoomEngine\func.h"

CMapScript::CMapScript()
	: m_pCurAnim(NULL)
{
}

CMapScript::~CMapScript()
{
}

void CMapScript::init(MAP_STATE _MapType)
{
	CGameObject* pCamera = CSceneManager::Manager().GetCurScene()->GetMainCamera();
	switch (_MapType)
	{
	case MAP_KOREA:
		GetTransform()->SetLocalPos(Vec3(0.f, 230.f, 100.f));
		GetTransform()->SetLocalScale(Vec3(1200.f, 800.f, 1.f));
		GetAnimatorEX()->ChangeAnimation(L"한국");
		pCamera->GetCamera()->SetScale(0.5f);
		break;

	case MAP_STADIUM:
		GetTransform()->SetLocalPos(Vec3(0.f, 230.f, 100.f));
		GetTransform()->SetLocalScale(Vec3(1200.f, 800.f, 1.f));
		GetAnimatorEX()->ChangeAnimation(L"스태디움");
		pCamera->GetCamera()->SetScale(0.5f);
		break;

	case MAP_TEMPLE:
		GetTransform()->SetLocalPos(Vec3(0.f, 230.f, 100.f));
		GetTransform()->SetLocalScale(Vec3(1200.f, 800.f, 1.f));
		GetAnimatorEX()->ChangeAnimation(L"사원");
		pCamera->GetCamera()->SetScale(0.5f);
		break;

	case MAP_CITY:
		GetTransform()->SetLocalPos(Vec3(0.f, 30.f, 100.f));
		GetTransform()->SetLocalScale(Vec3(1200.f, 400.f, 1.f));
		GetAnimatorEX()->ChangeAnimation(L"도시");
		pCamera->GetCamera()->SetScale(0.5f);
		((CCameraScript*)pCamera->GetFirstScript())->SetMaxScale(0.65f);
		break;
	default:
		break;
	}

}

void CMapScript::Awake()
{
	
}

void CMapScript::Update()
{
	if (m_pCurAnim == GetAnimatorEX()->GetCurAnim())
		return;

	m_pCurAnim = GetAnimatorEX()->GetCurAnim();

	if (m_pCurAnim == NULL)
		return;

	CCameraScript* CamScript = (CCameraScript*)(CSceneManager::Manager().GetCurScene()->GetMainCamera()->GetFirstScript());
	if (CamScript == NULL)
		return;

	CamScript->SetMapPos(GetTransform()->GetLocalPos());
	CamScript->SetMapScale(GetTransform()->GetLocalScale());
}

