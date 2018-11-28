#pragma once
#include "Script.h"
#include "KeyCommand.h"

class CUIBtnOutput :
	public Script
{
private:
	int m_iSize;
	PLAYER_KEY m_tKey;
	bool m_b1Player;
	bool m_bActive;
	float m_fTime;

	CGameObject* m_pTarget;

public:
	CUIBtnOutput();
	virtual ~CUIBtnOutput();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	CLONE(CUIBtnOutput);

public:
	CGameObject* FindTarget(const wstring& _strChildTag);
	void SetFrame(const wstring& _strPrev, const wstring& _strNext);
	void SetFrame();
	void SetActive(bool _bTrue) { m_bActive = _bTrue; }
	void Clear();
};

