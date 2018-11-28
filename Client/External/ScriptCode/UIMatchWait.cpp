#include "stdafx.h"
#include "UIMatchWait.h"


CUIMatchWait::CUIMatchWait()
	: m_fTerm(0.f)
{
}


CUIMatchWait::~CUIMatchWait()
{
}

void CUIMatchWait::Update()
{
	Vec3 vRot = GetTransform()->GetLocalRot();
	vRot.z -= (XM_PI) * DT;
	GetTransform()->SetLocalRot(vRot);
}
