#include "stdafx.h"
#include "KyoEffect.h"

#include "PlayerScKyo.h"

#include "AnimationEX.h"
#include "AnimatorEX.h"
#include "CharactorScript.h"

CKyoEffect::CKyoEffect()
	: m_fCharge(0.f)
{
}


CKyoEffect::~CKyoEffect()
{
}

void CKyoEffect::Awake()
{
	GetAnimatorEX()->SetCurAnim(NULL);
}

void CKyoEffect::Start()
{
}

void CKyoEffect::Update()
{
	if (GetAnimatorEX()->GetCurAnim() == NULL)
		return;

	CPlayerScKyo* pPlayerScript = ((CPlayerScKyo*)(GetGameObject()->GetParent()->GetFirstScript()));
	m_b1Player =pPlayerScript->is1P();
	PLAYER_KEY tKey=((CPlayerScKyo*)(GetGameObject()->GetParent()->GetFirstScript()))->GetPlayerKey();
	wstring strClipName = GetAnimatorEX()->GetCurAnim()->GetKey();
	bool bClipEnd = GetAnimatorEX()->GetCurAnim()->IsAnimFinish();
	int iCurFrame = GetAnimatorEX()->GetCurAnim()->GetCurIndex();

	if (pPlayerScript->GetOnGround()&&iCurFrame>4 && strClipName == L"100식")
	{
		strClipName = L"";
		GetTransform()->SetLocalPos(Vec3Zero);
		GetAnimatorEX()->SetCurAnim(NULL);
	}

	if (bClipEnd)
	{
		if (strClipName == L"100식" || strClipName == L"108식_3")
		{
			GetTransform()->SetLocalPos(Vec3Zero);
			GetAnimatorEX()->SetCurAnim(NULL);
			m_fCharge = 0.f;
			strClipName = L"";
		}

		else if (strClipName == L"108식_1")
			GetAnimatorEX()->ChangeAnimation(L"108식_2", 3, -1, 0);
	}

	if (strClipName == L"100식")
	{
		if (GetAnimatorEX()->GetCurAnim() != NULL)
		{
			if (GetAnimatorEX()->GetCurAnim()->GetCurIndex() == 10)
			{
				GetTransform()->SetLocalPos(Vec3(0.f, 0.5f, 0.f));
			}
		}
	}

	else if (strClipName == L"182식_1")
	{
		GetTransform()->SetLocalPos(Vec3(0.f, -0.05f, 0.f));

		if ((KEYPUSH(tKey.Z) || KEYPUSH(tKey.X))&&m_fCharge<=2.f)
		{
			m_fCharge += DT;
			if (iCurFrame >= 5)
			{
				GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
			}
			//if (m_fCharge > 2.f)
			//{
			//	GetAnimatorEX()->ChangeAnimation(L"182식_fast", 5, -1, 0);
			//}
		}
		else
		{
			GetAnimatorEX()->SetCurAnim(NULL);
			m_fCharge = 0.f;
		}
	}
	
	else if (strClipName == L"182식_fast")
	{
		if (iCurFrame >= 5)
		{
			if (KEYPUSH(tKey.Z) || KEYPUSH(tKey.X))
			{
				GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
			}
			else
			{
				GetAnimatorEX()->SetCurAnim(NULL);
				m_fCharge = 0.f;
			}
		}
	}

	else if (strClipName == L"108식_2")
	{
		if (iCurFrame	== 3)
		{
			if (KEYPUSH(tKey.Z) || KEYPUSH(tKey.X) && m_fCharge <= 2.f)
			{
				m_fCharge += DT;
				GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
			}
		}
	}
	if (strClipName == L"108식_2" || strClipName == L"108식_1")
	{
		if ((KEYUP(tKey.Z) || KEYUP(tKey.X))||m_fCharge>2.f)
		{
			GetAnimatorEX()->ChangeAnimation(L"108식_3", 5, -1, 0);
			m_fCharge = 0.f;
		}
	}
}

void CKyoEffect::LateUpdate()
{
}

void CKyoEffect::FinalUpdate()
{
}
