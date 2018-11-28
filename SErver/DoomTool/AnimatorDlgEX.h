#pragma once

#include "ComponentDlg.h"
#include "afxwin.h"

struct tAnimPairEX;

// CAnimatorDlgEX 대화 상자입니다.

class CAnimatorDlgEX : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimatorDlgEX)

public:
	CAnimatorDlgEX(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimatorDlgEX();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATORDLGEX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
