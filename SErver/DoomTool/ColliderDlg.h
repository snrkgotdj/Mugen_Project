#pragma once


#include "ComponentDlg.h"
#include "afxwin.h"

// CColliderDlg ��ȭ �����Դϴ�.

class CColliderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CColliderDlg)

public:
	CColliderDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CColliderDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual void UpdateComponent(CGameObject* _pObj);

	bool m_arrFocus[3][3];
	CEdit m_arrCollider[3][3];

	afx_msg void OnEnSetfocus(UINT _id);
	afx_msg void OnEnKillfocus(UINT _id);
	afx_msg void OnEnChange(UINT _id);
	virtual void PostNcDestroy();
};
