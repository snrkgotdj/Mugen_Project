#pragma once
#include "Component.h"

class CTransform :
	public CComponent
{
private:
	Vec3	m_vtxPos;
	Vec3	m_vtxScale;
	Vec3	m_vtxRot;

	Matrix	m_matWorld;
	Matrix  m_matFinalWorld;

public:
	virtual void Awake() {}
	virtual void Start() {}
	virtual void Update() {}
	virtual void LateUpdate() {}
	virtual void FinalUpdate();

public:
	void UpdateData();

public:
	const Vec3& GetLocalPos() { return m_vtxPos; }
	void SetLocalPos(const Vec3& _vtxPos) { m_vtxPos = _vtxPos; }

	Vec3 GetWorldPos();

	const Vec3& GetLocalScale() { return m_vtxScale; }
	void SetLocalScale(const Vec3&	_vtxScale) { m_vtxScale = _vtxScale; }
	const Vec3& GetLocalRot() { return m_vtxRot; }
	void SetLocalRot(const Vec3& _vtxRot) { m_vtxRot = _vtxRot; }
	Vec3& GetWorldScale();

	const Matrix& GetWorldMat() { return m_matWorld; }
	const Matrix& GetFinalWorldMat() { return m_matFinalWorld; }


public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);
	virtual CTransform* Clone() { return new CTransform(*this); }

public:
	CTransform();
	CTransform(Vec3 _vecPos, Vec3 _vecScale, Vec3 _vecRot);
	virtual ~CTransform();
};

