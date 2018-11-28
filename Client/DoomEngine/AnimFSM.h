#pragma once
#include "Component.h"

class CAnimation;

typedef struct _tagAnimPair
{
	wstring strKey;
	CAnimation* pAnim;
}tAnimPair;

class CAnimFSM :
	public CComponent
{
public:
	CAnimFSM();
	CAnimFSM(const CAnimFSM& _fsm);
	virtual ~CAnimFSM();

private:
	map<wstring, CAnimation*> m_mapAnimClip;
	CAnimation* m_pCurAnimClip;

private:
	bool m_bEnable;

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate() {}
	virtual void Render();
	CLONE(CAnimFSM);

public:
	void AddAnimClip(const wstring& _strKey, CAnimation* _pClip);
	void AddAnimClip(CAnimation* _pClip);
	CGameObject* GetGameObject() { return CComponent::GetGameObject(); }
	CAnimation* GetCurAnimClip() { return m_pCurAnimClip; }
	wstring GetCurAnimClipName();
	void GetAnimCilps(vector<tAnimPair>& _pVecOut);

	void SetAnim(const wstring& _strClipName);
	void SetAnim(CAnimation* _pClip) { m_pCurAnimClip = _pClip; }

	void SetEnable(bool _bEnable) { m_bEnable = _bEnable; }

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
};
