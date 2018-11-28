// LayerDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "LayerDlg.h"
#include "afxdialogex.h"

#include "Scene.h"


// CLayerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLayerDlg, CDialogEx)

CLayerDlg::CLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LAYERDLG, pParent)
{

}

CLayerDlg::~CLayerDlg()
{
}

void CLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editLayerName);
}


BEGIN_MESSAGE_MAP(CLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLayerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLayerDlg �޽��� ó�����Դϴ�.


void CLayerDlg::OnBnClickedOk()
{
	CString str;
	m_editLayerName.GetWindowTextW(str);
	m_pCurScene->AddLayer(str.GetBuffer());

	CDialogEx::OnOK();
}
