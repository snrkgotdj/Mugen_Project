// CameraDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "CameraDlg.h"
#include "afxdialogex.h"

#include "Camera.h"


// CCameraDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_CAMERADLG, pParent)
	, m_bFar(false)
	, m_bNear(false)
	, m_bScale(false)
{

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT2, m_editFar);
	DDX_Control(pDX, IDC_EDIT1, m_editNear);
	DDX_Control(pDX, IDC_EDIT3, m_editScale);
	DDX_Control(pDX, IDC_SLIDER1, m_Slider);
}

void CCameraDlg::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj)
		return;

	m_pTarget = _pObj;

	CCamera* Cam = m_pTarget->GetCamera();

	if (m_bNear == false && m_bFar == false && m_bScale == false)
	{
		float fNear = Cam->GetNear();
		float fFar = Cam->GetFar();
		float fScale = Cam->GetScale();

		CString str;
		str.Format(L"%f", fNear);
		m_editNear.SetWindowTextW(str);

		str.Format(L"%f", fFar);
		m_editFar.SetWindowTextW(str);
	
		str.Format(L"%f", fScale);
		m_editScale.SetWindowTextW(str);
	}
}


BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT1, &CCameraDlg::OnEnSetfocusNear)
	ON_EN_SETFOCUS(IDC_EDIT2, &CCameraDlg::OnEnSetfocusFar)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CCameraDlg::OnEnKillfocusNear)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CCameraDlg::OnEnKillfocusFar)
	ON_EN_CHANGE(IDC_EDIT1, &CCameraDlg::OnEnChangeNear)
	ON_EN_CHANGE(IDC_EDIT2, &CCameraDlg::OnEnChangeFar)
	ON_EN_SETFOCUS(IDC_EDIT3, &CCameraDlg::OnEnSetfocusScale)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CCameraDlg::OnEnKillfocusScale)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON1, &CCameraDlg::OnBnClickedShootLayer)
END_MESSAGE_MAP()


// CCameraDlg �޽��� ó�����Դϴ�.


void CCameraDlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CComponentDlg::PostNcDestroy();
}


void CCameraDlg::OnEnSetfocusNear()
{
	m_bNear = true;
}


void CCameraDlg::OnEnSetfocusFar()
{
	m_bFar = true;
}


void CCameraDlg::OnEnSetfocusScale()
{
	m_bScale = true;
}


void CCameraDlg::OnEnKillfocusNear()
{
	m_bNear = false;
}


void CCameraDlg::OnEnKillfocusFar()
{
	m_bFar = false;
}


void CCameraDlg::OnEnKillfocusScale()
{
	m_bScale = false;
}


void CCameraDlg::OnEnChangeNear()
{
	if (false == m_bNear)
		return;

	CString str;
	m_editNear.GetWindowTextW(str);

	float fNear = (float)_wtof(str.GetBuffer());
	m_pTarget->GetCamera()->SetNear(fNear);
}


void CCameraDlg::OnEnChangeFar()
{
	if (false == m_bFar)
		return;

	CString str;
	m_editFar.GetWindowTextW(str);

	float fFar = (float)_wtof(str.GetBuffer());
	m_pTarget->GetCamera()->SetNear(fFar);
}


BOOL CCameraDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_Slider.SetRange(0, 400);
	m_Slider.SetRangeMin(0);
	m_Slider.SetRangeMax(400);
	m_Slider.SetPos(100);

	m_Slider.SetTicFreq(1);
	m_Slider.SetPageSize(1);

	return TRUE;  
}


void CCameraDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	if (m_pTarget == NULL)
		return;

	if (pScrollBar)
	{
		// � �����̴����� �˻�
		if (pScrollBar == (CScrollBar*)&m_Slider)
		{
			// �����̴��� ��ġ�� �˻��Ѵ�.
			float fScale = (float)m_Slider.GetPos() / 100.f;
			if (fScale == 0.f)
				fScale = 0.05f;

			m_pTarget->GetCamera()->SetScale(fScale);
		}
	}
	else
	{
		// CScrollView�� ��ӹ��� ���� ��� �������� ��ũ�ѷ� ���۽� pScrollBar�� NULL�ȴ�.
	}


	CComponentDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

#include "ShootLayerDlg.h"
#include "SceneManager.h"

void CCameraDlg::OnBnClickedShootLayer()
{
	CShootLayerDlg dlg;
	dlg.m_pCurScene = CSceneManager::Manager().GetCurScene();

	dlg.DoModal();
}
