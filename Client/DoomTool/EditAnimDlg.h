#pragma once

#include "ComponentDlg.h"
#include "afxwin.h"

class CAnimationEX;

// CEditAnimDlg 대화 상자입니다.

enum MOUSE_MODE
{
	MODE_UP,
	MODE_DOWN,
	MODE_HIT,
	MODE_END
};

class CEditAnimDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CEditAnimDlg)

public:
	CEditAnimDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEditAnimDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EDITANIMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	CAnimationEX* m_pEditAnim;

	MOUSE_MODE m_eMouseMode;
	CButton m_btnAllClip;
	CButton m_bMouseMode;
	
	CListBox m_MouseModeList;
	CListBox m_AnimList;
	
	Vec2 m_vecMouse;
	CEdit m_arrOffset[2][6];
	bool m_arrFocus[2][6];

	CEdit m_editTerm;
	bool m_bTermFocus;

	bool m_bMove[4];	
	CEdit m_editMoveX;
	CEdit m_editMoveY;
	CEdit m_editMoveSpeed;

public:
	void SetAnimation(CAnimationEX* _pAnim);
	void SetMousePos(Vec2& _point) { m_vecMouse = _point; }
	void SetOffsetByMouse(Vec2 _down, Vec2 _up);

public:
	virtual void UpdateComponent(CGameObject* _pObj);


	virtual void PostNcDestroy();
	
	afx_msg void OnLbnSelchangeList();
	afx_msg void OnLbnSelchangeMoseMode();
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedPause();
	
	afx_msg void OnEnKillfocusOffset(UINT _id);
	afx_msg void OnEnSetfocusOffset(UINT _id);
	afx_msg void OnEnChangeOffset(UINT _id);
	afx_msg void OnBnClickedMouseMode();
	afx_msg void OnEnChangeTerm();
	afx_msg void OnEnKillfocusTerm();
	afx_msg void OnEnSetfocusTerm();
	afx_msg void OnEnSetfocusMoveX();
	afx_msg void OnEnKillfocusMoveX();
	afx_msg void OnEnKillfocusMoveY();
	afx_msg void OnEnSetfocusMoveY();
	afx_msg void OnEnChangeMoveX();
	afx_msg void OnEnChangeMoveY();
	afx_msg void OnEnKillfocusMoveSpeed();
	afx_msg void OnEnSetfocusMoveSpeed();
	afx_msg void OnEnChangeMoveSpeed();
	CEdit m_editJumpSpeed;
	afx_msg void OnEnKillfocusJumpSpeed();
	afx_msg void OnEnSetfocusUJumpSpeed();
	afx_msg void OnEnChangeJumpSpeed();
};
