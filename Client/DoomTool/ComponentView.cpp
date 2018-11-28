// ComponentView.cpp : implementation file
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ComponentView.h"

#include "GameObject.h"
#include "TransformDlg.h"
#include "MeshRenderDlg.h"
#include "ColliderDlg.h"
#include "AnimatorDlgEX.h"
#include "ScriptDlg.h"

#include "MeshRender.h"
#include "Collider2D.h"
#include "Camera.h"
#include "Script.h"
#include "AnimatorEX.h"
#include "CameraDlg.h"

// CComponentView

IMPLEMENT_DYNCREATE(CComponentView, CFormView)

CComponentView::CComponentView()
	: CFormView(IDD_COMPONENTVIEW)
	, m_pTargetObj(NULL)
	, m_arrComDlg{}
	, m_bAC(false)
{

}

CComponentView::~CComponentView()
{
}

void CComponentView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_bnAC);
	DDX_Control(pDX, IDC_COMBO2, m_cbAC);

	static bool bCall = false;
	if (bCall == true)
		return;

	bCall = true;
	m_cbAC.ShowWindow(false);
	m_cbAC.InsertString(0, L"MESHRENDER");
	m_cbAC.InsertString(1, L"COLLIDER_2D");
	m_cbAC.InsertString(2, L"COLLIDER_3D");
	m_cbAC.InsertString(3, L"ANIMATOR_EX");
	m_cbAC.InsertString(4, L"CAMERA");
}

BEGIN_MESSAGE_MAP(CComponentView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CComponentView::OnBnAddComponent)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CComponentView::OnCbnSelchange)
	ON_CBN_KILLFOCUS(IDC_COMBO2, &CComponentView::OnCbnKillfocus)
	ON_CBN_SETFOCUS(IDC_COMBO2, &CComponentView::OnCbnSetfocus)
	ON_WM_VSCROLL()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CComponentView diagnostics

#ifdef _DEBUG
void CComponentView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CComponentView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CComponentView message handlers


int CComponentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM] = new CTransformDlg;
	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM]->Create(IDD_TRANSFORMDLG, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::TRANSFORM]->UpdateWindow();

	m_arrComDlg[(UINT)COMPONENT_TYPE::MESHRENDER] = new CMeshRenderDlg;
	m_arrComDlg[(UINT)COMPONENT_TYPE::MESHRENDER]->Create(IDD_MESHRENDERDLG, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::MESHRENDER]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::MESHRENDER]->UpdateWindow();

	m_arrComDlg[(UINT)COMPONENT_TYPE::COLLIDER] = new CColliderDlg;
	m_arrComDlg[(UINT)COMPONENT_TYPE::COLLIDER]->Create(IDD_COLLIDERDLG, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::COLLIDER]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::COLLIDER]->UpdateWindow();

	m_arrComDlg[(UINT)COMPONENT_TYPE::ANIMATOR_EX] = new CAnimatorDlgEX;
	m_arrComDlg[(UINT)COMPONENT_TYPE::ANIMATOR_EX]->Create(IDD_ANIMATORDLGEX, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::ANIMATOR_EX]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::ANIMATOR_EX]->UpdateWindow();

	m_arrComDlg[(UINT)COMPONENT_TYPE::CAMERA] = new CCameraDlg;
	m_arrComDlg[(UINT)COMPONENT_TYPE::CAMERA]->Create(IDD_CAMERADLG, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::CAMERA]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::CAMERA]->UpdateWindow();

	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT] = new CScriptDlg;
	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->Create(IDD_SCRIPTDLG, this);
	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->ShowWindow(false);
	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->UpdateWindow();

	return 0;
}


void CComponentView::InitTarget()
{
	m_pTargetObj = NULL;
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrComDlg[i])
			m_arrComDlg[i]->SetTarget(NULL);
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{

		m_arrComDlg[i]->ShowWindow(false);
	}
}

void CComponentView::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj)
		return;

	if (_pObj != m_pTargetObj)
	{
		m_pTargetObj = _pObj;
		for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
		{

			m_arrComDlg[i]->ShowWindow(false);
		}
	}
	
	UINT iStartPosY = 0;
	RECT rtDlg = {};

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if ( NULL == m_pTargetObj->GetComponent((COMPONENT_TYPE)i))
			continue;

		m_arrComDlg[i]->GetWindowRect(&rtDlg);

		m_arrComDlg[i]->ShowWindow(true);
		m_arrComDlg[i]->SetWindowPos(NULL, 0, iStartPosY
			, rtDlg.right - rtDlg.left
			, rtDlg.bottom - rtDlg.top, 0);
		m_arrComDlg[i]->UpdateComponent(_pObj);

		iStartPosY += rtDlg.bottom - rtDlg.top - 1;

	}

	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->GetWindowRect(&rtDlg);

	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->ShowWindow(true);
	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->SetWindowPos(NULL, 0, iStartPosY
		, rtDlg.right - rtDlg.left
		, rtDlg.bottom - rtDlg.top, 0);
	m_arrComDlg[(UINT)COMPONENT_TYPE::SCRIPT]->UpdateComponent(_pObj);

	iStartPosY += rtDlg.bottom - rtDlg.top + 10;

	m_bnAC.GetWindowRect(&rtDlg);
	ScreenToClient(&rtDlg);

	m_bnAC.SetWindowPos(NULL, rtDlg.left, iStartPosY
		, rtDlg.right - rtDlg.left
		, rtDlg.bottom - rtDlg.top, 0);

	iStartPosY += rtDlg.bottom - rtDlg.top;

	m_cbAC.GetWindowRect(&rtDlg);
	ScreenToClient(&rtDlg);

	m_cbAC.SetWindowPos(NULL, rtDlg.left, iStartPosY
		, rtDlg.right - rtDlg.left
		, rtDlg.bottom - rtDlg.top, 0);

	if (m_bAC == false)
	{
		m_cbAC.SetCurSel(-1);
	}
}

void CComponentView::Update()
{
	if (NULL == m_pTargetObj)
		return;

	UpdateComponent(m_pTargetObj);
}

void CComponentView::OnDestroy()
{
	CFormView::OnDestroy();

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrComDlg[i])
			m_arrComDlg[i]->DestroyWindow();
	}
}


void CComponentView::OnBnAddComponent()
{
	m_cbAC.ShowWindow(true);
	m_cbAC.SendMessage(WM_LBUTTONDOWN);
	m_cbAC.SendMessage(WM_LBUTTONUP);
}


void CComponentView::OnCbnSelchange()
{
	if (NULL == m_pTargetObj)
		return;

	int iIdx = m_cbAC.GetCurSel();
	if (0 == iIdx)
	{
		if (NULL == m_pTargetObj->GetComponent(COMPONENT_TYPE::MESHRENDER))
			m_pTargetObj->AddComponent<CMeshRender>(new CMeshRender);
	}
	else if(1 == iIdx)
	{
		if (NULL == m_pTargetObj->GetComponent(COMPONENT_TYPE::COLLIDER))
			m_pTargetObj->AddComponent<CCollider>(new Collider2D);
	}
	else if (2 == iIdx)
	{
	
	}
	else if (3 == iIdx)
	{
		if (NULL == m_pTargetObj->GetComponent(COMPONENT_TYPE::ANIMATOR_EX))
			m_pTargetObj->AddComponent<CAnimatorEX>(new CAnimatorEX);
	}

	else if (4 == iIdx)
	{
		if (NULL == m_pTargetObj->GetComponent(COMPONENT_TYPE::CAMERA))
			m_pTargetObj->AddComponent<CCamera>(new CCamera);
	}

	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		m_arrComDlg[i]->ShowWindow(false);
	}

	UpdateComponent(m_pTargetObj);
	m_cbAC.ShowWindow(false);
}


void CComponentView::OnCbnKillfocus()
{
	m_bAC = false;
	m_cbAC.ShowWindow(false);
}


void CComponentView::OnCbnSetfocus()
{
	m_bAC = true;
}

void CComponentView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_cbAC.ShowWindow(false);

	CFormView::OnLButtonDown(nFlags, point);
}
