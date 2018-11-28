#include "Animation.h"
#include "Animator.h"
#include "Component.h"
#include "MeshRender.h"
#include "Transform.h"

#include "GameObject.h"

#include "ResManager.h"
#include "TimeManager.h"
#include "ShaderManager.h"
#include "PathManager.h"

#include "Material.h"
#include "Texture.h"

CAnimation::CAnimation()
	: m_fTime(0.f)
	, m_iCurFrame(0)
	, m_bRepeat(false)
{
}

CAnimation::CAnimation(const wstring& _strClipName, bool _bRepeat, const wstring& _strNextClip)
	: m_fTime(0.f)
	, m_iCurFrame(0)
	, m_bRepeat(_bRepeat)
{
	m_strClipName = _strClipName;
	m_strNextClipName = _strNextClip;
}

CAnimation::CAnimation(const CAnimation & _pAnim)
	: m_fTime(0.f)
	, m_iCurFrame(0)
	, m_bRepeat(_pAnim.m_bRepeat)
{
	m_strClipName = _pAnim.m_strClipName;
	m_strNextClipName = _pAnim.m_strNextClipName;

	vector<tAnimFrame>::const_iterator iter = _pAnim.m_vecFrame.begin();
	vector<tAnimFrame>::const_iterator iterEnd = _pAnim.m_vecFrame.end();

	for (iter; iter != iterEnd; ++iter)
	{
		m_vecFrame.push_back(*iter);
	}
}

CAnimation::~CAnimation()
{
}

void CAnimation::Awake()
{
}

void CAnimation::Start()
{
}

int CAnimation::Update()
{
	float fDeltaTime = CTimeManager::Manager().GetDeltaTime();
	m_fTime += fDeltaTime;

	if (!m_vecFrame.empty())
	{
		if (m_fTime > m_vecFrame[m_iCurFrame].fTime)
		{
			if (m_iCurFrame >= m_vecFrame.size() - 1)
			{
				if (m_bRepeat)
					m_iCurFrame = 0;
				else
					m_pAnimator->SetAnim(m_strNextClipName);
			}
			else
				m_iCurFrame++;
			m_fTime = 0.f;
		}
		else
		{
			CTransform* pTr = m_pAnimator->GetGameObject()->GetTransform();
			Vec3 vPos = pTr->GetLocalPos();
			vPos += m_vecFrame[m_iCurFrame].vMove*fDeltaTime;
			pTr->SetLocalPos(vPos);
		}
	}

	return 0;
}

void CAnimation::Render()
{
}

void CAnimation::AddFrame(tAnimFrame _tFrame)
{
	m_vecFrame.push_back(_tFrame);
}

void CAnimation::AddFrame(wstring _strFileName, float _fTime, int _iIdx, Vec3 _vPosModify)
{
	tAnimFrame tFrame = {};
	tFrame.strFileName = _strFileName;
	tFrame.fTime = _fTime;
	tFrame.iIdx = _iIdx;
	tFrame.vMove = _vPosModify;

	m_vecFrame.push_back(tFrame);
}

void CAnimation::AddFrame(const wstring & _strFileName, int _iStart, int _iEnd,
	float _fTime, Vec3 _vPosModify)
{
	if (_iStart > _iEnd)
	{
		for (int i = _iStart - 1; i > _iEnd - 1; --i)
		{
			tAnimFrame tFrame = {};
			tFrame.strFileName = _strFileName;
			tFrame.iIdx = i;
			tFrame.fTime = _fTime;
			tFrame.vMove = _vPosModify;
			AddFrame(tFrame);
		}
	}

	else
	{
		for (int i = _iStart; i < _iEnd; ++i)
		{
			tAnimFrame tFrame = {};
			tFrame.strFileName = _strFileName;
			tFrame.iIdx = i;
			tFrame.fTime = _fTime;
			tFrame.vMove = _vPosModify;
			AddFrame(tFrame);
		}
	}
}

void CAnimation::AddFrameByFile(const wstring & _strFileName, const wstring& _strTexKey)
{
	m_strFileName = _strFileName;
	m_strTexKey = _strTexKey;

	FILE* pFile = NULL;

	wstring strResPath = CPathManager::GetResPath();
	wstring strAnimPath = strResPath + L"\\AnimBin\\" + _strTexKey + L"\\" + _strFileName;

	_wfopen_s(&pFile, strAnimPath.c_str(), L"rb");

	tAnimFrame tFrame = {};

	int iSize;
	fread(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		fread(&tFrame.fTime, sizeof(float), 1, pFile);
		fread(&tFrame.iIdx, sizeof(int), 1, pFile);
		fread(&tFrame.vMove, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vUpColPos, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vUpColScale, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vDownColPos, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vDownColScale, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vHitColPos, sizeof(Vec3), 1, pFile);
		fread(&tFrame.vHitColScale, sizeof(Vec3), 1, pFile);
		tFrame.strFileName = _strTexKey;
		m_vecFrame.push_back(tFrame);

		wchar_t szIdxName[16];
		wchar_t szIdxName2[16];
		if(tFrame.iIdx<10)
			wsprintfW(szIdxName, L"000%d", tFrame.iIdx);
		else if (tFrame.iIdx<100&&tFrame.iIdx>=10)
			wsprintfW(szIdxName, L"00%d", tFrame.iIdx);
		else if (tFrame.iIdx<1000&&tFrame.iIdx>=100)
			wsprintfW(szIdxName, L"0%d", tFrame.iIdx);

		wstring strFileName = L"Texture\\Player_" + _strTexKey + L"\\" + _strTexKey + szIdxName + L".png";
		wsprintfW(szIdxName2, L"%d", tFrame.iIdx);

		wstring strFileKey = _strTexKey + szIdxName2;
		CTexture* pTex = (CTexture*)CResManager::Manager().Load<CTexture>(strFileKey, strFileName);
	}

	fclose(pFile);
}

void CAnimation::SaveFrameFile(const wstring & _strFileName, const wstring& _strTexKey)
{
	FILE* pFile = NULL;

	wstring strResPath = CPathManager::GetResPathEX();
	wstring strAnimPath = strResPath + L"AnimBin\\" + _strTexKey + L"\\" + _strFileName;

	_wfopen_s(&pFile, strAnimPath.c_str(), L"wb");

	int iSize = m_vecFrame.size();
	fwrite(&iSize, sizeof(int), 1, pFile);

	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		fwrite(&m_vecFrame[i].fTime, sizeof(float), 1, pFile);
		fwrite(&m_vecFrame[i].iIdx, sizeof(int), 1, pFile);
		fwrite(&m_vecFrame[i].vMove, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vUpColPos, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vUpColScale, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vDownColPos, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vDownColScale, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vHitColPos, sizeof(Vec3), 1, pFile);
		fwrite(&m_vecFrame[i].vHitColScale, sizeof(Vec3), 1, pFile);
	}

	fclose(pFile);
}

void CAnimation::SetFrame(UINT _idx, tAnimFrame _frame)
{
	if (_idx<0 || _idx>m_vecFrame.size() - 1)
		return;

	m_vecFrame[_idx] = _frame;
}

void CAnimation::SetVecFrame(vector<tAnimFrame>& _vecFrame)
{
	m_vecFrame.clear();

	for (UINT i = 0; i < _vecFrame.size(); ++i)
	{
		m_vecFrame.push_back(_vecFrame[i]);
	}
}

bool CAnimation::IsClipEnd()
{
	if (m_iCurFrame == m_vecFrame.size() - 1)
		return true;

	return false;
}