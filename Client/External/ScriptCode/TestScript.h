#pragma once
#include "Script.h"
#include "..\Doom\define_client.h"
class CTestScript :
	public Script
{


public:
	virtual void Update();

	CLONE(CTestScript);

public:
	CTestScript();
	virtual ~CTestScript();
};

