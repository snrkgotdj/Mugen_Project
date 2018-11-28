#pragma once


// CLayerDialog ��ȭ �����Դϴ�.
#include "TreeCtrlDlg.h"
#include "afxcmn.h"

class CLayerDialog : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CLayerDialog)

public:
	CLayerDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLayerDialog();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERDIALOG };
#endif

public:
	void UpdateObjects();
	void Initialize();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PostNcDestroy();
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void On2Checkcollayer();
};
