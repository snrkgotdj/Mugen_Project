#pragma once
#include "Res.h"

#include "FMOD\fmod.h"
#include "FMOD\fmod.hpp"
#include "FMOD\fmod_codec.h"

#ifdef _DEBUG
#pragma comment(lib, "fmod_vc.lib")
#elif NDEBUG
#pragma comment(lib, "fmodL_vc.lib")
#endif

class CSound :
	public CRes
{
public:
	static FMOD::System* g_pSystem;

private:
	FMOD::Sound* m_pSound;
	bool m_bRepeat;
	bool m_bStop;
	int m_bIsPlaying;
	FMOD_CHANNEL* m_pChannel;

public:
	static CSound* Create(wstring& _strFullPath);
	void Play(int _iRepeatCount = -1);
	void Stop(bool _bTrue);
	void SetVolume(float _fVol);
	bool GetIsPlaying();

public:
	CSound();
	virtual ~CSound();
};

