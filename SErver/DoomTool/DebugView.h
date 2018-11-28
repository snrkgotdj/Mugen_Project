#pragma once
#include "afxwin.h"

class CComponentDlg;
class CGameObject;
class CAnimationEX;
class CEditAnimDlg;

// CDebugView form view

class CDebugView : public CFormView
{
	DECLARE_DYNCREATE(CDebugView)

protected:
	CDebugView();           // protected constructor used by dynamic creation
	virtual ~CDebugView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEBUGVIEW };
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
	void Update();

private:
	CGameObject* m_pTarget;
	CEditAnimDlg* m_pEditAnimDlg;
	
public:
	void SetEditAnim(CAnimationEX* _pAnim);
	void SetMousePos(CPoint* _point);

public:
	CEditAnimDlg* GetEditAnimDlg() { return m_pEditAnimDlg; }

public:
	void SetTargetObj(CGameObject* _pObj) { m_pTarget = _pObj; }
	void InitTarget();
	void UpdateComponent(CGameObject* _pObj);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

