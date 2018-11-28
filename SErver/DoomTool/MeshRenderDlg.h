#pragma once

#include "ComponentDlg.h"
#include "afxwin.h"

// CMeshRenderDlg ��ȭ �����Դϴ�.

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CMeshRenderDlg();

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDERDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

protected:
	virtual void UpdateComponent(CGameObject* _pObj);

public:
	CEdit m_editMesh;
	CEdit m_editMtrl;
	CComboBox m_cbRS;

	bool m_bFocus_rs;

	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSetfocusCbrs();
	afx_msg void OnCbnKillfocusCbrs();
	afx_msg void OnCbnEditchangeCbrs();
	afx_msg void OnSelMeshBnClicked();
	afx_msg void OnSelMaterialBnClicked();
};
