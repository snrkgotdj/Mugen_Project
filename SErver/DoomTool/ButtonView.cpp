// ButtonView.cpp : implementation file
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ButtonView.h"

#include "PathManager.h"
#include "TimeManager.h"
#include "SaveLoadManager.h"
#include "SceneManager.h"

#include "MainFrm.h"
#include "ComponentView.h"
#include "HierachyView.h"
#include "DebugView.h"

#include "GameObject.h"
#include "TransForm.h"
#include "Camera.h"
#include "ScriptManager.h"
#include "Script.h"


// CButtonView

IMPLEMENT_DYNCREATE(CButtonView, CFormView)

CButtonView::CButtonView()
	: CFormView(IDD_BUTTONVIEW)
	, m_bPlayOn(false)
{

}

CButtonView::~CButtonView()
{
}

void CButtonView::update()
{
	m_pToolCam->Update();
	m_pToolCam->LateUpdate();
	m_pToolCam->FinalUpdate();
}

void CButtonView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON1, m_Play);
	DDX_Control(pDX, IDC_BUTTON2, m_Pause);
	DDX_Control(pDX, IDC_BUTTON3, m_Stop);
}

BEGIN_MESSAGE_MAP(CButtonView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON1, &CButtonView::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CButtonView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_BUTTON3, &CButtonView::OnBnClickedStop)
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CButtonView diagnostics

#ifdef _DEBUG
void CButtonView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CButtonView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CButtonView message handlers


void CButtonView::OnBnClickedPlay()
{
	if (m_bPlayOn == false)
	{
		m_bPlayOn = true;

		wstring strPath = CPathManager::GetResPathEX();
		strPath += L"Save\\Play\\play.scene";
		CSaveLoadManager::Manager().SaveScene(strPath.c_str());
	}
	CSceneManager::Manager().SetPlayMode(true);
	
}


void CButtonView::OnBnClickedPause()
{
	CSceneManager::Manager().SetPlayMode(false);
}


void CButtonView::OnBnClickedStop()
{
	if (m_bPlayOn == false)
		return;

	m_bPlayOn = false;

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	((CHierachyView*)pMainWnd->GetHierachyView())->UpdateObject();
	((CComponentView*)pMainWnd->GetComponentView())->InitTarget();
	((CDebugView*)pMainWnd->GetDebugView())->InitTarget();

	CSceneManager::Manager().SetPlayMode(false);
	wstring strPath = CPathManager::GetResPathEX();
	strPath += L"Save\\Play\\play.scene";
	CSaveLoadManager::Manager().LoadScene(strPath.c_str());
}


int CButtonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pToolCam = new CGameObject;
	m_pToolCam->AddComponent<CTransform>(new CTransform);
	m_pToolCam->AddComponent<CCamera>(new CCamera);
	
	m_pToolCam->GetTransform()->SetLocalPos(Vec3(0.f, 0.f, -1.f));
	m_pToolCam->GetCamera()->CheckRenderLayerAll();

	CSceneManager::Manager().SetToolCam(m_pToolCam);

	return 0;
}


void CButtonView::OnDestroy()
{
	CFormView::OnDestroy();

	SAFE_DELETE(m_pToolCam);
}
