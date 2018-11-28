#pragma once
#include "afxcmn.h"

class CScene;
// CShootLayerDlg 대화 상자입니다.

class CShootLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShootLayerDlg)

public:
	CShootLayerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CShootLayerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOOTLAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	virtual BOOL OnInitDialog();
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
};
