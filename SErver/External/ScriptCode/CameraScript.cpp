#include "stdafx.h"
#include "CameraScript.h"
#include "SceneManager.h"

CCameraScript::CCameraScript()
	:m_StartChangeScale(400)
	, m_LimitCamScale(0.6f)
	, m_iMaxScale(1.f)
	, m_fVibTime(0.f)
	, m_fVibTimeLimit(-1.f)
	, m_fVibRange(0.f)
{
}


CCameraScript::~CCameraScript()
{
}

void CCameraScript::Vibration(float _fRange, float _fTime)
{
	Vec3 vCamPos = GetTransform()->GetLocalPos();

	m_fVibTimeLimit = _fTime;


}

void CCameraScript::Awake()
{
	
}

void CCameraScript::Start()
{
}

void CCameraScript::Update()
{
	Vec3 vec1P = m_pGameObject_1P->GetTransform()->GetLocalPos();
	Vec3 vec2P = m_pGameObject_2P->GetTransform()->GetLocalPos();
	Vec3 vecCam = GetTransform()->GetLocalPos();

	float fScale = 0;
	if (sqrt(pow((vec1P.x - vec2P.x), 2) + pow((vec1P.y - vec2P.y), 2)) <= m_StartChangeScale)
	{
		fScale = m_LimitCamScale;
	}
	else if (sqrt(pow((vec1P.x - vec2P.x), 2) + pow((vec1P.y - vec2P.y), 2)) >= m_StartChangeScale)
	{
		fScale = sqrt(pow((vec1P.x - vec2P.x), 2) + pow((vec1P.y - vec2P.y), 2)) / (m_StartChangeScale/ m_LimitCamScale);
	}

	if (fScale >= m_iMaxScale)
	{
		fScale = m_iMaxScale;
	}

	//==============
	//			Áøµ¿
	//==============
	if (m_fVibTimeLimit>0.f)
	{
		Vibration(m_fVibRange, m_fVibTimeLimit);
	}
	else
	{
		m_fVibTime = 0.f;
		m_fVibTimeLimit = -1.f;
	}


	vecCam.x = (vec1P.x + vec2P.x) / 2;
	if (vecCam.x - WindowSizeX*fScale *0.5f <= -(m_vecMapScale.x / 2))
		vecCam.x = WindowSizeX * fScale * 0.5f - (m_vecMapScale.x / 2);

	if (vecCam.x + WindowSizeX*fScale *0.5f >= (m_vecMapScale.x / 2.f))
		vecCam.x = (m_vecMapScale.x / 2.f) - WindowSizeX * fScale * 0.5f;

	vecCam.y = (-(m_vecMapScale.y/2.f) + m_vecMapPos.y) + (WindowSizeY * fScale * 0.5f);

	GetCamera()->SetScale(fScale);
	GetTransform()->SetLocalPos(vecCam);
}

void CCameraScript::LateUpdate()
{
}

