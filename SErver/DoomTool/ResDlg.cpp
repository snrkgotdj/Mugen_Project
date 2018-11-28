// ResDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ResDlg.h"
#include "afxdialogex.h"

#include "ResManager.h"
#include "Sound.h"
#include "Res.h"

// CResDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CResDlg, CDialogEx)

CResDlg::CResDlg(CWnd* pParent /*=NULL*/)
	: CTreeCtrlDlg(IDD_RESDLG, pParent)
	, m_hMesh(NULL)
	, m_hTexture(NULL)
	, m_hPrefab(NULL)
	, m_hMaterial(NULL)
	, m_hClones(NULL)
{

}

CResDlg::~CResDlg()
{
}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, (IDC_TREE7), m_Tree);
}

void CResDlg::UpdateObjects()
{
	HTREEITEM hRoot = m_Tree.GetRootItem();

	while (true)
	{
		if (NULL == hRoot)
			break;
		DeleteChildren(hRoot);
		hRoot = m_Tree.GetNextItem(hRoot, TVGN_NEXT);
	}

	map<wstring, CResPtr<CMesh>>& mapMesh = CResManager::Manager().GetMeshList();
	map<wstring, CResPtr<CTexture>>& mapTexture = CResManager::Manager().GetTextureList();
	map<wstring, CResPtr<CMaterial>>& mapMtrl = CResManager::Manager().GetMaterialList();
	map<wstring, CResPtr<CPrefab>>& mapPrefab = CResManager::Manager().GetPrefabList();
	map<wstring, CResPtr<CSound>>& mapSound = CResManager::Manager().GetSoundList();
	vector<CResPtr<CRes>>& vecClones = CResManager::Manager().GetCloneList();
	
	for (auto& pair : mapMesh)
		AddItem(pair.first.c_str(), m_hMesh, (DWORD_PTR)((CMesh*)(pair.second)));

	for (auto& pair : mapTexture)
		AddItem(pair.first.c_str(), m_hTexture, (DWORD_PTR)((CTexture*)(pair.second)));

	for (auto& pair : mapMtrl)
		AddItem(pair.first.c_str(), m_hMaterial, (DWORD_PTR)((CMaterial*)(pair.second)));

	for (auto& pair : mapPrefab)
		AddItem(pair.first.c_str(), m_hPrefab, (DWORD_PTR)((CPrefab*)(pair.second)));

	for (auto& pair : mapSound)
		AddItem(pair.first.c_str(), m_hSound, (DWORD_PTR)((CSound*)(pair.second)));

	for (auto& res : vecClones)
		AddItem(res->GetKey(), m_hClones, (DWORD_PTR)((CRes*)(res)));

	m_Tree.Invalidate();
}



BEGIN_MESSAGE_MAP(CResDlg, CDialogEx)
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

BOOL CResDlg::OnInitDialog()
{
	CTreeCtrlDlg::OnInitDialog();

	m_hMesh = AddItem(L"Mesh", NULL, NULL);
	m_hTexture = AddItem(L"Texture", NULL, NULL);
	m_hMaterial = AddItem(L"Material", NULL, NULL);
	m_hPrefab = AddItem(L"Prefab", NULL, NULL);
	m_hSound = AddItem(L"Sound", NULL, NULL);
	m_hClones = AddItem(L"Clones", NULL, NULL);

	SetTimer(0, 500, NULL);

	return TRUE; 
}


void CResDlg::PostNcDestroy()
{
	delete this;
	CTreeCtrlDlg::PostNcDestroy();
}


void CResDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (!CResManager::Manager().IsChange())
		return;

	CResManager::Manager().SetChange(false);

	UpdateObjects();

	CTreeCtrlDlg::OnTimer(nIDEvent);
}


void CResDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	HTREEITEM hSelect = m_Tree.GetSelectedItem();
	CString str = m_Tree.GetItemText(hSelect);

	CTreeCtrlDlg::OnLButtonUp(nFlags, point);
}


void CResDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CTreeCtrlDlg::OnMouseMove(nFlags, point);
}
