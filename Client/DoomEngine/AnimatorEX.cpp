#include "AnimatorEX.h"

#include "ResManager.h"
#include "PathManager.h"

#include "AnimationEX.h"
#include "Device.h"

#include "Shlwapi.h"
#pragma comment(lib, "Shlwapi.lib")

CAnimatorEX::CAnimatorEX()
	:m_pCurAnim(NULL)
	, m_bRepeat(true)
	, m_bPause(false)
{
}

CAnimatorEX::CAnimatorEX(const CAnimatorEX & _other)
	: CComponent(_other)
	, m_pCurAnim(NULL)
	, m_bRepeat(_other.m_bRepeat)
{
	map<wstring, CAnimationEX*>::const_iterator iter = _other.m_mapAnim.begin();
	for (; iter != _other.m_mapAnim.end(); ++iter)
	{
		CAnimationEX* pAnim = iter->second->Clone();
		pAnim->SetAnimator(this);
		m_mapAnim.insert(make_pair(iter->first, pAnim));
	}
}

CAnimatorEX::~CAnimatorEX()
{
	Safe_Delete_Map(m_mapAnim);
}

void CAnimatorEX::Update()
{
}

void CAnimatorEX::LateUpdate()
{
	if (NULL != m_pCurAnim && m_bPause == false)
	{
		m_pCurAnim->LateUpdate();
		if (false == m_pCurAnim->IsPlay() && m_bRepeat)
		{
			m_pCurAnim->SetPlay(true);		
		}
	}
}

void CAnimatorEX::UpdateData()
{
	const tFrameInfo* pCurFrame = GetAnimatorEX()->GetCurFrameInfo();
	tAnimInfo info = {};

	if (NULL == pCurFrame)
	{
		info.vUseAnim.x = 0;
	}

	else
	{
		const CResPtr<CTexture> pTex = pCurFrame->pTex;
		pTex->UpdateData((UINT)SHADER_TYPE::ST_PIXEL, 16);

		info.vUseAnim.x = 1;
		info.vUV = Vec4(pCurFrame->vLeftTopUV.x, pCurFrame->vLeftTopUV.y, pCurFrame->fWidth, pCurFrame->fHeight);
	}

	static const BUFFER* pBuffer = CDevice::GetInst().FindConstBuffer(L"AnimationInfo");

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(pBuffer->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &info, sizeof(tAnimInfo));
	CONTEXT->Unmap(pBuffer->pConstBuffer, 0);

	CONTEXT->VSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->HSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->DSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->GSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->CSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->PSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
}

void CAnimatorEX::Disable()
{
	// Animation 재생인지 아닌지
	// 해당 택스쳐의 UV
	tAnimInfo info = {};
	info.vUseAnim.x = 0;

	static const BUFFER* pBuffer = CDevice::GetInst().FindConstBuffer(L"AnimationInfo");

	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(pBuffer->pConstBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, &info, sizeof(tAnimInfo));
	CONTEXT->Unmap(pBuffer->pConstBuffer, 0);

	CONTEXT->VSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->HSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->DSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->GSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->CSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
	CONTEXT->PSSetConstantBuffers(12, 1, &pBuffer->pConstBuffer);
}

void CAnimatorEX::LoadAnimation(const tDescInfo * _pDescInfo, const wstring & _strFolderPath)
{
	CAnimationEX* pAnim = CAnimationEX::FromDescToAnim(_pDescInfo, _strFolderPath);

	pAnim->SetAnimator(this);
	m_mapAnim.insert(make_pair(_pDescInfo->strAnimName, pAnim));
}



void CAnimatorEX::ChangeAnimation(const wstring & _strKey, UINT _iStartFrameIdx, int _iEndFrameIdx , int _CurIndex)
{
	if (_strKey == L"")
		return;

	CAnimationEX* pAnim = FindAnimation(_strKey);

	if (NULL == pAnim )
	{
#ifdef _DEBUG
		MessageBox(NULL, L"바꾸려는 이미지가 존재 하지 않습니다.", L"실패", MB_OK);
#endif // _DEBUG
		return;
	}

	m_pCurAnim = pAnim;
	m_pCurAnim->SetStartIndex(_iStartFrameIdx);
	m_pCurAnim->SetFrameIndex(_iStartFrameIdx);
	m_pCurAnim->SetEndIndex(_iEndFrameIdx);
	if (_CurIndex >= 0)
		m_pCurAnim->SetFrameIndex(_CurIndex);
	m_pCurAnim->SetAnimFinish(false);
}

const tFrameInfo * CAnimatorEX::GetCurFrameInfo()
{
	if (NULL == m_pCurAnim)
		return NULL;

	return m_pCurAnim->GetCurFrameInfo();
}

void CAnimatorEX::GetAnimations(vector<tAnimPairEX>& _vecOut)
{
	map<wstring, CAnimationEX*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		_vecOut.push_back(tAnimPairEX{ iter->first, iter->second });
	}
}

void CAnimatorEX::DeleteClip(const wstring & _strKey)
{
	map<wstring, CAnimationEX*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
	{
		return;
	}

	m_mapAnim.erase(iter);
}

CAnimationEX * CAnimatorEX::FindAnimation(const wstring & _strKey)
{
	map<wstring, CAnimationEX*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
	{
		return NULL;
	}

	return iter->second;
}

void CAnimatorEX::Save(FILE* _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::ANIMATOR_EX;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	UINT iCount = m_mapAnim.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);

	map<wstring, CAnimationEX*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{
		iter->second->Save(_pFile);
	}

	SaveResKey(m_pCurAnim, _pFile);

	fwrite(&m_bRepeat, 1, 1, _pFile);
}

void CAnimatorEX::Load(FILE* _pFile)
{
	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iCount; ++i)
	{
		CAnimationEX* pAnim = new CAnimationEX;
		pAnim->Load(_pFile);

		pAnim->SetAnimator(this);
		m_mapAnim.insert(make_pair(pAnim->GetKey(), pAnim));
	}

	wstring strKey = LoadResKey(_pFile);
	ChangeAnimation(strKey);

	fread(&m_bRepeat, 1, 1, _pFile);
}

bool CAnimatorEX::IsAnimFinish()
{
	if (m_pCurAnim == NULL)
		return false;

	return m_pCurAnim->IsAnimFinish();
}
