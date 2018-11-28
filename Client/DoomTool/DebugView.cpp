// DebugView.cpp : implementation file
//

#include "stdafx.h"
#include "DoomTool.h"
#include "DebugView.h"

#include "EditAnimDlg.h"

// CDebugView

IMPLEMENT_DYNCREATE(CDebugView, CFormView)

CDebugView::CDebugView()
	: CFormView(IDD_DEBUGVIEW)
	, m_pTarget(NULL)
{

}

CDebugView::~CDebugView()
{
}

void CDebugView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

void CDebugView::Update()
{
	if (NULL == m_pTarget)
		return;

	UpdateComponent(m_pTarget);
}

BEGIN_MESSAGE_MAP(CDebugView, CFormView)
	ON_WM_CREATE()
END_MESSAGE_MAP()


// CDebugView diagnostics

#ifdef _DEBUG
void CDebugView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CDebugView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

// CDebugView message handlers

void CDebugView::SetEditAnim(CAnimationEX * _pAnim)
{
	if (NULL == _pAnim)
	{
		MessageBox(L"수정할 애니메이션이 잘못됬습니다.", L"오류");
		return;
	}
	m_pEditAnimDlg->ShowWindow(true);
	m_pEditAnimDlg->SetAnimation(_pAnim);
}

void CDebugView::SetMousePos(CPoint* _point)
{
	m_pEditAnimDlg->SetMousePos(Vec2(_point->x, _point->y));
}

void CDebugView::InitTarget()
{
	m_pTarget = NULL;

	m_pEditAnimDlg->SetAnimation(NULL);
	m_pEditAnimDlg->SetTarget(NULL);
	m_pEditAnimDlg->ShowWindow(false);
}

void CDebugView::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj)
		return;

	if (_pObj != m_pTarget)
	{
		m_pTarget = _pObj;
		m_pEditAnimDlg->ShowWindow(false);
	}

	else if (NULL != m_pTarget->GetComponent(COMPONENT_TYPE::ANIMATOR_EX))
	{
		m_pEditAnimDlg->UpdateComponent(_pObj);
	}
	
}

int CDebugView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pEditAnimDlg = new CEditAnimDlg;
	m_pEditAnimDlg->Create(IDD_EDITANIMDLG, this);
	m_pEditAnimDlg->ShowWindow(false);
	m_pEditAnimDlg->UpdateWindow();

	return 0;
}
