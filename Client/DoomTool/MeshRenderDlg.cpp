// MeshRenderDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "MeshRenderDlg.h"
#include "afxdialogex.h"

#include "MeshRender.h"
#include "Mesh.h"
#include "Material.h"
#include "ListDlg.h"
#include "Mesh.h"
#include "Material.h"


// CMeshRenderDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_MESHRENDERDLG, pParent)
	, m_bFocus_rs(false)
{

}

CMeshRenderDlg::~CMeshRenderDlg()
{
}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editMesh);
	DDX_Control(pDX, IDC_EDIT2, m_editMtrl);
	DDX_Control(pDX, IDC_CBRS, m_cbRS);
}

void CMeshRenderDlg::UpdateComponent(CGameObject * _pObj)
{
	if (_pObj == NULL)
		return;

	m_pTarget = _pObj;
	CString str;

	CMesh* pMesh = m_pTarget->GetMeshRender()->GetMesh();
	if (NULL != pMesh)
	{
		str = pMesh->GetKey().c_str();
		m_editMesh.SetWindowTextW(str);
	}
	

	CMaterial* pMtrl = m_pTarget->GetMeshRender()->GetSharedMaterial();
	if (NULL != pMtrl)
	{
		str = pMtrl->GetKey().c_str();
		m_editMtrl.SetWindowTextW(str);
	}
	
	if (m_bFocus_rs == false)
	{
		m_cbRS.SetCurSel((UINT)m_pTarget->GetMeshRender()->GetRSType());
	}
}


BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_CBN_SETFOCUS(IDC_CBRS, &CMeshRenderDlg::OnCbnSetfocusCbrs)
	ON_CBN_KILLFOCUS(IDC_CBRS, &CMeshRenderDlg::OnCbnKillfocusCbrs)
	ON_CBN_SELCHANGE(IDC_CBRS, &CMeshRenderDlg::OnCbnEditchangeCbrs)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnSelMeshBnClicked)
	ON_BN_CLICKED(IDC_BUTTON4, &CMeshRenderDlg::OnSelMaterialBnClicked)
END_MESSAGE_MAP()


// CMeshRenderDlg 메시지 처리기입니다.


void CMeshRenderDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CComponentDlg::PostNcDestroy();
}


BOOL CMeshRenderDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	m_cbRS.InsertString(0, L"RS_FRONT");
	m_cbRS.InsertString(1, L"RS_BACK");
	m_cbRS.InsertString(2, L"RS_ALL");
	m_cbRS.InsertString(3, L"RS_WIREFRAME");

	return TRUE; 
}


void CMeshRenderDlg::OnCbnSetfocusCbrs()
{
	m_bFocus_rs = true;
}


void CMeshRenderDlg::OnCbnKillfocusCbrs()
{
	m_bFocus_rs = false;
}


void CMeshRenderDlg::OnCbnEditchangeCbrs()
{
	int iIdx = m_cbRS.GetCurSel();
	m_pTarget->GetMeshRender()->SetRSType((RS_TYPE)iIdx);
}


void CMeshRenderDlg::OnSelMeshBnClicked()
{
	CListDlg dlg;
	dlg.SetResType(RESOURCE_TYPE::MESH);

	CMesh* pMesh = (CMesh*)dlg.DoModal();

	if (NULL != pMesh)
		m_pTarget->GetMeshRender()->SetMesh(pMesh);
}


void CMeshRenderDlg::OnSelMaterialBnClicked()
{
	CListDlg dlg;
	dlg.SetResType(RESOURCE_TYPE::MATERIAL);

	CMaterial* pMtrl = (CMaterial*)dlg.DoModal();
	if (NULL != pMtrl)
		m_pTarget->GetMeshRender()->SetMaterial(pMtrl);
}
