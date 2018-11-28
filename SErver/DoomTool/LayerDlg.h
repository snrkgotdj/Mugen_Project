#pragma once
#include "afxwin.h"


class CScene;
// CLayerDlg 대화 상자입니다.

class CLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLayerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	CEdit m_editLayerName;
	afx_msg void OnBnClickedOk();
};
