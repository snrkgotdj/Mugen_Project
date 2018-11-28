#include "Sound.h"

FMOD::System* CSound::g_pSystem = NULL;

CSound::CSound()
	:m_pSound(NULL)
	, m_pChannel(NULL)
	, m_bRepeat(false)
	, m_bStop(false)
{
}

CSound::~CSound()
{
	if (NULL != m_pSound)
		m_pSound->release();
}

CSound * CSound::Create(wstring & _strFullPath)
{
	string strFullPath(_strFullPath.begin(), _strFullPath.end());

	FMOD::Sound* pSound = NULL;
	g_pSystem->createSound(strFullPath.c_str(), FMOD_DEFAULT, NULL, &pSound);

	if (NULL == pSound)
	{
		assert(NULL);
	}

	CSound* p = new CSound;
	p->m_pSound = pSound;

	return p;
}

void CSound::Play(int _iRepeatCount)
{
	if (nullptr == this)
		return;

	if (NULL == m_pSound)
		assert(NULL);

	FMOD::Channel* pChanel = NULL;
	g_pSystem->playSound(m_pSound, NULL, false, &pChanel);
	m_pChannel = (FMOD_CHANNEL*)pChanel;

	//if (m_bStop)
	//{
	//	pChanel->setMute(true);
	//	pChanel->setPaused(true);
	//	pChanel->setVolume(0.f);
	//	return;
	//}

	if (_iRepeatCount > 0)
	{
		_iRepeatCount -= 1;
		m_bRepeat = false;
	}

	else
	{
		_iRepeatCount = -1;
		m_bRepeat = true;
	}

	pChanel->setMode(FMOD_LOOP_NORMAL);
	pChanel->setLoopCount(_iRepeatCount);
}

void CSound::Stop(bool _bTrue)
{
	if (this == nullptr)
		return;

	FMOD_Channel_SetPaused(m_pChannel, _bTrue);
}

void CSound::SetVolume(float _fVol)
{
	if (this == nullptr)
		return;

	FMOD_Channel_SetVolume(m_pChannel, _fVol);
}

bool CSound::GetIsPlaying()
{
	if (this == nullptr)
		return false;

	FMOD_Channel_IsPlaying(m_pChannel, &m_bIsPlaying);
	return (bool)m_bIsPlaying;
}
