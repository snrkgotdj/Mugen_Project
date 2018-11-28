#pragma once
#include "afxcmn.h"

class CScene;
class CGameObject;
// CChangeLayerDlg ��ȭ �����Դϴ�.

class CChangeLayerDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChangeLayerDlg)

public:
	CChangeLayerDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CChangeLayerDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHANGELAYERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()


public:
	CScene* m_pCurScene;
	CGameObject* m_pTarget;
	CListCtrl m_LayerList;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};
