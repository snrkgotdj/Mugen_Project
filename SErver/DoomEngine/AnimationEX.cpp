#include "AnimationEX.h"

#include "PathManager.h"
#include "ResManager.h"
#include "TimeManager.h"

#include "AnimatorEX.h"
#include "GameObject.h"
#include "TransForm.h"

#define DEFAULT_ANIMTIME 0.05f;

CAnimationEX::CAnimationEX()
	:m_bPlay(true)
	, m_iCurIdx(0)
	, m_fAccTime(0.f)
	, m_iStartIndx(0)
	, m_iEndIndex(-1)
	, m_bIsAnimFinish(false)
{
}


CAnimationEX::~CAnimationEX()
{
}

void CAnimationEX::ChangeAllClipTerm(float _fTerm)
{
	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		m_vecFrame[i].fTerm = _fTerm;
	}
}

void CAnimationEX::ChangeAllClipUpColInfo(Vec3 _pos, Vec3 _size)
{
	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		m_vecFrame[i].tUpCol.vPos = _pos;
		m_vecFrame[i].tUpCol.vSize = _size;
	}
}

void CAnimationEX::ChangeAllClipDownColInfo(Vec3 _pos, Vec3 _size)
{
	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		m_vecFrame[i].tDownCol.vPos = _pos;
		m_vecFrame[i].tDownCol.vSize = _size;
	}
}

void CAnimationEX::ChangeAllClipHitColInfo(Vec3 _pos, Vec3 _size)
{
	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		m_vecFrame[i].tHitCol.vPos = _pos;
		m_vecFrame[i].tHitCol.vSize = _size;
	}
}

void CAnimationEX::ChangeAllClipUpColPos(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tUpCol.vPos.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tUpCol.vPos.y = _data;
		}
	}
}

void CAnimationEX::ChangeAllClipDownColPos(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tDownCol.vPos.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tDownCol.vPos.y = _data;
		}
	}
}

void CAnimationEX::ChangeAllClipHitColPos(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tHitCol.vPos.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tHitCol.vPos.y = _data;
		}
	}
}

void CAnimationEX::ChangeAllClipUpColSize(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tUpCol.vSize.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tUpCol.vSize.y = _data;
		}
	}
}

void CAnimationEX::ChangeAllClipDownColSize(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tDownCol.vSize.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tDownCol.vSize.y = _data;
		}
	}
}

void CAnimationEX::ChangeAllClipHitColSize(float _data, bool _isX)
{
	if (_isX == true)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tHitCol.vSize.x = _data;
		}
	}

	else if (_isX == false)
	{
		for (UINT i = 0; i < m_vecFrame.size(); ++i)
		{
			m_vecFrame[i].tHitCol.vSize.y = _data;
		}
	}
}

void CAnimationEX::LateUpdate()
{
	if (false == m_bPlay)
	{
		return;
	}
	m_bIsAnimFinish = false;
	m_fAccTime += DT;

	if (m_vecFrame[m_iCurIdx].fTerm <= m_fAccTime)
	{
		m_iCurIdx += 1;
		m_fAccTime = 0.f;

		if (m_iEndIndex == -1)
		{
			if (m_iCurIdx >= m_vecFrame.size())
			{
				m_iCurIdx = m_vecFrame.size() - 1;
				m_bIsAnimFinish = true;
				m_bPlay = false;
			}
		}
		else
		{
			if (m_iCurIdx >= (UINT)m_iEndIndex + 1)
			{
				m_iCurIdx = m_iEndIndex - 1;
				m_bIsAnimFinish = true;
				m_bPlay = false;
			}
		}
	}
}

CAnimationEX * CAnimationEX::FromDescToAnim(const tDescInfo * _pDesc, const wstring & _strFolderPath)
{
	CAnimationEX* pAnim = new CAnimationEX;

	pAnim->SetKey(_pDesc->strAnimName);

	if (-1 == _strFolderPath.find(_strFolderPath))
		assert(NULL);

	wstring strFolderPath = _strFolderPath;
	strFolderPath = strFolderPath.substr(wcslen(CPathManager::GetResPathEX()), strFolderPath.length());

	if (_pDesc->iContinue)
	{
		WIN32_FIND_DATA tData = {};
		HANDLE hHandle = FindFirstFile(wstring(_strFolderPath + L"\\*.*").c_str(), &tData);

		if (INVALID_HANDLE_VALUE == hHandle)
		{
			MessageBox(NULL, L"존재하지 않는 폴더입니다.", L"애니메이션 로드 실패", MB_OK);
			assert(NULL);
		}

		if (_pDesc->strTexName != tData.cFileName)
		{
			while (true)
			{
				if (-1 == FindNextFile(hHandle, &tData))
					assert(NULL);

				if (_pDesc->strTexName == tData.cFileName)
					break;
			}
		}
		tFrameInfo info = {};
		int i = 0;
		do
		{
			wstring strFilePath = strFolderPath + L"\\" + tData.cFileName;
			CTexture* pTex = (CTexture*)CResManager::Manager().Load<CTexture>(tData.cFileName, strFilePath);

			info.vLeftTopUV = Vec2(0.f, 0.f);
			info.fHeight = 1.f;
			info.fWidth = 1.f;
			info.pTex = pTex;
			info.fTerm = DEFAULT_ANIMTIME;

			pAnim->AddFrame(info);
			if (++i >= _pDesc->iFrameCount)
				break;
		} while (FindNextFile(hHandle, &tData));

		FindClose(hHandle);
	}
	else
	{
		CTexture* pTex =
			(CTexture*)CResManager::Manager().Load<CTexture>(_pDesc->strTexName.c_str()
				, wstring(strFolderPath + L"\\" + _pDesc->strTexName).c_str());

		for (UINT j = 0; j < (UINT)_pDesc->iFrameCount; ++j)
		{
			float fXPlus = _pDesc->vSize.x / pTex->GetWith();
			tFrameInfo tFrame = {};
			tFrame.vLeftTopUV.x = _pDesc->vLeftTop.x / pTex->GetWith() + (fXPlus*j);
			tFrame.vLeftTopUV.y = _pDesc->vLeftTop.y / pTex->GEtHeight();

			tFrame.fWidth = fXPlus;
			tFrame.fHeight = _pDesc->vSize.y / pTex->GEtHeight();

			tFrame.fTerm = DEFAULT_ANIMTIME;
			tFrame.pTex = pTex;

			pAnim->AddFrame(tFrame);
		}
	}

	return pAnim;
}

void CAnimationEX::Save(FILE * _pFile)
{
	SaveResKey(this, _pFile);

	UINT iCount = m_vecFrame.size();
	fwrite(&iCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < m_vecFrame.size(); ++i)
	{
		fwrite(&m_vecFrame[i].vLeftTopUV, sizeof(Vec2), 1, _pFile);
		fwrite(&m_vecFrame[i].fWidth, sizeof(float), 1, _pFile);
		fwrite(&m_vecFrame[i].fHeight, sizeof(float), 1, _pFile);
		fwrite(&m_vecFrame[i].fTerm, sizeof(float), 1, _pFile);
		fwrite(&m_vecFrame[i].tUpCol, sizeof(ColInfo), 1, _pFile);
		fwrite(&m_vecFrame[i].tDownCol, sizeof(ColInfo), 1, _pFile);
		fwrite(&m_vecFrame[i].tHitCol, sizeof(ColInfo), 1, _pFile);
		fwrite(&m_vecFrame[i].tMove, sizeof(tPOINT), 1, _pFile);
		fwrite(&m_vecFrame[i].fSpeed, sizeof(float), 1, _pFile);
		fwrite(&m_vecFrame[i].fJumpSpeed, sizeof(float), 1, _pFile);

		SaveResKey((CRes*)m_vecFrame[i].pTex, _pFile);
	}
}

void CAnimationEX::Load(FILE * _pFile)
{
	m_strKey = LoadResKey(_pFile);

	UINT iCount = 0;
	fread(&iCount, sizeof(UINT), 1, _pFile);

	tFrameInfo info = {};
	for (UINT i = 0; i < iCount; ++i)
	{
		fread(&info.vLeftTopUV, sizeof(Vec2), 1, _pFile);
		fread(&info.fWidth, sizeof(float), 1, _pFile);
		fread(&info.fHeight, sizeof(float), 1, _pFile);
		fread(&info.fTerm, sizeof(float), 1, _pFile);
		fread(&info.tUpCol, sizeof(ColInfo), 1, _pFile);
		fread(&info.tDownCol, sizeof(ColInfo), 1, _pFile);
		fread(&info.tHitCol, sizeof(ColInfo), 1, _pFile);
		fread(&info.tMove, sizeof(tPOINT), 1, _pFile);
		fread(&info.fSpeed, sizeof(float), 1, _pFile);
		fread(&info.fJumpSpeed, sizeof(float), 1, _pFile);

		wstring strKey = LoadResKey(_pFile);
		info.pTex = (CTexture*)CResManager::Manager().Load<CTexture>(strKey);

		m_vecFrame.push_back(info);
	}
}

void CAnimationEX::SetPlay(bool _bOn)
{
	m_bPlay = _bOn;
	m_iCurIdx = m_iStartIndx;
}