#pragma once
#include "afxcmn.h"

class CScene;

// CCheckColLayerDlg ��ȭ �����Դϴ�.

class CCheckColLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckColLayerDlg)

public:
	CCheckColLayerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CCheckColLayerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECKCOLLAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	wstring m_strLayerName;
	virtual BOOL OnInitDialog();
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
};
