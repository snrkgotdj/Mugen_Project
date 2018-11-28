#include "stdafx.h"
#include "TreeCtrlDlg.h"


CTreeCtrlDlg::CTreeCtrlDlg()
{
}

CTreeCtrlDlg::CTreeCtrlDlg(UINT nIDTemplate, CWnd * pParent)
	: CDialogEx(nIDTemplate, pParent)
{
}

CTreeCtrlDlg::~CTreeCtrlDlg()
{
}

HTREEITEM CTreeCtrlDlg::AddItem(const wstring & _strName, HTREEITEM _ParentItem, DWORD_PTR _dwData)
{
	TVINSERTSTRUCT tItem = {};

	tItem.hParent = _ParentItem;
	tItem.hInsertAfter = TVI_SORT;
	tItem.item.mask = TVIF_TEXT;
	tItem.item.iImage = 0;
	tItem.item.iSelectedImage = 0;
	tItem.item.pszText = const_cast<wchar_t*>(_strName.c_str());

	HTREEITEM hItem = m_Tree.InsertItem(&tItem);
	m_Tree.SetItemData(hItem, _dwData);

	return hItem;
}

void CTreeCtrlDlg::MoveItem(HTREEITEM _hSrcItem, HTREEITEM _hDestItem)
{
	TVITEMW item = {};

	wchar_t szName[255] = {};

	item.hItem = _hSrcItem;
	item.mask = TVIF_TEXT | TVIF_HANDLE;
	item.pszText = szName;
	item.cchTextMax = 255;

	m_Tree.GetItem(&item);
	DWORD_PTR dwData = m_Tree.GetItemData(_hSrcItem);

	HTREEITEM hCopyItem = AddItem(szName, _hDestItem, dwData);

	vector<HTREEITEM> vecChildren;
	GetChildren(_hSrcItem, vecChildren);

	for (UINT i = 0; i < vecChildren.size(); ++i)
	{
		MoveItem(vecChildren[i], hCopyItem);
	}

	if (item.state& TVIS_EXPANDED)
	{
		m_Tree.Expand(hCopyItem, TVIS_EXPANDED);
	}

	m_Tree.SelectItem(hCopyItem);
	m_Tree.DeleteItem(_hSrcItem);
}

void CTreeCtrlDlg::GetChildren(HTREEITEM _hParent, vector<HTREEITEM>& _vecOut)
{
	if (false == m_Tree.ItemHasChildren(_hParent))
		return;

	HTREEITEM hChild = m_Tree.GetChildItem(_hParent);

	while (true)
	{
		_vecOut.push_back(hChild);
		hChild = m_Tree.GetNextSiblingItem(hChild);

		if (NULL == hChild)
			break;
	}
}

void CTreeCtrlDlg::DeleteChildren(HTREEITEM _hParent)
{
	if (false == m_Tree.ItemHasChildren(_hParent))
		return;

	HTREEITEM hChild = m_Tree.GetChildItem(_hParent);
	HTREEITEM hNextChild = NULL;
	while (true)
	{
		hNextChild = m_Tree.GetNextSiblingItem(hChild);
		m_Tree.DeleteItem(hChild);
		hChild = hNextChild;

		if (NULL == hChild)
			break;
	}
}
