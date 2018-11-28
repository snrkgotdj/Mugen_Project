#pragma once
#include "afxdialogex.h"

class CTreeCtrlDlg :
	public CDialogEx
{
protected:
	CTreeCtrl m_Tree;

public:
	HTREEITEM AddItem(const wstring& _strName, HTREEITEM _ParentItem  = NULL, DWORD_PTR _dwData = 0);
	void MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem);
	void GetChildren(HTREEITEM _hParent, vector<HTREEITEM>& _vecOut);
	void DeleteChildren(HTREEITEM _hParent);

public:
	CTreeCtrlDlg();
	CTreeCtrlDlg(UINT nIDTemplate, CWnd *pParent = NULL);
	virtual ~CTreeCtrlDlg();
};

