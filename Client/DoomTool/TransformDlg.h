#pragma once
#include "ComponentDlg.h"
#include "afxwin.h"


// CTransformDlg 대화 상자입니다.

class CTransformDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTransformDlg)

public:
	CTransformDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTransformDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual void UpdateComponent(CGameObject* _pObj);

	CEdit m_arrTrans[3][3];
	bool m_arrFocus[3][3];

	virtual void PostNcDestroy();
	afx_msg void OnEnSetfocus(UINT _id);
	afx_msg void OnEnKillfocus(UINT _id);
	afx_msg void OnEnChange(UINT _id);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
