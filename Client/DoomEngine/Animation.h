#pragma once
#include "global.h"

class CTexture;
class CMeshRender;
class CAnimator;


class CAnimation
{
private:
	CAnimator* m_pAnimator;

private:
	wstring m_strClipName;
	wstring m_strNextClipName;
	vector<tAnimFrame> m_vecFrame;
	float m_fTime;
	UINT m_iCurFrame;
	bool m_bRepeat;

	wstring m_strFileName;
	wstring m_strTexKey;

public:
	CAnimation();
	CAnimation(const wstring& _strClipName, bool _bRepeat, const wstring& _strNextClip);
	CAnimation(const CAnimation& _pAnim);
	~CAnimation();

public:
	virtual void Awake();
	virtual void Start();
	virtual int Update();
	virtual int LateUpdate() { return 0; }
	virtual void Render();

public:
	void AddFrame(tAnimFrame _tFrame);
	void AddFrame(wstring _strFileName, float _fTime, int _iIdx, Vec3 _vPosModify = Vec3Zero);
	void AddFrame(const wstring& _strFileName, int _iStart, int _iEnd, float _fTime, Vec3 _vPosModify = Vec3Zero);
	void AddFrameByFile(const wstring& _strFileName, const wstring& _strTexKey);
	void SaveFrameFile(const wstring& _strFileName, const wstring& _strTexKey);

	void SetAnimator(CAnimator* _pCom) { m_pAnimator = _pCom; }
	int GetCurFrame() { return m_iCurFrame; }
	int GetMaxFrame() { return m_vecFrame.size() - 1; }
	void SetCurFrame(int _iFrame) { m_iCurFrame = _iFrame; }
	void SetFrame(UINT _idx, tAnimFrame _frame);

	wstring GetClipName() { return m_strClipName; }
	void SetClipName(const wstring& _strClipName) { m_strClipName = _strClipName; }

	wstring GetNextClipName() { return m_strNextClipName; }
	void SetNextClipName(const wstring& _strClipName) { m_strNextClipName = _strClipName; }

	vector<tAnimFrame>& GetVecFrame() { return m_vecFrame; }
	void SetVecFrame(vector<tAnimFrame>& _vecFrame);

	bool IsClipEnd();
	bool GetClipRepeat() { return m_bRepeat; }

	CAnimation* Clone() { return new CAnimation(*this); }

	wstring GetFileName() { return m_strFileName; }
	wstring GetTexKey() { return m_strTexKey; }
};
