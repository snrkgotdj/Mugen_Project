#pragma once
#include "Component.h"

class CLayer;

class CCamera :
	public CComponent
{
public:
	Matrix m_matView;
	Matrix m_matProj;

	bool m_bPerspective;

	float m_fFOV;

	float m_fWidth;
	float m_fHeight;
	float m_fScale;

	float m_fNear;
	float m_fFar;

	UINT m_iLayerMask;

public:
	void SetNear(float _near) { m_fNear = _near; }
	void SetFar(float _far) { m_fFar = _far; }

	float GetNear() const { return m_fNear; }
	float GetFar() const { return m_fFar; }
	float GetScale() const { return m_fScale; }

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update();
	virtual void LateUpdate() {}
	virtual void FinalUpdate();
	void Render(CLayer* _pLayer);

public:
	void CheckRenderLayer(const wstring& _strLayerName);
	void CheckRenderLayerAll() { m_iLayerMask = 0xffffffff; }
	void SetScale(float _fScale) { m_fScale = _fScale; }
	float GetScale() { return m_fScale; }

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	CCamera* Clone() { return new CCamera(*this); }

public:
	CCamera();
	virtual ~CCamera();
};
