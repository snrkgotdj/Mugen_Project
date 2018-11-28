#pragma once
#include "afxwin.h"


class CGameObject;
class CTransformDlg;
class CComponentDlg;

// CComponentView form view

class CComponentView : public CFormView
{
	DECLARE_DYNCREATE(CComponentView)

protected:
	CComponentView();           // protected constructor used by dynamic creation
	virtual ~CComponentView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPONENTVIEW };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:
	CGameObject* m_pTargetObj;
	CComponentDlg* m_arrComDlg[(UINT)COMPONENT_TYPE::END];

private:
	bool m_bAC;

public:
	void SetTargetObj(CGameObject* _pObj) { m_pTargetObj = _pObj; }
	void InitTarget();
public:
	void UpdateComponent(CGameObject* _pObj);
	void Update();
	afx_msg void OnDestroy();
	CButton m_bnAC;
	CComboBox m_cbAC;
	afx_msg void OnBnAddComponent();
	afx_msg void OnCbnSelchange();
	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbnSetfocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};


