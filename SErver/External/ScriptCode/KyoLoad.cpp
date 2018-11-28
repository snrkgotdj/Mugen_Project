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

	if (strClip == L"427�� ��ö ��"|| strClip == L"427�� ��ö ��"|| strClip == L"�ܽ� �����ձ�"
		||strClip==L"125�� ĥ��"||strClip==L"127�� ��û"||strClip==L"402�� ������"||strClip==L"100�� �ͽ��¿��")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"Idle");
			m_iRandAct = getRandomNumber_int(0, 10);
		}
	}

	if (strClip == L"114�� Ȳ����")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"128�� ����");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"127�� ��û");
				break;
			case 2:
				GetAnimatorEX()->ChangeAnimation(L"427�� ��ö ��");
				break;
			}
		}
	}

	else if (strClip == L"128�� ����")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"125�� ĥ��");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"127�� ��û");
				break;
			case 2:
				GetAnimatorEX()->ChangeAnimation(L"427�� ��ö ��");
				break;
			}
		}
	}

	else if (strClip == L"127�� ��û")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			int iRand = getRandomNumber_int(0, 2);

			switch (iRand)
			{
			case 0:
				GetAnimatorEX()->ChangeAnimation(L"�ܽ� �����ձ�");
				break;
			case 1:
				GetAnimatorEX()->ChangeAnimation(L"125�� ĥ��");
				break;

			}
		}
	}

	else if (strClip == L"115�� ������")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"401�� ������");
		}
	}

	else if (strClip == L"401�� ������")
	{
		if (GetAnimatorEX()->IsAnimFinish())
		{
			GetAnimatorEX()->ChangeAnimation(L"402�� ������");
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
				GetAnimatorEX()->ChangeAnimation(L"114�� Ȳ����");
				break;
			case 7: case 9:
				GetAnimatorEX()->ChangeAnimation(L"115�� ������");
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
