#pragma once
#include "ComponentDlg.h"
#include "Animator.h"

// CAnimatorDlg ��ȭ �����Դϴ�.

class CAnimation;

class CAnimatorDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimatorDlg)

public:
	CAnimatorDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimatorDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATORDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	vector<tAnimPair> m_vecAnimPair;

public:
	virtual void UpdateComponent(CGameObject* _pObj);

	virtual void PostNcDestroy();
	CComboBox m_cbAnim;
	bool m_bFocusAnim;
	BOOL m_bRepeat;
	afx_msg void OnCbnKillfocus();
	afx_msg void OnCbnSetfocus();
	afx_msg void OnCbnSelchange();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRepchkbox();

	afx_msg void OnEnChange(UINT _id);
	afx_msg void OnEnSetFocus(UINT _id);
	afx_msg void OnEnKillFocus(UINT _id);
	bool m_arrFocus[28];
	CEdit m_arrVal[28];

	CGameObject* m_pTarget;
	tAnimFrame m_tFrame;
	vector<tAnimFrame> m_vecFrame;
	afx_msg void OnBnClickedAddframe();
	afx_msg void OnBnClickedDelframe();
	afx_msg void OnBnClickedSaveclip();
	afx_msg void OnBnClickedLoadclip();

private:
	wstring m_strFileName;
	wstring m_strTexKey;

public:
	afx_msg void OnLbnSelchangeFramelist();
	afx_msg void OnLbnKillfocusFramelist();
	CListBox m_ListFrame;
	CString m_strNextClip;
	afx_msg void OnEnChangeEdit1();
	BOOL m_bAllFrame;
	afx_msg void OnBnClickedAllframe();
};
