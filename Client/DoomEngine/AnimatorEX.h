#pragma once
#include "Component.h"

class CAnimationEX;
struct tDescInfo;
struct tFrameInfo;

struct tAnimPairEX
{
	wstring strKey;
	CAnimationEX* pAnim;
};

class CAnimatorEX :
	public CComponent
{
private:
	map<wstring, CAnimationEX*> m_mapAnim;
	CAnimationEX* m_pCurAnim;
	bool m_bRepeat;
	bool m_bPause;

public:
	virtual void Update();
	virtual void LateUpdate();

public:
	void UpdateData();
	static void Disable();

public:
	void SetPause(bool _bPause) { m_bPause = _bPause; }
	bool IsPause() { return m_bPause; }
	bool IsRepeat() { return m_bRepeat; }
	bool IsAnimFinish();

	void LoadAnimation(const tDescInfo* _pDescInfo, const wstring& _strFolderPath);

	void ChangeAnimation(const wstring& _strKey, UINT _iStartFrameIdx = 0, int _iEndFrameIdx = -1, int _CurIndex = -1);
	const tFrameInfo* GetCurFrameInfo();
	CAnimationEX* GetCurAnim() { return m_pCurAnim; }
	void GetAnimations(vector<tAnimPairEX>& _vecOut);
	void SetCurAnimFirst() { m_pCurAnim = m_mapAnim.begin()->second; }
	void SetCurAnim(CAnimationEX* _Anim) { m_pCurAnim = _Anim; }
	void SetRepeat(bool _Repeat) { m_bRepeat = _Repeat; }
	void DeleteClip(const wstring& _strKey);

private:
	CAnimationEX* FindAnimation(const wstring& _strKey);

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	CLONE(CAnimatorEX);

public:
	CAnimatorEX();
	CAnimatorEX(const CAnimatorEX& _other);
	virtual ~CAnimatorEX();
};

