#include "CollisionManager.h"

#include "SceneManager.h"
#include "Scene.h"
#include "Layer.h"

#include "GameObject.h"
#include "Collider.h"
#include "TransForm.h"

CCollisionManager::CCollisionManager()
	:m_arrMatrix{}
{
}

CCollisionManager::~CCollisionManager()
{
}

int CCollisionManager::CheckLayerCollision(const wstring & _strLayer1, const wstring & _strLayer2)
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();

	CLayer* pLayer1 = pCurScene->FindLayer(_strLayer1);
	CLayer* pLayer2 = pCurScene->FindLayer(_strLayer2);

	if (NULL == pLayer1 || NULL == pLayer2)
		return RET_FAIL;

	UINT iIdx1 = pLayer1->GetLayerIdx();
	UINT iIdx2 = pLayer2->GetLayerIdx();

	if (iIdx1 > iIdx2)
	{
		UINT iTemp = iIdx1;
		iIdx1 = iIdx2;
		iIdx2 = iTemp;
	}

	m_arrMatrix[iIdx1] |= (1 << iIdx2);

	return RET_SUCCESS;
}

int CCollisionManager::Update()
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	vector<CLayer*>& vecLayer = pCurScene->GetVecLayer();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		for (UINT j = i; j < MAX_LAYER; ++j)
		{
			if ((m_arrMatrix[i] >> j) & 1)
			{
				CollisionLayer(vecLayer[i], vecLayer[j]);
			}
		}
	}

	return 0;
}

void CCollisionManager::CollisionLayer(CLayer * _pLeftLayer, CLayer * _pRightLayer)
{
	list<CGameObject*>& leftList = _pLeftLayer->GetObjList();
	list<CGameObject*>& rightList = _pRightLayer->GetObjList();

	list<CGameObject*>::iterator leftIter = leftList.begin();

	for (; leftIter != leftList.end(); ++leftIter)
	{
		if (NULL == (*leftIter)->GetCollider())
			continue;

		list<CGameObject*>::iterator rightIter = rightList.begin();
		for (rightIter; rightIter != rightList.end(); ++rightIter)
		{
			if (NULL == (*rightIter)->GetCollider())
				continue;

			CCollider* pLeftCol = (*leftIter)->GetCollider();
			CCollider* pRightCol = (*rightIter)->GetCollider();

			if (pLeftCol->Is2DCollider() != pRightCol->Is2DCollider())
				continue;

			Col_ID id;
			id.iLeft = pLeftCol->GetID();
			id.iRight = pRightCol->GetID();

			map<DLONG, bool>::iterator mapIter = m_mapColID.find(id.ID);

			if (IsCollision(pLeftCol, pRightCol))
			{
				if (mapIter == m_mapColID.end())
				{
					m_mapColID.insert(make_pair(id.ID, true));
					pLeftCol->OnCollisionEnter(pRightCol);
					pRightCol->OnCollisionEnter(pLeftCol);
				}

				else
				{
					if (false == mapIter->second)
					{
						pLeftCol->OnCollisionEnter(pRightCol);
						pRightCol->OnCollisionEnter(pLeftCol);
						mapIter->second = true;
					}
					else
					{
						pLeftCol->OnCollision(pRightCol);
						pRightCol->OnCollision(pLeftCol);
					}
				}
			}
			else
			{
				if (mapIter != m_mapColID.end() && true == mapIter->second)
				{
					pLeftCol->OnCollisionExit(pRightCol);
					pRightCol->OnCollisionExit(pLeftCol);
					mapIter->second = false;
				}
			}
		}
	}
}

bool CCollisionManager::IsCollision(CCollider * _pLeftCol, CCollider * _pRightCol)
{
	if (0 == _pLeftCol->GetTransform()->GetLocalScale().x || 0 == _pLeftCol->GetTransform()->GetLocalScale().y
		|| 0 == _pRightCol->GetTransform()->GetLocalScale().x || 0 == _pRightCol->GetTransform()->GetLocalScale().y)
		return false;


	Vec3 vProj[4] = {}; 
	Vec3 vOut[4] = {};
	Vec3 vCenter = _pRightCol->GetWorldPos() - _pLeftCol->GetWorldPos();
	vCenter.z = 0;

	GetProjAxis(_pLeftCol->GetWorldMat(), vProj, vOut);
	GetProjAxis(_pRightCol->GetWorldMat(), vProj + 2, vOut + 2);

	for (int i = 0; i < 4; ++i)
	{
		float fDist = 0.f;
		for (int j = 0; j < 4; ++j)
		{
			fDist += abs(vOut[j].Dot(vProj[i]));
		}
		fDist /= 2.f;

		float fCenter = abs(vCenter.Dot(vProj[i]));

		if (fCenter > fDist)
			return false;
	}
	return true;
}

void CCollisionManager::GetProjAxis(const Matrix & _matWorld, Vec3 * _vProjOut, Vec3 * _vPos)
{
	Vec3 vLocal[4] = { Vec3(-0.5f, 0.5f, 0.f) , Vec3(0.5f, 0.5f, 0.f), Vec3(0.5f, -0.5f, 0.f) , Vec3(-0.5f, -0.5f, 0.f) };

	Vec3 vWorldPos[3] = {};

	vWorldPos[0] = XMVector3TransformCoord(vLocal[0].Convert(), _matWorld);
	vWorldPos[1] = XMVector3TransformCoord(vLocal[1].Convert(), _matWorld);
	vWorldPos[2] = XMVector3TransformCoord(vLocal[3].Convert(), _matWorld);

	_vPos[0] = vWorldPos[1] - vWorldPos[0];
	_vPos[0].z = 0;

	_vPos[1] = vWorldPos[2] - vWorldPos[0];
	_vPos[1].z = 0;
	
	_vProjOut[0] = _vPos[0];
	_vProjOut[0].Normalize();

	_vProjOut[1] = _vPos[1];
	_vProjOut[1].Normalize();
}
