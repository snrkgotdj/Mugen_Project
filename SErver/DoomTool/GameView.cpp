// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "DoomTool.h"
#include "GameView.h"

#include "GameObject.h"
#include "Scene.h"
#include "SceneManager.h"

#include "MainFrm.h"
#include "ComponentView.h"
#include "DebugView.h"

#include "EditAnimDlg.h"


// CGameView

IMPLEMENT_DYNCREATE(CGameView, CFormView)

CGameView::CGameView()
	: CFormView(IDD_GAMEVIEW)
	, m_bEditAnim(false)
	, m_bMouseClicked(false)
{

}

CGameView::~CGameView()
{
}

void CGameView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGameView, CFormView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CGameView diagnostics

#ifdef _DEBUG
void CGameView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CGameView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CGameView message handlers


void CGameView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RECT rt = {};
	GetWindowRect(&rt);

	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	CGameObject* pObj = pCurScene->FindObjec(Vec2((float)point.x, (float)point.y)
										  , Vec2((float)(rt.right - rt.left), (float)(rt.bottom - rt.top)));

	if (NULL != pObj)
	{
		CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
		CComponentView* pComView = (CComponentView*)pMainWnd->GetComponentView();
		CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();
		pComView->UpdateComponent(pObj);
		pDebugView->UpdateComponent(pObj);
	}

	vecMouseDown = pCurScene->MtoW(Vec2((float)point.x, (float)point.y)
		, Vec2((float)(rt.right - rt.left), (float)(rt.bottom - rt.top)));

	m_bMouseClicked = true;

}


void CGameView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	RECT rt = {};
	GetWindowRect(&rt);
	
	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	vecMouseUp = pCurScene->MtoW(Vec2((float)point.x, (float)point.y)
		, Vec2((float)(rt.right - rt.left), (float)(rt.bottom - rt.top)));


	if (true == m_bEditAnim)
	{
		CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
		CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();
		pDebugView->GetEditAnimDlg()->SetOffsetByMouse(vecMouseDown, vecMouseUp);
	}
	m_bMouseClicked = false;

	CFormView::OnLButtonUp(nFlags, point);
}


void CGameView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bMouseClicked == false)
		return;

	RECT rt = {};
	GetWindowRect(&rt);

	CScene* pCurScene = CSceneManager::Manager().GetCurScene();
	vecMouseUp = pCurScene->MtoW(Vec2((float)point.x, (float)point.y)
		, Vec2((float)(rt.right - rt.left), (float)(rt.bottom - rt.top)));

	if (true == m_bEditAnim)
	{
		CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
		CDebugView* pDebugView = (CDebugView*)pMainWnd->GetDebugView();
		pDebugView->GetEditAnimDlg()->SetOffsetByMouse(vecMouseDown, vecMouseUp);
	}

	CFormView::OnMouseMove(nFlags, point);
}
