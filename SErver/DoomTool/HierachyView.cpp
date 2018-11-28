// HierachyView.cpp : implementation file
//

#include "stdafx.h"
#include "DoomTool.h"
#include "HierachyView.h"

#include "HierachyDlg.h"
#include "ResDlg.h"
#include "LayerDialog.h"
// CHierachyView

IMPLEMENT_DYNCREATE(CHierachyView, CFormView)

CHierachyView::CHierachyView()
	: CFormView(IDD_HIERACHYVIEW)
	, m_pHierachyDlg(NULL)
	, m_pResDlg(NULL)
	, m_pLayerDlg(NULL)
{

}

CHierachyView::~CHierachyView()
{
}

void CHierachyView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHierachyView, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CHierachyView diagnostics

#ifdef _DEBUG
void CHierachyView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierachyView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierachyView message handlers


int CHierachyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.

	m_pHierachyDlg = new CHierachyDlg();
	m_pHierachyDlg->Create(IDD_HIERACHYDLG, this);
	m_pHierachyDlg->ShowWindow(true);
	m_pHierachyDlg->UpdateWindow();

	m_pResDlg = new CResDlg();
	m_pResDlg->Create(IDD_RESDLG, this);
	m_pResDlg->ShowWindow(true);
	m_pResDlg->UpdateWindow();

	m_pLayerDlg = new CLayerDialog();
	m_pLayerDlg->Create(IDD_LAYERDIALOG, this);
	m_pLayerDlg->ShowWindow(true);
	m_pLayerDlg->UpdateWindow();
	
	return 0;
}


void CHierachyView::UpdateObject()
{
	((CHierachyDlg*)m_pHierachyDlg)->UpdateObjects();

}

void CHierachyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	RECT rt = {};
	m_pHierachyDlg->GetClientRect(&rt);
	int iHeight = rt.bottom - rt.top;

	m_pResDlg->GetClientRect(&rt);

	m_pResDlg->SetWindowPos(NULL, 0, iHeight, rt.right - rt.left, 400, 0);

	iHeight += rt.bottom - rt.top;

	m_pLayerDlg->GetClientRect(&rt);

	m_pLayerDlg->SetWindowPos(NULL, 0, iHeight, rt.right - rt.left, 400, 0);
}
