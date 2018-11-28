#include "TimeManager.h"
#include "FontMgr.h"

void CTimeManager::init()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&CurCnt);
	OldCnt = CurCnt;


	m_fFPSTime = 0.f;
	m_fFrameLimit = 60;
	m_fAccTime = 0;
}

void CTimeManager::Update()
{
	QueryPerformanceCounter(&CurCnt);
	DeltaTime = (CurCnt.QuadPart - OldCnt.QuadPart) / (float)frequency.QuadPart;
	OldCnt = CurCnt;

	Renewfrequency += DeltaTime;
	if (Renewfrequency > 1.f)
	{
		Renewfrequency = 0.f;
		QueryPerformanceFrequency(&frequency);
	}
	
	m_fFPSTime = 1 / DeltaTime;

	if (OldDeltaTime != 0)
	{
		if (DeltaTime > OldDeltaTime * 10)
		{
			DeltaTime = 0.f;
		}
	}

	OldDeltaTime = DeltaTime;

	if (m_bStop == true)
	{
		m_fAccTime += DT;
		if (m_fAccTime >= m_fStopTime)
		{
			m_fAccTime = 0;
			m_bStop = false;
			m_fStopTime = 0;
		}

		else
			DeltaTime = 0;
	}
}

void CTimeManager::Render(HDC _dc)
{
	wchar_t str[128] = {};

	wsprintf(str, L"FPS : %d", (int)m_fFPSTime);
	CFontMgr::Manager().DrawFont(str, 10.f, 10.f, 15.f, ABGR(10, 240, 10, 255));
}

CTimeManager::CTimeManager()
	:m_fFPSTime(0.f)
	, OldDeltaTime(0.f)
	, m_bStop(false)
{
}


CTimeManager::~CTimeManager()
{
}
