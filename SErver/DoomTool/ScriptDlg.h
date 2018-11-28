#pragma once


// CScriptDlg ��ȭ �����Դϴ�.
#include "ComponentDlg.h"
#include "afxwin.h"
#include "afxcmn.h"

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CScriptDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual void UpdateComponent(CGameObject* _pObj);

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
private:
	bool m_bList;

public:
	virtual BOOL OnInitDialog();
	CListCtrl m_List;
	CComboBox m_cbScript;
	afx_msg void OnCbnSelchange();
	void UpdateList();
	void DeleteScript();
	afx_msg void OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
};
