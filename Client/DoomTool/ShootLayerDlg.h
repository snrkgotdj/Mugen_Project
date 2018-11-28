#pragma once
#include "afxcmn.h"

class CScene;
// CShootLayerDlg ��ȭ �����Դϴ�.

class CShootLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShootLayerDlg)

public:
	CShootLayerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CShootLayerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SHOOTLAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CScene* m_pCurScene;
	virtual BOOL OnInitDialog();
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
};
