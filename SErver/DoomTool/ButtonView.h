#pragma once
#include "afxwin.h"


class CGameObject;
// CButtonView form view

class CButtonView : public CFormView
{
	DECLARE_DYNCREATE(CButtonView)

protected:
	CButtonView();           // protected constructor used by dynamic creation
	virtual ~CButtonView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BUTTONVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif
private:
	CGameObject* m_pToolCam;

public:
	void update();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	bool m_bPlayOn;

public:
	CButton m_Play;
	CButton m_Pause;
	CButton m_Stop;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedStop();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
};


