#pragma once
#include "afxcmn.h"

// CListDlg 대화 상자입니다.

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
	CListDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CListDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
