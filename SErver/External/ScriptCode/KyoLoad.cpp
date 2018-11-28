#include "stdafx.h"
#include "KyoLoad.h"

#include "AnimationEX.h"
#include "AnimatorEX.h"

#include "SoundLoadMgr.h"

CKyoLoad::CKyoLoad()
	: m_iRandAct(0)
{
}


CKyoLoad::~CKyoLoad()
{
}

void CKyoLoad::Awake()
{
	CSoundLoadMgr::GetInst()->LoadSoundFolder(L"Kyo");


}

void CKyoLoad::Start()
{

}

void CKyoLoad::Update()
{
	if (GetAnimatorEX() == NULL)
		return;

	wstring strClip = GetAnimatorEX()->GetCurAnim()->GetKey();

	if (strClip == L"427½Ä ¿ªÃ¶ °­"|| strClip == L"427½Ä ¿ªÃ¶ ¾à"|| strClip == L"¿Ü½Ä ¼¶µ¹¶Õ±â"
		||strClip==L"125½Ä Ä¥·Ú"||strClip==L"127½Ä ÆÈÃ»"||strClip==L"402½Ä ¹úÀ¼±â"||strClip==L"100½Ä ±Í½ÅÅÂ¿ì±â")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_iRandAct = getRandomNumber_int(0, 10);
		}
	}

	if (strClip == L"114½Ä È²¹°±â")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"128½Ä ±¸»ó");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"127½Ä ÆÈÃ»");
				break;
			case 2:
				GetAnimatorEX()->ChangeAnimation(L"427½Ä ¿ªÃ¶ °­");
				break;
			}
		}
	}

	else if (strClip == L"128½Ä ±¸»ó")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"125½Ä Ä¥·Ú");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"127½Ä ÆÈÃ»");
				break;
			case 2:
				GetAnimatorEX()->ChangeAnimation(L"427½Ä ¿ªÃ¶ ¾à");
				break;
			}
		}
	}

	else if (strClip == L"127½Ä ÆÈÃ»")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"¿Ü½Ä ¼¶µ¹¶Õ±â");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"125½Ä Ä¥·Ú");
				break;

			}
		}
	}

	else if (strClip == L"115½Ä µ¶¹°±â")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"401½Ä ÁËÀ¼±â");
		}
	}

	else if (strClip == L"401½Ä ÁËÀ¼±â")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"402½Ä ¹úÀ¼±â");
		}
	}

	if (strClip == L"Idle")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			m_iRandAct = getRandomNumber_int(0, 10);
			switch (m_iRandAct)
			{
			case 0: case 2: case 4: case 6: case 8:case 1: case 3: case 5:
				GetAnimatorEX()->ChangeAnimation(L"114½Ä È²¹°±â");
				break;
			case 7: case 9:
				GetAnimatorEX()->ChangeAnimation(L"115½Ä µ¶¹°±â");
				break;
			
			}
		}
	}
}

void CKyoLoad::LateUpdate()
{
}

void CKyoLoad::FinalUpdate()
{
}
