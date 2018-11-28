#pragma once


// CLayerDialog 대화 상자입니다.
#include "TreeCtrlDlg.h"
#include "afxcmn.h"

class CLayerDialog : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CLayerDialog)

public:
	CLayerDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayerDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERDIALOG };
#endif

public:
	void UpdateObjects();
	void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PostNcDestroy();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On2Checkcollayer();
};
