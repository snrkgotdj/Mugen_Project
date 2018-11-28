#pragma once
#include "afxcmn.h"

// CListDlg ��ȭ �����Դϴ�.

enum class RESOURCE_TYPE
{
	MESH,
	MATERIAL,
	END,
};

class CListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDlg)

public:
	CListDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CListDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	RESOURCE_TYPE m_eResType;
	vector<CString> m_vecItemText;

public:
	void SetResType(RESOURCE_TYPE _eType) { m_eResType = _eType; }
	void InitListControl();
	

	virtual BOOL OnInitDialog();

	CListCtrl m_List;
	virtual void OnOK();
	virtual void OnCancel();
};
