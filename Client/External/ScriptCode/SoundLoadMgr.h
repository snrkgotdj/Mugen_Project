#pragma once
#include "Single\SingletonScript.h"

class CSoundLoadMgr :
	public CSingletonScript<CSoundLoadMgr>
{
public:
	CSoundLoadMgr();
	virtual ~CSoundLoadMgr();

public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();

public:
	void LoadSoundFolder(const wstring& _strFolder);

	CLONE(CSoundLoadMgr);
};

