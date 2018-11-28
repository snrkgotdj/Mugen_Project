#pragma once
#include "Single\SingletonScript.h"
class CLayerManager :
	public CSingletonScript<CLayerManager>
{
public:
	virtual void Awake();
	virtual void Start();
	virtual void Update();

public:
	CLONE(CLayerManager);

public:
	CLayerManager();
	virtual ~CLayerManager();
};

