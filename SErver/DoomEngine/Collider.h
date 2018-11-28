#pragma once
#include "Component.h"
#include "ResPtr.h"

class CMesh;
class CMaterial;

class CCollider :
	public CComponent
{
private:
	static UINT g_iColID;

protected:
	UINT m_iColID;
	Vec3 m_vObjPos;
	Vec3 m_vOffsetPos;
	Vec3 m_vOffsetScale;

	Matrix m_matColWorld;
	int m_iCollision;

	CResPtr<CMesh> m_pMesh;
	CResPtr<CMaterial> m_pMtrl;

public:
	virtual void Update() {}
	virtual void FinalUpdate();
	virtual void Render() {}

public:
	UINT GetID() { return m_iColID; }

	void SetWorldPos(Vec3 _vWorldPos) { m_vObjPos = _vWorldPos; }
	void SetPos(Vec3 _vPos) { m_vOffsetPos = _vPos; }
	void SetScale(Vec3 _vScale) { m_vOffsetScale = _vScale; }

	const Vec3& GetWorldPos() { return m_vObjPos; }
	const Vec3& GetOffsetPos() { return m_vOffsetPos; }
	const Vec3& GetScale() { return m_vOffsetScale; }
	const Matrix& GetWorldMat() { return m_matColWorld; }
	CResPtr<CMaterial>& GetMtrl(){ return m_pMtrl; }

public:
	virtual void OnCollisionEnter(CCollider* _pOther);
	virtual void OnCollision(CCollider* _pOther);
	virtual void OnCollisionExit(CCollider* _pOther);

public:
	virtual bool Is2DCollider() = 0;
	virtual bool Is3DCollider() = 0;

protected:
	void UpdateData();

public:
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CCollider();
	virtual ~CCollider();
};

