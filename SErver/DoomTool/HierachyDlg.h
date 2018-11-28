#pragma once

#include "TreeCtrlDlg.h"
#include "afxcmn.h"

// CHierachyDlg 대화 상자입니다.
class CGameObject;

class CHierachyDlg : public CTreeCtrlDlg
{
	DECLARE_DYNAMIC(CHierachyDlg)

public:
	CHierachyDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CHierachyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERACHYDLG };
#endif

private:
	HTREEITEM m_hDragItem;
	HTREEITEM m_hTargetItem;

public: 
	void UpdateObjects();
	void Initialize();

private:
	void AddGameObject(CGameObject* _pObj, HTREEITEM _ParentItem);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnTvnBegindragTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnAddprefab();
	afx_msg void OnAddgameobject();
	afx_msg void OnChangetag();
	afx_msg void OnChangelayer();
	afx_msg void On0Deletecolldier();
	afx_msg void On0Deletechild();
};
