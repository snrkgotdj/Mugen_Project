// CheckColLayerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "CheckColLayerDlg.h"
#include "afxdialogex.h"
#include "GameObject.h"
#include "SceneManager.h"
#include "CollisionManager.h"

#include "Scene.h"
#include "Layer.h"

// CCheckColLayerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCheckColLayerDlg, CDialogEx)

CCheckColLayerDlg::CCheckColLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHECKCOLLAYERDLG, pParent)
{

}

CCheckColLayerDlg::~CCheckColLayerDlg()
{
}

void CCheckColLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LayerList);
}


BEGIN_MESSAGE_MAP(CCheckColLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CCheckColLayerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CCheckColLayerDlg 메시지 처리기입니다.


BOOL CCheckColLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CDialogEx::OnInitDialog();

	vector<CLayer*>& vecLayer = m_pCurScene->GetVecLayer();
	vector<CLayer*>::iterator iter = vecLayer.begin();

	for (int i = 0; (*iter) != NULL; ++iter, ++i)
	{
		m_LayerList.InsertItem(i, (*iter)->GetLayerName().c_str());
	}

	return TRUE; 
}


void CCheckColLayerDlg::OnBnClickedOk()
{
	int iIdx = m_LayerList.GetSelectionMark();
	CString str = m_LayerList.GetItemText(iIdx, NULL);

	CCollisionManager::Manager().CheckLayerCollision(str.GetBuffer(), m_strLayerName.c_str());

	CDialogEx::OnOK();
}
