#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CAddAnimDlg ��ȭ �����Դϴ�.

class CGameObject;

class CAddAnimDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddAnimDlg)

public:
	CAddAnimDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddAnimDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDANIMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CGameObject* m_pTarget;
	CString m_strFolderPath;

	CString m_strTexName;
	CString m_strAnimKey;
	BOOL m_bContinue;
	CEdit m_editFrameCount1;
	CEdit m_editFrameCount2;
	CEdit m_editLT_X;
	CEdit m_editLT_Y;
	CEdit m_editSizeX;
	CEdit m_editSizeY;
	
	afx_msg void OnFolderBnClicked();
	CListCtrl m_List;
	afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheck();
	virtual void OnOK();
	afx_msg void OnEnChangeEdit2();
};
