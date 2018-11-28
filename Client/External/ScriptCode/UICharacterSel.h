#pragma once
#include "Script.h"

class CUICharacterSel :
	public Script
{
private:
	wstring m_strName;
	bool m_bIs1P;
	bool m_bIsFinish;
	UINT m_iNum;
	bool m_bFirstCall;
	char m_cPlayer;

	UINT m_iPlayMode;
	UINT m_iTrainMode;

	CSound* m_pSound;

private:
	void Mode_Server();

public:
	void Set1P(bool _is1P) { m_bIs1P = _is1P; }
	void SetPlayer(char _cPlayer);

public:
	CUICharacterSel();
	virtual ~CUICharacterSel();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUICharacterSel);

public:
	void SetPlayMode(UINT _iMode) { m_iPlayMode = _iMode; }
	void SetTrainMode(UINT _iMode) { m_iTrainMode = _iMode; }
};

