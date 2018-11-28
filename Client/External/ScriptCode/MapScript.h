#pragma once
#include "Script.h"
#include "..\Doom\define_client.h"
class CAnimationEX;

class CMapScript :
	public Script
{
private:
	CAnimationEX* m_pCurAnim;

public:
	void init(MAP_STATE _MapType);

public:
	virtual void Awake();
	virtual void Update();

	CLONE(CMapScript);

public:
	CMapScript();
	virtual ~CMapScript();
};

