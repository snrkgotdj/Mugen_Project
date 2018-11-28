// AnimatorDlgEX.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "AnimatorDlgEX.h"
#include "afxdialogex.h"

#include "AnimationEX.h"
#include "AnimatorEX.h"
#include "AddAnimDlg.h"

#include "MainFrm.h"
#include "DebugView.h"


// CAnimatorDlgEX 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimatorDlgEX, CDialogEx)

CAnimatorDlgEX::CAnimatorDlgEX(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_ANIMATORDLGEX, pParent)
	, m_bRepeatCheck(FALSE)
{

}

CAnimatorDlgEX::~CAnimatorDlgEX()
{
}

void CAnimatorDlgEX::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_Animation);
	DDX_Check(pDX, IDC_CHECK1, m_bRepeatCheck);
}

void CAnimatorDlgEX::UpdateComponent(CGameObject * _pObj)
{
	m_pTarget = _pObj;

	if (m_pTarget == NULL)
		return;

	if (m_bFocusRC == true)
		return;

	CAnimationEX* pCurAnim = m_pTarget->GetAnimatorEX()->GetCurAnim();

	m_vecAnimInfo.clear();
	m_Animation.ResetContent();

	m_pTarget->GetAnimatorEX()->GetAnimations(m_vecAnimInfo);

	for (UINT i = 0; i < m_vecAnimInfo.size(); ++i)
	{
		m_Animation.InsertString(i, m_vecAnimInfo[i].strKey.c_str());
	}

	if (NULL == pCurAnim)
		m_Animation.SetCurSel(-1);

	else
	{
		int iIdx = m_Animation.FindString(0, pCurAnim->GetKey().c_str());
		m_Animation.SetCurSel(iIdx);
	}

	m_bRepeatCheck = m_pTarget->GetAnimatorEX()->IsRepeat();
	UpdateData(false);
}


BEGIN_MESSAGE_MAP(CAnimatorDlgEX, CDialogEx)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CAnimatorDlgEX::OnCbnSetfocusCombo1)
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CAnimatorDlgEX::OnCbnKillfocusCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAnimatorDlgEX::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CHECK1, &CAnimatorDlgEX::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimatorDlgEX::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimatorDlgEX::OnBnClickedEditAnim)
END_MESSAGE_MAP()



void CAnimatorDlgEX::PostNcDestroy()
{
	delete this;

	CDialogEx::PostNcDestroy();
}


void CAnimatorDlgEX::OnCbnSetfocusCombo1()
{
	m_bFocusRC = true;
}


void CAnimatorDlgEX::OnCbnKillfocusCombo1()
{
	m_bFocusRC = false;
}


void CAnimatorDlgEX::OnCbnSelchangeCombo1()
{
	CString str;
	m_Animation.GetWindowTextW(str);

	m_pTarget->GetAnimatorEX()->ChangeAnimation(str.GetBuffer());
}


void CAnimatorDlgEX::OnBnClickedCheck1()
{
	if (m_bRepeatCheck == true)
		m_bRepeatCheck = false;

	else
		m_bRepeatCheck = true;

	UpdateData(false);
	m_pTarget->GetAnimatorEX()->SetRepeat(m_bRepeatCheck);
}


void CAnimatorDlgEX::OnBnClickedButton1()
{
	CAddAnimDlg dlg;
	dlg.m_pTarget = m_pTarget;
	dlg.DoModal();
}


void CAnimatorDlgEX::OnBnClickedEditAnim()
{
	CAnimationEX* pAnimation = m_pTarget->GetAnimatorEX()->GetCurAnim();

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	((CDebugView*)pMainWnd->GetDebugView())->SetEditAnim(pAnimation);
}
