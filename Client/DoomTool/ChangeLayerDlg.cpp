// ChangeLayerDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ChangeLayerDlg.h"
#include "afxdialogex.h"
#include "GameObject.h"
#include "SceneManager.h"

#include "Scene.h"
#include "Layer.h"

// CChangeLayerDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChangeLayerDlg, CDialogEx)

CChangeLayerDlg::CChangeLayerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHANGELAYERDLG, pParent)
{

}

CChangeLayerDlg::~CChangeLayerDlg()
{
}

void CChangeLayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_LayerList);
}


BEGIN_MESSAGE_MAP(CChangeLayerDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChangeLayerDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CChangeLayerDlg 메시지 처리기입니다.

BOOL CChangeLayerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	vector<CLayer*>& vecLayer = m_pCurScene->GetVecLayer();
	vector<CLayer*>::iterator iter = vecLayer.begin();

	for (int i = 0; (*iter) != NULL ; ++iter, ++i)
	{
		m_LayerList.InsertItem(i, (*iter)->GetLayerName().c_str());
	}

	return TRUE;  
}

void CChangeLayerDlg::OnBnClickedOk()
{
	int iIdx = m_LayerList.GetSelectionMark();
	CString str = m_LayerList.GetItemText(iIdx, NULL);

	CSceneManager::Manager().GetCurScene()->FindLayer(m_pTarget->GetLayerName())->RemoveListOnly(m_pTarget);

	CLayer* pLayer = CSceneManager::Manager().GetCurScene()->FindLayer(str.GetBuffer());
	
	pLayer->AddGameObject(m_pTarget);

	CDialogEx::OnOK();
}
