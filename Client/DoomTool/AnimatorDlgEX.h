#pragma once

#include "ComponentDlg.h"
#include "afxwin.h"

struct tAnimPairEX;

// CAnimatorDlgEX ��ȭ �����Դϴ�.

class CAnimatorDlgEX : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimatorDlgEX)

public:
	CAnimatorDlgEX(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAnimatorDlgEX();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATORDLGEX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

private:
	bool m_bNewList;
	bool m_bFocusRC;

private:
	vector<tAnimPairEX> m_vecAnimInfo;
	virtual void UpdateComponent(CGameObject* _pObj);
	virtual void PostNcDestroy();

public:
	CComboBox m_Animation;
	BOOL m_bRepeatCheck;

	afx_msg void OnCbnSetfocusCombo1();
	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedEditAnim();
};
