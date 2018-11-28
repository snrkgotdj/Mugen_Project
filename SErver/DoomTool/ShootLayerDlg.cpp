// ShootLayerDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ShootLayerDlg.h"
#include "afxdialogex.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Layer.h"
#include "GameObject.h"
#include "Camera.h"

// CShootLayerDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CShootLayerDlg, CDialogEx)

CShootLayerDlg::CShootLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SHOOTLAYERDLG, pParent)
{

}

CShootLayerDlg::~CShootLayerDlg()
{
}

void CShootLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LayerList);
}


BEGIN_MESSAGE_MAP(CShootLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CShootLayerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CShootLayerDlg �޽��� ó�����Դϴ�.


BOOL CShootLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	vector<CLayer*>& vecLayer = m_pCurScene->GetVecLayer();
	vector<CLayer*>::iterator iter = vecLayer.begin();

	for (int i = 0; (*iter) != NULL; ++iter, ++i)
	{
		m_LayerList.InsertItem(i, (*iter)->GetLayerName().c_str());
	}

	return TRUE;
}


void CShootLayerDlg::OnBnClickedOk()
{
	int iIdx = m_LayerList.GetSelectionMark();
	CString str = m_LayerList.GetItemText(iIdx, NULL);

	CGameObject* pCamera = m_pCurScene->FindLayer(L"Camera")->FindObjectByTag(L"Main_Camera");
	pCamera->GetCamera()->CheckRenderLayer(str.GetBuffer());

	CDialogEx::OnOK();
}
