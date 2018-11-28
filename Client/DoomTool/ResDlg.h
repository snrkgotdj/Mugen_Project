#pragma once


// CResDlg 대화 상자입니다.
#include "TreeCtrlDlg.h"

class CResDlg : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CResDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	HTREEITEM	m_hMesh;
	HTREEITEM	m_hTexture;
	HTREEITEM   m_hPrefab;
	HTREEITEM   m_hMaterial;
	HTREEITEM   m_hClones;
	HTREEITEM   m_hSound;

public:
	void UpdateObjects();

	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
