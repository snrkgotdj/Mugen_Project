#pragma once
#include "afxwin.h"


class CGameObject;
// CGameObjDlg 대화 상자입니다.

class CGameObjDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameObjDlg)

public:
	CGameObjDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGameObjDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAMEOBJDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	CGameObject* m_pTarget;

	CEdit m_editName;
	afx_msg void OnBnClickedOk();
};
