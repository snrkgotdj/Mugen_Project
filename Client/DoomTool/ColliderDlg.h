#pragma once


#include "ComponentDlg.h"
#include "afxwin.h"

// CColliderDlg 대화 상자입니다.

class CColliderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CColliderDlg)

public:
	CColliderDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColliderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
