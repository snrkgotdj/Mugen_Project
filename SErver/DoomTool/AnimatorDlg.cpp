#include "stdafx.h"
#include "DoomTool.h"
#include "AnimatorDlg.h"
#include "afxdialogex.h"

#include "Animation.h"

#define MAX_CNT IDC_PIVOTY-IDC_FILENAME+1

// CAnimFSMDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimatorDlg, CDialogEx)

CAnimatorDlg::CAnimatorDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_ANIMATORDLG, pParent)
	, m_bRepeat(FALSE)
	, m_bFocusAnim(false)
	, m_tFrame{}
	, m_bAllFrame(FALSE)
{

}

CAnimatorDlg::~CAnimatorDlg()
{
}

void CAnimatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbAnim);
	DDX_Check(pDX, IDC_CHECK1, m_bRepeat);

	int iStartID = IDC_FILENAME;

	for (int i = 0; i < 26; ++i)
	{
		DDX_Control(pDX, iStartID++, m_arrVal[i]);
	}
	DDX_Control(pDX, IDC_FRAMELIST, m_ListFrame);
	DDX_Text(pDX, IDC_EDIT1, m_strNextClip);
//	DDX_Check(pDX, IDC_ALLFRAME, m_bAllFrame);
}

void CAnimatorDlg::UpdateComponent(CGameObject * _pObj)
{
	m_pTarget = _pObj;

	if (m_pTarget == NULL || m_pTarget->GetAnimator() == NULL)
		return;

	int iCurSel = m_ListFrame.GetCurSel();

	if (m_pTarget->GetAnimator()->GetCurAnimClip() == NULL)
		return;

	int iMaxFrame = m_pTarget->GetAnimator()->GetCurAnimClip()->GetMaxFrame();

	if (m_bRepeat)
	{
		if (iCurSel != -1 && iCurSel < iMaxFrame)
		{
			m_pTarget->GetAnimator()->GetCurAnimClip()->SetFrame(iCurSel, m_tFrame);
		}
		else if (iCurSel > iMaxFrame)
		{
			return;
		}
		else
		{
			m_pTarget->GetAnimator()->GetCurAnimClip()->SetCurFrame(0);
		}
	}
}


BEGIN_MESSAGE_MAP(CAnimatorDlg, CDialogEx)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CAnimatorDlg::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CAnimatorDlg::OnCbnSetfocus)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimatorDlg::OnCbnSelchange)
	ON_BN_CLICKED(IDC_CHECK1, &CAnimatorDlg::OnBnClickedRepchkbox)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_FILENAME, IDC_PIVOTY, &CAnimatorDlg::OnEnChange)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_FILENAME, IDC_PIVOTY, &CAnimatorDlg::OnEnSetFocus)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_FILENAME, IDC_PIVOTY, &CAnimatorDlg::OnEnKillFocus)
	ON_BN_CLICKED(IDC_ADDFRAME, &CAnimatorDlg::OnBnClickedAddframe)
	ON_BN_CLICKED(IDC_DELFRAME, &CAnimatorDlg::OnBnClickedDelframe)
	ON_BN_CLICKED(IDC_SAVECLIP, &CAnimatorDlg::OnBnClickedSaveclip)
	ON_BN_CLICKED(IDC_LOADCLIP, &CAnimatorDlg::OnBnClickedLoadclip)
	ON_LBN_SELCHANGE(IDC_FRAMELIST, &CAnimatorDlg::OnLbnSelchangeFramelist)
	ON_LBN_KILLFOCUS(IDC_FRAMELIST, &CAnimatorDlg::OnLbnKillfocusFramelist)
	ON_EN_CHANGE(IDC_EDIT1, &CAnimatorDlg::OnEnChangeEdit1)
//	ON_BN_CLICKED(IDC_ALLFRAME, &CAnimatorDlg::OnBnClickedAllframe)
END_MESSAGE_MAP()


// CAnimFSMDlg 메시지 처리기입니다.


void CAnimatorDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CComponentDlg::PostNcDestroy();
}


void CAnimatorDlg::OnCbnKillfocus()
{
	//m_vecAnimPair.clear();
	//m_cbAnim.ResetContent();
	//m_bFocusAnim = false;
}


void CAnimatorDlg::OnCbnSetfocus()
{
	if (m_vecAnimPair.empty() && NULL != m_pTarget)
	{
		m_pTarget->GetAnimator()->GetAnimCilps(m_vecAnimPair);

		for (UINT i = 0; i < m_vecAnimPair.size(); ++i)
		{
			m_cbAnim.InsertString(i, m_vecAnimPair[i].strKey.c_str());
		}
	}
	m_bFocusAnim = true;
}

void CAnimatorDlg::OnCbnSelchange()
{
	m_ListFrame.ResetContent();

	m_vecFrame.clear();

	int iIdx = m_cbAnim.GetCurSel();
	m_cbAnim.SetCurSel(iIdx);

	UINT iSize = m_vecAnimPair[iIdx].pAnim->GetVecFrame().size();

	for (UINT i = 0; i < iSize; ++i)
	{
		wstring strTemp = m_vecAnimPair[iIdx].pAnim->GetVecFrame()[i].strFileName;
		LPCWSTR str = strTemp.c_str();

		m_ListFrame.InsertString(-1, str);
		m_tFrame = m_vecAnimPair[iIdx].pAnim->GetVecFrame()[i];
		m_vecFrame.push_back(m_tFrame);
	}
}

BOOL CAnimatorDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_vecAnimPair.clear();
	m_cbAnim.ResetContent();
	m_bFocusAnim = false;
	m_pTarget = NULL;

	return TRUE;
}


void CAnimatorDlg::OnBnClickedRepchkbox()
{
	if (TRUE == m_bRepeat)
		m_bRepeat = FALSE;
	else
		m_bRepeat = TRUE;
}

void CAnimatorDlg::OnEnChange(UINT _id)
{
	int id = _id - 3003;

	if (false == m_arrFocus[id])
		return;

	CString str;
	float f;

	if (0 == id)
	{
		m_arrVal[id].GetWindowTextW(str);
		m_tFrame.strFileName = str;
	}
	else if (1 == id)
	{
		m_arrVal[id].GetWindowTextW(str);
		f = _wtof(str.GetBuffer());
		m_tFrame.fTime = f;
	}
	else if (2 == id)
	{
		m_arrVal[id].GetWindowTextW(str);
		int i = _wtoi(str.GetBuffer());
		m_tFrame.iIdx = i;
	}
	else if (24 == id)
	{
		m_arrVal[id].GetWindowTextW(str);
		m_strFileName = str;
	}
	else if (25 == id)
	{
		m_arrVal[id].GetWindowTextW(str);
		m_strTexKey = str;
	}
	else if (26 == id || 27 == id)
	{
		//m_arrVal[id].GetWindowTextW(str);
		//f = _wtof(str.GetBuffer());
		//m_tFrame.vPivot[id % 2] = f;
	}
	else
	{
		switch (id / 3)
		{
		case 1:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vMove[id % 3] = f;
			break;
		case 2:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vUpColPos[id % 3] = f;
			break;
		case 3:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vUpColScale[id % 3] = f;
			break;
		case 4:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vDownColPos[id % 3] = f;
			break;
		case 5:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vDownColScale[id % 3] = f;
			break;
		case 6:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vHitColPos[id % 3] = f;
			break;
		case 7:
			m_arrVal[id].GetWindowTextW(str);
			f = _wtof(str.GetBuffer());
			m_tFrame.vHitColScale[id % 3] = f;
			break;
		}
	}

	int iCurSel = m_ListFrame.GetCurSel();
	if (iCurSel != -1)
	{
		m_vecFrame[iCurSel] = m_tFrame;

		m_pTarget->GetAnimator()->GetCurAnimClip()->SetCurFrame(iCurSel);
		m_pTarget->GetAnimator()->GetCurAnimClip()->SetFrame(iCurSel, m_tFrame);
	}
}

void CAnimatorDlg::OnEnSetFocus(UINT _id)
{
	int id = _id - 3003;
	m_arrFocus[id] = true;
}

void CAnimatorDlg::OnEnKillFocus(UINT _id)
{
	int id = _id - 3003;
	m_arrFocus[id] = false;
}

void CAnimatorDlg::OnBnClickedAddframe()
{
	m_vecFrame.push_back(tAnimFrame{});
	m_ListFrame.InsertString(-1, L"NewFrame");
}

void CAnimatorDlg::OnBnClickedDelframe()
{
	int iCurSel = m_ListFrame.GetCurSel();
	if (iCurSel != -1)
	{
		m_ListFrame.DeleteString(iCurSel);

		vector<tAnimFrame>::iterator iter = m_vecFrame.begin();
		for (iter; iter != m_vecFrame.end(); ++iter)
		{
			if ((*iter).iIdx == m_vecFrame[iCurSel].iIdx)
			{
				m_vecFrame.erase(iter);
				break;
			}
		}
	}
}

void CAnimatorDlg::OnBnClickedSaveclip()
{
	CAnimation* pAnim = new CAnimation;

	pAnim->SetVecFrame(m_vecFrame);
	pAnim->SaveFrameFile(m_strFileName, m_strTexKey);

	SAFE_DELETE(pAnim);
}

void CAnimatorDlg::OnBnClickedLoadclip()
{
	m_ListFrame.ResetContent();
	m_vecFrame.clear();

	CAnimation* pAnim = new CAnimation(m_strFileName, m_bRepeat, (wstring)m_strNextClip);
	pAnim->AddFrameByFile(m_strFileName+L".bin", m_strTexKey);
	m_pTarget->GetAnimator()->AddAnimClip(pAnim);
	m_pTarget->GetAnimator()->SetAnim(pAnim);

	for (UINT i = 0; i < pAnim->GetVecFrame().size()-1; ++i)
	{
		wstring strTemp = pAnim->GetVecFrame()[i].strFileName;
		LPCWSTR str = strTemp.c_str();

		m_ListFrame.InsertString(-1, str);
		m_vecFrame.push_back(pAnim->GetVecFrame()[i]);
	}
}

void CAnimatorDlg::OnLbnSelchangeFramelist()
{
	int iCurSel = m_ListFrame.GetCurSel();
	if (iCurSel != -1)
	{
		m_tFrame = m_vecFrame[iCurSel];

		wstring strTemp;
		LPCWSTR str;
		TCHAR wstr[127];

		for (int i = 0; i < MAX_CNT; ++i)
		{
			if (0 == i)
			{
				strTemp = m_tFrame.strFileName;
				str = strTemp.c_str();
				m_arrVal[i].SetWindowTextW(str);
			}
			else if (1 == i)
			{
				swprintf(wstr, L"%f", m_tFrame.fTime);
				GetDlgItem(IDC_TIME)->SetWindowTextW(wstr);
			}
			else if (2 == i)
			{
				swprintf(wstr, L"%d", m_tFrame.iIdx);
				GetDlgItem(IDC_IDX)->SetWindowTextW(wstr);
			}
			else
			{
				switch (i / 3)
				{
				case 1:
					swprintf(wstr, L"%f", m_tFrame.vMove[i % 3]);
					GetDlgItem(IDC_MOVEX + i % 3)->SetWindowTextW(wstr);
					break;
				case 2:
					swprintf(wstr, L"%f", m_tFrame.vUpColPos[i % 3]);
					GetDlgItem(IDC_UPPOSX + i % 3)->SetWindowTextW(wstr);
					break;
				case 3:
					swprintf(wstr, L"%f", m_tFrame.vUpColScale[i % 3]);
					GetDlgItem(IDC_UPSCLX + i % 3)->SetWindowTextW(wstr);
					break;
				case 4:
					swprintf(wstr, L"%f", m_tFrame.vDownColPos[i % 3]);
					GetDlgItem(IDC_DOWNPOSX + i % 3)->SetWindowTextW(wstr);
					break;
				case 5:
					swprintf(wstr, L"%f", m_tFrame.vDownColScale[i % 3]);
					GetDlgItem(IDC_DOWNSCLX + i % 3)->SetWindowTextW(wstr);
					break;
				case 6:
					swprintf(wstr, L"%f", m_tFrame.vHitColPos[i % 3]);
					GetDlgItem(IDC_HITPOSX + i % 3)->SetWindowTextW(wstr);
					break;
				case 7:
					swprintf(wstr, L"%f", m_tFrame.vHitColScale[i % 3]);
					GetDlgItem(IDC_HITSCLX + i % 3)->SetWindowTextW(wstr);
					break;
				}
			}
		}
	}
}

void CAnimatorDlg::OnLbnKillfocusFramelist()
{
	int iCurSel = m_ListFrame.GetCurSel();
	if (iCurSel != -1)
	{
		m_vecFrame[iCurSel] = m_tFrame;
	}

	//m_pTarget->AnimFSM()->GetCurAnimClip()->SetVecFrame(m_vecFrame);
}


void CAnimatorDlg::OnEnChangeEdit1()
{
	GetDlgItem(IDC_EDIT1)->GetWindowTextW(m_strNextClip);
}


void CAnimatorDlg::OnBnClickedAllframe()
{
	if (TRUE == m_bAllFrame)
		m_bAllFrame = FALSE;
	else
		m_bAllFrame = TRUE;
}
