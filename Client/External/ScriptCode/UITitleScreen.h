#pragma once
#include "Script.h"
class CUITitleScreen :
	public Script
{
private:
	bool m_bEnterPushed;
	int m_iMode;
	CSound* m_pSound;

private:
	void BattleMode();
	void OnlineMode();


public:
	CUITitleScreen();
	virtual ~CUITitleScreen();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUITitleScreen);
};

