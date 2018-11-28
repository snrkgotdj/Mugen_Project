// ListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ListDlg.h"
#include "afxdialogex.h"

#include "ResManager.h"
#include "Mesh.h"
#include "Material.h"


// CListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CListDlg, CDialogEx)

CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LISTDLG, pParent)
{

}

CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_List);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
END_MESSAGE_MAP()


// CListDlg 메시지 처리기입니다.


void CListDlg::InitListControl()
{
	if (RESOURCE_TYPE::MESH == m_eResType)
	{
		map<wstring, CResPtr<CMesh>>& mapMesh = CResManager::Manager().GetMeshList();
		map<wstring, CResPtr<CMesh>>::iterator iter = mapMesh.begin();

		m_vecItemText.reserve(mapMesh.size());
		for (int i = 0; iter != mapMesh.end(); ++iter, ++i)
		{
			m_vecItemText.push_back(iter->first.c_str());

			LVITEM lvItem = {};
			lvItem.iItem = i;
			lvItem.iImage = 0;
			lvItem.pszText = m_vecItemText[i].GetBuffer();
			lvItem.cchTextMax = 255;
			lvItem.cColumns = 1;
			lvItem.mask = LVIF_PARAM | LVIF_TEXT;

			CRes* pRes = iter->second;
			lvItem.lParam = (LPARAM)(pRes);

			m_List.InsertItem(&lvItem);
		}
	}
	else if (RESOURCE_TYPE::MATERIAL == m_eResType)
	{
		map<wstring, CResPtr<CMaterial>>& mapMaterial = CResManager::Manager().GetMaterialList();
		map<wstring, CResPtr<CMaterial>>::iterator iter = mapMaterial.begin();

		m_vecItemText.reserve(mapMaterial.size());
		for (int i = 0; iter != mapMaterial.end(); ++iter, ++i)
		{
			m_vecItemText.push_back(iter->first.c_str());

			LVITEM lvItem = {};
			lvItem.iItem = i;
			lvItem.iImage = 0;
			lvItem.pszText = m_vecItemText[i].GetBuffer();
			lvItem.cchTextMax = 255;
			lvItem.cColumns = 1;
			lvItem.mask = LVIF_PARAM | LVIF_TEXT;

			CRes* pRes = iter->second;
			lvItem.lParam = (LPARAM)(pRes);

			m_List.InsertItem(&lvItem);
		}
	}
}

BOOL CListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_List.ModifyStyle(NULL, LVS_SINGLESEL, 0);

	InitListControl();
	return TRUE;  
}


void CListDlg::OnOK()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	int iSel = m_List.GetSelectionMark();

	LVITEM lvItem = {};
	lvItem.iItem = iSel;
	LPARAM lp = m_List.GetItemData(iSel);

	EndDialog(lp);
}


void CListDlg::OnCancel()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	EndDialog(NULL);
}
