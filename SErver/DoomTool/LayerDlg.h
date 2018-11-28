#pragma once
#include "afxwin.h"


class CScene;
// CLayerDlg ��ȭ �����Դϴ�.

class CLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLayerDlg)

public:
	CLayerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CLayerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	CEdit m_editLayerName;
	afx_msg void OnBnClickedOk();
};
