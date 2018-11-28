#pragma once
#include "afxcmn.h"

class CScene;
class CGameObject;
// CChangeLayerDlg 대화 상자입니다.

class CChangeLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeLayerDlg)

public:
	CChangeLayerDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChangeLayerDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGELAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	CScene* m_pCurScene;
	CGameObject* m_pTarget;
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
