// ColliderDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ColliderDlg.h"
#include "afxdialogex.h"
#include "Collider.h"


// CColliderDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CColliderDlg, CDialogEx)

CColliderDlg::CColliderDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_COLLIDERDLG, pParent)
	, m_arrFocus{}
{
}

CColliderDlg::~CColliderDlg()
{
}

void CColliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int iStartIdx = IDC_CPOSX;

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			DDX_Control(pDX, iStartIdx++, m_arrCollider[i][j]);
		}
	}
}

void CColliderDlg::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj)
		return;

	m_pTarget = _pObj;

	CCollider* pCollider = _pObj->GetCollider();

	Vec3 arrCol[3] = { pCollider->GetWorldPos(), pCollider->GetOffsetPos(), pCollider->GetScale() };

	CString str;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (false == m_arrFocus[i][j])
			{
				str.Format(L"%f", arrCol[i][j]);
				m_arrCollider[i][j].SetWindowTextW(str);
			}
		}
	}
}


BEGIN_MESSAGE_MAP(CColliderDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_CPOSX, IDC_CSCAZ, &CColliderDlg::OnEnSetfocus)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_CPOSX, IDC_CSCAZ, &CColliderDlg::OnEnKillfocus)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_CPOSX, IDC_CSCAZ, &CColliderDlg::OnEnChange)
END_MESSAGE_MAP()


// CColliderDlg �޽��� ó�����Դϴ�.


void CColliderDlg::OnEnSetfocus(UINT _id)
{
	int iIdx = _id - IDC_CPOSX;
	m_arrFocus[iIdx / 3][iIdx % 3] = true;
}


void CColliderDlg::OnEnKillfocus(UINT _id)
{
	int iIdx = _id - IDC_CPOSX;
	m_arrFocus[iIdx / 3][iIdx % 3] = false;
}


void CColliderDlg::OnEnChange(UINT _id)
{
	int id = _id - IDC_CPOSX;

	if (false == m_arrFocus[id / 3][id % 3])
		return;

	CString str;
	m_arrCollider[id / 3][id % 3].GetWindowTextW(str);

	float f = (float)_wtof(str.GetBuffer());

	if (id / 3 == 0)
	{
		Vec3 vPos = m_pTarget->GetCollider()->GetWorldPos();
		vPos[id % 3] = f;
		m_pTarget->GetCollider()->SetWorldPos(vPos);
	}

	else if (id / 3 == 1)
	{
		Vec3 vRot = m_pTarget->GetCollider()->GetOffsetPos();
		vRot[id % 3] = f;
		m_pTarget->GetCollider()->SetPos(vRot);
	}

	else
	{
		Vec3 vScale = m_pTarget->GetCollider()->GetScale();
		vScale[id % 3] = f;
		m_pTarget->GetCollider()->SetScale(vScale);
	}

}


void CColliderDlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	delete this;

	CComponentDlg::PostNcDestroy();
}
