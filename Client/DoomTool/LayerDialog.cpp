// LayerDialog.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "LayerDialog.h"
#include "afxdialogex.h"

#include "MouseManager.h"
#include "SceneMAnager.h"
#include "Layer.h"
#include "Scene.h"

#include "GameObject.h"
#include "CheckColLayerDlg.h"


// CLayerDialog ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CLayerDialog, CDialogEx)

CLayerDialog::CLayerDialog(CWnd* pParent /*=NULL*/)
	: CTreeCtrlDlg(IDD_LAYERDIALOG, pParent)
{

}

CLayerDialog::~CLayerDialog()
{
}

void CLayerDialog::UpdateObjects()
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();

	vector<CLayer*>& vecLayer = pCurScene->GetVecLayer();
	
	m_Tree.DeleteAllItems();

	vector<CLayer*>::iterator iter = vecLayer.begin();

	int iCount = pCurScene->GetLayerCount();
	for (int i = 0; i <iCount; ++iter, ++i)
	{
		HTREEITEM hItem = AddItem((*iter)->GetLayerName(), NULL, NULL);

		list<CGameObject*>& listObj = (*iter)->GetObjList();
		list<CGameObject*>::iterator listiter = listObj.begin();
		for (; listiter != listObj.end(); ++listiter)
		{
			AddItem((*listiter)->GetTagName(), hItem, (DWORD_PTR)(*listiter));
		}
	}
}

void CLayerDialog::Initialize()
{
	m_Tree.DeleteAllItems();
}

void CLayerDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CLayerDialog, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CLayerDialog::OnNMRClickTree1)
	ON_COMMAND(ID_2_CHECKCOLLAYER, &CLayerDialog::On2Checkcollayer)
END_MESSAGE_MAP()


// CLayerDialog �޽��� ó�����Դϴ�.


BOOL CLayerDialog::OnInitDialog()
{
	CTreeCtrlDlg::OnInitDialog();

	SetTimer(0, 100, NULL);
	
	return TRUE;  
}


void CLayerDialog::OnTimer(UINT_PTR nIDEvent)
{
	CTreeCtrlDlg::OnTimer(nIDEvent);

	if (CSceneManager::Manager().CheckLayerChange())
	{
		CSceneManager::Manager().SetLayerChange(false);
		UpdateObjects();
	}
}


void CLayerDialog::PostNcDestroy()
{
	delete this;
	CTreeCtrlDlg::PostNcDestroy();
}


void CLayerDialog::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint ptInTree, ptInSrceen;
	GetCursorPos(&ptInSrceen); //R��ư ������ǥ����
	ptInTree = ptInSrceen;

	m_Tree.ScreenToClient(&ptInTree);       //Ʈ����Ʈ�� ��ǥ�κ�ȯ
	HTREEITEM hit = m_Tree.HitTest(ptInTree);	//��ǥ�� �ش��ϴ� �������� ������

	CMenu menu, *pMenu;
	menu.LoadMenu(IDR_MENU1); //�޴����ҽ� �ε�

	if (hit) //��������������
	{
		m_Tree.SelectItem(hit);
		pMenu = menu.GetSubMenu(2);

		CString str = m_Tree.GetItemText(hit);

		CGameObject* pObj = (CGameObject*)m_Tree.GetItemData(hit);
		MouseManager::Manager().SetGameObject(pObj);
		
		MouseManager::Manager().SetText(str.GetBuffer());
	}

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, ptInSrceen.x, ptInSrceen.y, this);

	*pResult = 0;
}


void CLayerDialog::On2Checkcollayer()
{
	CCheckColLayerDlg Dlg;
	Dlg.m_strLayerName = MouseManager::Manager().GetText();
	Dlg.m_pCurScene = CSceneManager::Manager().GetCurScene();
	Dlg.DoModal();
}
