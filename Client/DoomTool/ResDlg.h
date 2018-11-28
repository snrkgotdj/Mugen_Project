#pragma once


// CResDlg ��ȭ �����Դϴ�.
#include "TreeCtrlDlg.h"

class CResDlg : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CResDlg)

public:
	CResDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CResDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RESDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
