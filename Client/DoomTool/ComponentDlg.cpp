#include "stdafx.h"
#include "ComponentDlg.h"


CComponentDlg::CComponentDlg()
	:m_pTarget(NULL)
{
}

CComponentDlg::CComponentDlg(UINT nIDTemplate, CWnd * pParent)
	: CDialogEx(nIDTemplate, pParent)
	, m_pTarget(NULL)
{
}


CComponentDlg::~CComponentDlg()
{
}


void CComponentDlg::OnOK()
{

}


void CComponentDlg::OnCancel()
{

}
