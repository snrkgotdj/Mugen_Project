#pragma once

#include "ComponentDlg.h"
#include "afxwin.h"
#include "afxcmn.h"
// CCameraDlg 대화 상자입니다.

class CCameraDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCameraDlg)

public:
	CCameraDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	bool m_bNear;
	bool m_bFar;
	bool m_bScale;

public:
	virtual void UpdateComponent(CGameObject* _pObj);
	virtual void PostNcDestroy();
	CEdit m_editFar;
	CEdit m_editNear;
	CEdit m_editScale;
	CSliderCtrl m_Slider;
	afx_msg void OnEnSetfocusNear();
	afx_msg void OnEnSetfocusFar();
	afx_msg void OnEnKillfocusNear();
	afx_msg void OnEnKillfocusFar();
	afx_msg void OnEnChangeNear();
	afx_msg void OnEnChangeFar();
	afx_msg void OnEnSetfocusScale();
	afx_msg void OnEnKillfocusScale();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedShootLayer();
};
