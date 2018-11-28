#include "stdafx.h"
#include "EffectManager.h"
#include "SceneManager.h"
#include "SaveLoadManager.h"
#include "PathManager.h"
#include "ResManager.h"

#include "AnimatorEX.h"
#include "AnimationEX.h"

#include "EffectScript.h"
#include "Layer.h"
#include "Scene.h"
#include "Script.h"

CEffectManager::CEffectManager()
	: m_iCallCnt(0)
{
}

CEffectManager::~CEffectManager()
{
}

void CEffectManager::ActiveEffect(const wstring& _strEffect, const Vec3& _vecPos, bool _bContinue)
{
	return;
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	CGameObject* pEffectObj = NULL;
	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		if (false == ((CEffectScript*)(vecEffect[i]->GetFirstScript()))->isActive())
		{
			((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(true);
			pEffectObj = vecEffect[i];
			pEffectObj->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
			break;
		}
	}
	if (pEffectObj == NULL)
	{
		CGameObject* pClone = vecEffect[0]->Clone();
		((CEffectScript*)(pClone->GetFirstScript()))->SetActive(true);

		pClone->GetAnimatorEX()->SetCurAnimFirst();
		pClone->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
		pClone->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);

		if(pClone->HasParent())
		pClone->EndRelationship();
		CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect")->AddGameObject(pClone);

		vecEffect.push_back(pClone);
		pEffectObj = pClone;
	}
	if(true == _bContinue)
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(true);
	else
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(false);

	pEffectObj->GetAnimatorEX()->SetPause(false);
	pEffectObj->GetTransform()->SetLocalPos(_vecPos);*/
}

void CEffectManager::ActiveEffect(const wstring & _strEffect, const wstring & _strLayer, const Vec3 & _vecPos, bool _bContinue)
{
	return;
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	CGameObject* pEffectObj = NULL;
	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		if (false == ((CEffectScript*)(vecEffect[i]->GetFirstScript()))->isActive())
		{
			((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(true);
			pEffectObj = vecEffect[i];
			pEffectObj->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
			break;
		}
	}
	if (pEffectObj == NULL&&!vecEffect.empty())
	{
		CGameObject* pClone = vecEffect[0]->Clone();
		((CEffectScript*)(pClone->GetFirstScript()))->SetActive(true);

		pClone->GetAnimatorEX()->SetCurAnimFirst();
		pClone->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
		pClone->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);

		if (pClone->HasParent())
			pClone->EndRelationship();
		CSceneManager::Manager().GetCurScene()->FindLayer(_strLayer)->AddGameObject(pClone);

		vecEffect.push_back(pClone);
		pEffectObj = pClone;
	}
	else
	{
		m_iCallCnt++;
	}

	if (pEffectObj == NULL)
		return;

	if (m_iCallCnt >= 1)
	{
		CSceneManager::Manager().GetCurScene()->FindLayer(_strLayer)->AddGameObject(pEffectObj);
		wstring strCloneTag = pEffectObj->GetTagName()+L"Clone";
		pEffectObj->SetTagName(strCloneTag);
	}

	if (true == _bContinue)
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(true);
	else
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(false);

	pEffectObj->SetLayerName(_strLayer);
	pEffectObj->GetAnimatorEX()->SetPause(false);
	pEffectObj->GetTransform()->SetLocalPos(_vecPos);*/
}

void CEffectManager::ActiveEffect(const wstring & _strEffect, const Vec3 & _vecPos, const Vec3 & _vScale, bool _bContinue)
{
	return;
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	if (GetGameObject()->isDead())
		return;

	CGameObject* pEffectObj = NULL;
	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		if (false == ((CEffectScript*)(vecEffect[i]->GetFirstScript()))->isActive())
		{
			((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(true);
			pEffectObj = vecEffect[i];
			pEffectObj->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
			break;
		}
	}
	if (pEffectObj == NULL)
	{
		CGameObject* pClone = vecEffect[0]->Clone();
		((CEffectScript*)(pClone->GetFirstScript()))->SetActive(true);

		pClone->GetAnimatorEX()->SetCurAnimFirst();
		pClone->GetAnimatorEX()->GetCurAnim()->RemoveEnd();
		pClone->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);

		if (pClone->HasParent())
			pClone->EndRelationship();
		CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect")->AddGameObject(pClone);

		vecEffect.push_back(pClone);
		pEffectObj = pClone;
	}
	if (true == _bContinue)
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(true);
	else
		((CEffectScript*)(pEffectObj->GetFirstScript()))->SetContinue(false);

	pEffectObj->GetAnimatorEX()->SetPause(false);
	pEffectObj->GetTransform()->SetLocalPos(_vecPos);

	pEffectObj->GetTransform()->SetLocalScale(_vScale);*/
}

vector<CGameObject*>& CEffectManager::FindEffect(const wstring& _strEffect)
{
	map<wstring, vector<CGameObject*>>::iterator iter = m_mapEffect.find(_strEffect);

	if (iter == m_mapEffect.end())
	{
		vector<CGameObject*> vecEmpty;
		return vecEmpty;
	}

	return iter->second;
}

void CEffectManager::Awake()
{
	//CEffectManager::GetInst()->LoadEffect(L"00Common");
}

void CEffectManager::Update()
{
}

void CEffectManager::LoadEffect(const wstring & _strEffectFolder)
{
	/*if (NULL == CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect"))
		return;

	wstring strFolderPath = {};
	strFolderPath = CPathManager::GetResPathEX();
	strFolderPath += L"Save\\Effect\\" + _strEffectFolder;

	WIN32_FIND_DATA tData = {};
	HANDLE hHandle = FindFirstFile(wstring(strFolderPath + L"\\*.*").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == hHandle)
	{
		MessageBox(NULL, L"존재하지 않는 폴더입니다", L"이펙트 로드 실패", MB_OK);
		assert(NULL);
	}

	CGameObject* pEffect;
	vector<CGameObject*> vecEffect;
	wstring strExt;
	while (true)
	{
		strExt = CPathManager::GetExt(tData.cFileName);
		if (L".goc" == strExt)
		{
			vecEffect.clear();
			pEffect = CSaveLoadManager::Manager().LoadGameObjectOnly(wstring(strFolderPath +L"\\"+ tData.cFileName));
			pEffect->GetTransform()->SetLocalPos(Vec3(0, 0, 3000));
			pEffect->GetAnimatorEX()->SetPause(true);
			pEffect->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
			pEffect->EndRelationship();
			CSceneManager::Manager().GetCurScene()->FindLayer(L"Effect")->AddGameObject(pEffect);
			vecEffect.push_back(pEffect);
			m_mapEffect.insert(make_pair(pEffect->GetTagName(), vecEffect));
		}

		if (0 == FindNextFile(hHandle, &tData))
			break;
	}
	FindClose(hHandle);*/
}

void CEffectManager::StopEffect(const wstring & _strEffect)
{
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		if (true == ((CEffectScript*)(vecEffect[i]->GetFirstScript()))->isActive())
		{
			((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(false);
			vecEffect[i]->GetTransform()->SetLocalPos(Vec3(0.f, 0.f, 3000.f));
			vecEffect[i]->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(0);
			vecEffect[i]->GetAnimatorEX()->GetCurAnim()->SetAnimFinish(false);
			vecEffect[i]->GetAnimatorEX()->SetPause(true);
			break;
		}
	}*/
}

void CEffectManager::FollowPos(const wstring & _strEffect, const Vec3 & _vecPos, int _Dir)
{
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	if (vecEffect.empty())
		return;

	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(true);
		vecEffect[i]->GetAnimatorEX()->SetPause(false);
		vecEffect[i]->GetTransform()->SetLocalPos(_vecPos);
		
		Vec3 vecScale = vecEffect[i]->GetTransform()->GetLocalScale();
		if (_Dir)
		{
			if (vecScale.x < 0)
			{
				vecEffect[i]->GetTransform()->SetLocalScale(Vec3(-vecScale.x, vecScale.y, vecScale.z));
			}
		}
		
		else if (_Dir == 0)
		{
			if (vecScale.x > 0)
			{
				vecEffect[i]->GetTransform()->SetLocalScale(Vec3(-vecScale.x, vecScale.y, vecScale.z));
			}
		}
	}*/
}

void CEffectManager::FollowPos(const wstring & _strEffect, const Vec3 & _vecPos, const Vec3 & _vScale, int _Dir)
{
	/*vector<CGameObject*>& vecEffect = FindEffect(_strEffect);

	for (UINT i = 0; i < vecEffect.size(); ++i)
	{
		((CEffectScript*)(vecEffect[i]->GetFirstScript()))->SetActive(true);
		vecEffect[i]->GetAnimatorEX()->SetPause(false);
		vecEffect[i]->GetTransform()->SetLocalPos(_vecPos);

		Vec3 vecScale = vecEffect[i]->GetTransform()->GetLocalScale();
		if (_Dir)
		{
			if (vecScale.x < 0)
			{
				vecEffect[i]->GetTransform()->SetLocalScale(_vScale);
			}
		}

		else if (_Dir == 0)
		{
			if (vecScale.x > 0)
			{
				vecEffect[i]->GetTransform()->SetLocalScale(_vScale);
			}
		}
	}*/
}
