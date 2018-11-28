// GameObjDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "GameObjDlg.h"
#include "afxdialogex.h"

#include "GameObject.h"


// CGameObjDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CGameObjDlg, CDialogEx)

CGameObjDlg::CGameObjDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GAMEOBJDLG, pParent)
{

}

CGameObjDlg::~CGameObjDlg()
{
}

void CGameObjDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editName);
}


BEGIN_MESSAGE_MAP(CGameObjDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGameObjDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CGameObjDlg �޽��� ó�����Դϴ�.


void CGameObjDlg::OnBnClickedOk()
{
	CString str;
	m_editName.GetWindowTextW(str);
	m_pTarget->SetTagName(str.GetBuffer());

	CDialogEx::OnOK();
}
