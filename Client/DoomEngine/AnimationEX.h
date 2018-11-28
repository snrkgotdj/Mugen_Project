#pragma once

#include "global.h"

#include "ResPtr.h"

class CTexture;

struct ColInfo
{
	Vec3 vPos;
	Vec3 vSize;
};

struct tFrameInfo
{
	float fTerm;
	CResPtr<CTexture> pTex;
	Vec2 vLeftTopUV;
	float fWidth;
	float fHeight;
	ColInfo tUpCol;
	ColInfo tDownCol;
	ColInfo tHitCol;
	tPOINT tMove;
	float fSpeed;
	float fJumpSpeed;
};

struct tDescInfo
{
	wstring strTexName;
	wstring strAnimName;
	Vec2 vLeftTop;
	Vec2 vSize;
	int iFrameCount;
	int iContinue;
};

class CAnimatorEX;

class CAnimationEX
{
private:
	CAnimatorEX* m_pAnimator;
	wstring m_strKey;
	vector<tFrameInfo> m_vecFrame;

	UINT m_iCurIdx;
	UINT m_iStartIndx;
	int m_iEndIndex;
	float m_fAccTime;
	bool m_bPlay;
	bool m_bIsAnimFinish;

public:
	CAnimatorEX* GetAnimatorEx() { return m_pAnimator; }
	int GetCurIndex() { return m_iCurIdx; }
	bool IsAnimFinish() { return m_bIsAnimFinish; }
	void SetAnimFinish(bool _bool) { m_bIsAnimFinish = _bool; }

public:
	vector<tFrameInfo>& GetVecFrame() { return m_vecFrame; }
	void ChangeAllClipTerm(float _fTerm);

	void ChangeAllClipUpColInfo(Vec3 _pos, Vec3 _size);
	void ChangeAllClipDownColInfo(Vec3 _pos, Vec3 _size);
	void ChangeAllClipHitColInfo(Vec3 _pos, Vec3 _size);

	void ChangeAllClipUpColPos(float _data, bool _isX);
	void ChangeAllClipDownColPos(float _data, bool _isX);
	void ChangeAllClipHitColPos(float _data, bool _isX);

	void ChangeAllClipUpColSize(float _data, bool _isX);
	void ChangeAllClipDownColSize(float _data, bool _isX);
	void ChangeAllClipHitColSize(float _data, bool _isX);

private:
	void AddFrame(const tFrameInfo& _tFrame) { m_vecFrame.push_back(_tFrame); }

public:
	void RemoveEnd() { m_iStartIndx = m_vecFrame.size() - 1; }
	bool IsPlay() { return m_bPlay; }
	void SetPlay(bool _bOn);
	void SetAnimator(CAnimatorEX* _pAnimator) { m_pAnimator = _pAnimator; }
	void SetFrameIndex(UINT _iCurFrameIdx) { m_iCurIdx = _iCurFrameIdx; }
	void SetStartIndex(UINT _iStartFrameIdx) { m_iStartIndx = _iStartFrameIdx; }
	void SetEndIndex(int _iEndFrameIdx) { m_iEndIndex = _iEndFrameIdx; }
	void SetKey(const wstring& _strKey) { m_strKey = _strKey; }
	const wstring& GetKey() { return m_strKey; }

	int GetCurFrameCnt() { return m_iCurIdx; }
	int GetMaxFrameCnt() { return m_vecFrame.size(); }

public:
	void LateUpdate();
	const tFrameInfo* GetCurFrameInfo() { return &m_vecFrame[m_iCurIdx]; }

public:
	static CAnimationEX* FromDescToAnim(const tDescInfo* _pDesc, const wstring& _strFolderPath);
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);
	CLONE(CAnimationEX);

public:
	CAnimationEX();
	~CAnimationEX();
};

