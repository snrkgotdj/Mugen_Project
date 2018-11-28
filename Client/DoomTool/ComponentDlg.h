#pragma once
#include "afxdialogex.h"

#include "GameObject.h"

class CComponentDlg :
	public CDialogEx
{
protected:
	CGameObject* m_pTarget;

public:
	virtual void UpdateComponent(CGameObject* _pObj) = 0;
	void SetTarget(CGameObject* _pObj) { m_pTarget = _pObj; }

public:
	CComponentDlg();
	CComponentDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CComponentDlg();
	virtual void OnOK();
	virtual void OnCancel();
};

