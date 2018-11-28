// TransformDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "TransformDlg.h"
#include "afxdialogex.h"

#include "GameObject.h"
#include "TransForm.h"

// CTransformDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_TRANSFORMDLG, pParent)
	, m_arrFocus{}
{

}

CTransformDlg::~CTransformDlg()
{
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	
	int iStartID = IDC_POSX;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			DDX_Control(pDX, iStartID++, m_arrTrans[i][j]);
		}
	}
}

void CTransformDlg::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj)
		return;

	m_pTarget = _pObj;

	CTransform* pTrans = _pObj->GetTransform();

	Vec3 arrTrans[3] = { pTrans->GetLocalPos(), pTrans->GetLocalRot(), pTrans->GetLocalScale() };

	CString str;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (false == m_arrFocus[i][j])
			{
				str.Format(L"%f", arrTrans[i][j]);
				m_arrTrans[i][j].SetWindowTextW(str);
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_POSX, IDC_SCAZ, &CTransformDlg::OnEnSetfocus)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_POSX, IDC_SCAZ, &CTransformDlg::OnEnKillfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_POSX, IDC_SCAZ, &CTransformDlg::OnEnChange)
END_MESSAGE_MAP()


// CTransformDlg 메시지 처리기입니다.


void CTransformDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	CComponentDlg::PostNcDestroy();
}

void CTransformDlg::OnEnSetfocus(UINT _id)
{
	int iIdx = _id - 2000;
	m_arrFocus[iIdx / 3][iIdx % 3] = true;
}


void CTransformDlg::OnEnKillfocus(UINT _id)
{
	int iIdx = _id - 2000;
	m_arrFocus[iIdx / 3][iIdx % 3] = false;
}


void CTransformDlg::OnEnChange(UINT _id)
{
	int id = _id - 2000;

	if (false == m_arrFocus[id / 3][id % 3])
		return;

	CString str;
	m_arrTrans[id / 3][id % 3].GetWindowTextW(str);

	float f = (float)_wtof(str.GetBuffer());

	if (id / 3 == 0)
	{
		Vec3 vPos = m_pTarget->GetTransform()->GetLocalPos();
		vPos[id % 3] = f;
		m_pTarget->GetTransform()->SetLocalPos(vPos);
	}

	else if (id / 3 == 1)
	{
		Vec3 vRot = m_pTarget->GetTransform()->GetLocalRot();
		vRot[id % 3] = f;
		m_pTarget->GetTransform()->SetLocalRot(vRot);
	}

	else
	{
		Vec3 vScale = m_pTarget->GetTransform()->GetLocalScale();
		vScale[id % 3] = f;
		m_pTarget->GetTransform()->SetLocalScale(vScale);
	}
}

BOOL CTransformDlg::PreTranslateMessage(MSG* pMsg)
{
	// 엔터키
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		HWND hWnd = ::GetFocus();

		::SetFocus(NULL);
	}

	return CComponentDlg::PreTranslateMessage(pMsg);
}
