#pragma once
#include "global.h"

union Col_ID
{
	struct
	{
		UINT iLeft;
		UINT iRight;
	};
	DLONG ID;
};

class CLayer;
class CCollider;

class CCollisionManager
{
private:
	UINT m_arrMatrix[MAX_LAYER];
	map<DLONG, bool> m_mapColID;

public:
	int CheckLayerCollision(const wstring& _strLayer1, const wstring& _strLayer2);
	
private:
	void CollisionLayer(CLayer* _pLeftLayer, CLayer* _pRightLayer);
	bool IsCollision(CCollider* _pLeftCol, CCollider* _pRightCol);
	void GetProjAxis(const Matrix& _matWorld, Vec3* _vProjOut, Vec3* _vPos);

public:
	int Update();

public:
	static CCollisionManager& Manager()
	{
		static CCollisionManager inst;
		return inst;
	}

private:
	CCollisionManager();
	~CCollisionManager();
};

