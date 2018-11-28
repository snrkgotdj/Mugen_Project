// ScriptDlg.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "ScriptDlg.h"
#include "afxdialogex.h"
#include "../External/ScriptManager.h"
#include "KeyManager.h"


// CScriptDlg ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_SCRIPTDLG, pParent)
	, m_bList(false)
{

}

CScriptDlg::~CScriptDlg()
{
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Control(pDX, IDC_COMBO1, m_cbScript);
}

void CScriptDlg::UpdateComponent(CGameObject * _pObj)
{
	m_pTarget = _pObj;

	if (CKeyManager::Manager().GetKeyState(KEY_DEL, STATE_DOWN))
	{
		DeleteScript();

		m_List.DeleteAllItems();

		list<Script*>& listScript = m_pTarget->GetScriptList();
		list<Script*>::iterator iter = listScript.begin();
		for (int i = 0; iter != listScript.end(); ++iter, ++i)
		{
			CString strScriptName = CScriptMgr::GetScriptName(*iter);
			m_List.InsertItem(i, strScriptName.GetBuffer());
		}
	}

	else if (NULL != m_pTarget)
	{
		UpdateList();
	}
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CScriptDlg::OnCbnSelchange)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, &CScriptDlg::OnNMSetfocusList1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST1, &CScriptDlg::OnNMKillfocusList1)
END_MESSAGE_MAP()


// CScriptDlg �޽��� ó�����Դϴ�.


void CScriptDlg::PostNcDestroy()
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CComponentDlg::PostNcDestroy();
}

void CScriptDlg::OnCbnSelchange()
{
	int iIdx = m_cbScript.GetCurSel();

	if (-1 == iIdx)
		return;

	CString str;
	DWORD dwTypeID = m_cbScript.GetItemData(iIdx);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_pTarget->AddComponent<Script>(CScriptMgr::GetScript(dwTypeID));

	UpdateList();
}


BOOL CScriptDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	vector<tScriptInfo> vecScriptName;
	CScriptMgr::GetAllScriptName(vecScriptName);

	for (UINT i = 0; i < vecScriptName.size(); ++i)
	{
		m_cbScript.InsertString(i, vecScriptName[i].strScriptName.c_str());
		m_cbScript.SetItemData(i, vecScriptName[i].dwTypeID);
	}
	
	return TRUE;  
}

void CScriptDlg::UpdateList()
{
	if (m_bList == true)
		return;


	m_List.DeleteAllItems();

	list<Script*>& listScript = m_pTarget->GetScriptList();
	list<Script*>::iterator iter = listScript.begin();
	for (int i = 0; iter != listScript.end(); ++iter, ++i)
	{
		CString strScriptName = CScriptMgr::GetScriptName(*iter);
		m_List.InsertItem(i, strScriptName.GetBuffer());
	}
}

void CScriptDlg::DeleteScript()
{
	if (m_bList == false)
		return;

	if (m_pTarget->GetScriptList().size() == 0)
		return;

	int iIdx = m_List.GetSelectionMark();

	m_pTarget->DeleteScript(iIdx);
}


void CScriptDlg::OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	m_bList = true;
}


void CScriptDlg::OnNMKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

	m_bList = false;
}
