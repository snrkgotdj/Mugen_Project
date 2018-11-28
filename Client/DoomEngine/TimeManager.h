#pragma once
#include "global.h"

class CTimeManager
{
private:
	LARGE_INTEGER frequency;
	LARGE_INTEGER OldCnt;
	LARGE_INTEGER CurCnt;

	int m_iFrame;

	float DeltaTime;
	float OldDeltaTime;
	float Renewfrequency;
	float m_fFPSTime;
	float m_fFrameLimit;

	bool m_bStop;
	float m_fStopTime;
	float m_fAccTime;

public:
	float GetFrameTime() { return DeltaTime; }
	float GetDeltaTime() { return 1.f / 60.f; }	
	LARGE_INTEGER GetFrequency() { return frequency; }
	void SetStop(bool _true, float _stopTime = 0.1f) {	m_bStop = _true; m_fStopTime = _stopTime; }

public:
	void init();
	void Update();
	void Render(HDC _dc);

public:
	static CTimeManager& Manager()
	{
		static CTimeManager inst;
		return inst;
	}
public:
	CTimeManager();
	~CTimeManager();
};

