// HierachyDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "HierachyDlg.h"
#include "GameObjDlg.h"
#include "ChangeLayerDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "ComponentView.h"
#include "DebugView.h"

#include "SceneManager.h"
#include "Scene.h"
#include "GameObject.h"
#include "Layer.h"

#include "ResManager.h"
#include "DebugManager.h"
#include "MouseManager.h"

#include "TransForm.h"
// CHierachyDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CHierachyDlg, CDialogEx)

CHierachyDlg::CHierachyDlg(CWnd* pParent /*=NULL*/)
	: CTreeCtrlDlg(IDD_HIERACHYDLG, pParent)
	, m_hDragItem(NULL)
	, m_hTargetItem(NULL)
{
}

CHierachyDlg::~CHierachyDlg()
{
}

void CHierachyDlg::UpdateObjects()
{
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();

	list<CGameObject*> listObj;
	pCurScene->GetAllParentObject(listObj);

	m_Tree.DeleteAllItems();

	list<CGameObject*>::iterator iter = listObj.begin();
	for (; iter != listObj.end(); ++iter)
	{
		if(false==(*iter)->isDead())
		AddGameObject(*iter, NULL);
	}

}

void CHierachyDlg::Initialize()
{
	m_Tree.DeleteAllItems();
}

void CHierachyDlg::AddGameObject(CGameObject * _pObj, HTREEITEM _ParentItem)
{
	HTREEITEM hItem = AddItem(_pObj->GetTagName(), _ParentItem, (DWORD_PTR)_pObj);

	list<CGameObject*>& childlist = _pObj->GetChildList();
	list<CGameObject*>::iterator iter = childlist.begin();
	for (; iter != childlist.end(); ++iter)
	{
		AddGameObject(*iter, hItem);
	}
}

void CHierachyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_Tree);
}


BEGIN_MESSAGE_MAP(CHierachyDlg, CDialogEx)
	ON_WM_TIMER()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CHierachyDlg::OnTvnSelchangedTree1)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CHierachyDlg::OnTvnBegindragTree)
	ON_WM_MOUSELEAVE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE1, &CHierachyDlg::OnNMRClickTree1)
	ON_COMMAND(ID_0_ADDPREFAB, &CHierachyDlg::OnAddprefab)
	ON_COMMAND(ID_1_ADDGAMEOBJECT, &CHierachyDlg::OnAddgameobject)
	ON_COMMAND(ID_0_CHNAGETAG, &CHierachyDlg::OnChangetag)
	ON_COMMAND(ID_0_CHANGELAYER, &CHierachyDlg::OnChangelayer)
	ON_COMMAND(ID_0_DELETECOLLDIER, &CHierachyDlg::On0Deletecolldier)
	ON_COMMAND(ID_0_DELETECHILD, &CHierachyDlg::On0Deletechild)
END_MESSAGE_MAP()


// CHierachyDlg 메시지 처리기입니다.


void CHierachyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CSceneManager::Manager().CheckObjectChange())
	{
		CSceneManager::Manager().SetObjectChange(false);
		UpdateObjects();
	}

	CTreeCtrlDlg::OnTimer(nIDEvent);
}


BOOL CHierachyDlg::OnInitDialog()
{
	CTreeCtrlDlg::OnInitDialog();

	SetTimer(0, 100, NULL);

	CDebugManager::Manager().init();

	return TRUE;  
}

void CHierachyDlg::PostNcDestroy()
{
	delete this;
	CTreeCtrlDlg::PostNcDestroy();
}

void CHierachyDlg::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	
	CGameObject* pObj = (CGameObject*)m_Tree.GetItemData(pNMTreeView->itemNew.hItem);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	CComponentView* pComView = (CComponentView*)pMainWnd->GetComponentView();
	CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();

	pComView->UpdateComponent(pObj);
	pDebugView->UpdateComponent(pObj);

	pMainWnd->m_pGameObject = pObj;

	*pResult = 0;
}


void CHierachyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (NULL == m_hDragItem)
	{
		CTreeCtrlDlg::OnMouseMove(nFlags, point);
		return;
	}
	ClientToScreen(&point);
	m_Tree.ScreenToClient(&point);

	m_hTargetItem = m_Tree.HitTest(point);

	if (NULL != m_hTargetItem)
	{
		m_Tree.SelectDropTarget(m_hTargetItem);
	}

	CTreeCtrlDlg::OnMouseMove(nFlags, point);
}


void CHierachyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (NULL == m_hDragItem)
		return;

	CGameObject* pDragObj = (CGameObject*)m_Tree.GetItemData(m_hDragItem);
	if (NULL != m_hTargetItem)
	{
		CGameObject* pTargetObj = (CGameObject*)m_Tree.GetItemData(m_hTargetItem);
		if (pTargetObj == pDragObj)
			return;

		pTargetObj->AddChild(pDragObj);
	}
	else
	{
		CSceneManager::Manager().GetCurScene()->FindLayer(pDragObj->GetLayerName())->AddParent(pDragObj);
		pDragObj->EndRelationship();
	}

	MoveItem(m_hDragItem, m_hTargetItem);

	ReleaseCapture();
	m_Tree.SelectDropTarget(NULL);
	m_hDragItem = NULL;

	CTreeCtrlDlg::OnLButtonUp(nFlags, point);
}


void CHierachyDlg::OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	m_hDragItem = pNMTreeView->itemNew.hItem;
	

	SetCapture();
}

void CHierachyDlg::OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	CPoint ptInTree, ptInSrceen;
	GetCursorPos(&ptInSrceen); //R버튼 눌린좌표얻음
	ptInTree = ptInSrceen;

	m_Tree.ScreenToClient(&ptInTree);       //트리컨트롤 좌표로변환
	HTREEITEM hit = m_Tree.HitTest(ptInTree);	//좌표에 해당하는 아이템을 자져옴

	CMenu menu, *pMenu;
	menu.LoadMenu(IDR_MENU1); //메뉴리소스 로드

	if (hit) //뭔가선택했으면
	{
		m_Tree.SelectItem(hit);
		pMenu = menu.GetSubMenu(0);

		CGameObject* pObj = (CGameObject*)m_Tree.GetItemData(hit);
		MouseManager::Manager().SetGameObject(pObj);

	}
	else
		pMenu = menu.GetSubMenu(1);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, ptInSrceen.x, ptInSrceen.y, this);

	*pResult = 0;
}

void CHierachyDlg::OnAddprefab()
{
	CGameObject* pObj = MouseManager::Manager().GetGameObject();

	CResManager::Manager().AddPrefab(pObj->GetTagName(), pObj->Clone());
	CResManager::Manager().SetChange(true);
}

void CHierachyDlg::OnAddgameobject()
{
	CGameObject* pObj = new CGameObject;
	pObj->SetTagName(L"EmptyObject");
	pObj->AddComponent(new CTransform);

	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	CLayer* pLayer = pCurScene->FindLayer(L"Default");
	pLayer->AddGameObject(pObj);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	pMainWnd->GetComponentView();

	CComponentView* pView = (CComponentView*)pMainWnd->GetComponentView();
	CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();
	pView->SetTargetObj(pObj);
	pDebugView->SetTargetObj(pObj);
}

void CHierachyDlg::OnChangetag()
{
	CGameObjDlg dlg;
	dlg.m_pTarget = MouseManager::Manager().GetGameObject();
	dlg.DoModal();

	CSceneManager::Manager().SetObjectChange(true);
}


void CHierachyDlg::OnChangelayer()
{
	CChangeLayerDlg dlg;
	dlg.m_pCurScene = CSceneManager::Manager().GetCurScene();
	dlg.m_pTarget = MouseManager::Manager().GetGameObject();

	dlg.DoModal();

	CSceneManager::Manager().SetObjectChange(true);
}


void CHierachyDlg::On0Deletecolldier()
{
	MouseManager::Manager().GetGameObject()->DeleteCollider();
}


void CHierachyDlg::On0Deletechild()
{
	MouseManager::Manager().GetGameObject()->Delete();
	
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	
	CComponentView* pView = (CComponentView*)pMainWnd->GetComponentView();
	CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();

	pView->InitTarget();
	pDebugView->InitTarget();
	CSceneManager::Manager().SetObjectChange(true);
}
