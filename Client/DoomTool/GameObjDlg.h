#pragma once
#include "afxwin.h"


class CGameObject;
// CGameObjDlg ��ȭ �����Դϴ�.

class CGameObjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameObjDlg)

public:
	CGameObjDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGameObjDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEOBJDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

public:
	CGameObject* m_pTarget;

	CEdit m_editName;
	afx_msg void OnBnClickedOk();
};
