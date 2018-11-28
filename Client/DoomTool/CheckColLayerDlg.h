#pragma once
#include "afxcmn.h"

class CScene;

// CCheckColLayerDlg 대화 상자입니다.

class CCheckColLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckColLayerDlg)

public:
	CCheckColLayerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCheckColLayerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKCOLLAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	wstring m_strLayerName;
	virtual BOOL OnInitDialog();
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
};
