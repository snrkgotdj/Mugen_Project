// AddAnimDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "AddAnimDlg.h"
#include "afxdialogex.h"

#include "PathManager.h"
#include "GameObject.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"


// CAddAnimDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAddAnimDlg, CDialogEx)

CAddAnimDlg::CAddAnimDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDANIMDLG, pParent)
	, m_strTexName(_T(""))
	, m_strAnimKey(_T(""))
	, m_bContinue(FALSE)
{

}

CAddAnimDlg::~CAddAnimDlg()
{
}

void CAddAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strTexName);
	DDX_Text(pDX, IDC_EDIT2, m_strAnimKey);
	DDX_Check(pDX, IDC_CHECK1, m_bContinue);
	DDX_Control(pDX, IDC_EDIT3, m_editFrameCount1);
	DDX_Control(pDX, IDC_EDIT8, m_editFrameCount2);
	DDX_Control(pDX, IDC_EDIT4, m_editLT_X);
	DDX_Control(pDX, IDC_EDIT5, m_editLT_Y);
	DDX_Control(pDX, IDC_EDIT6, m_editSizeX);
	DDX_Control(pDX, IDC_EDIT7, m_editSizeY);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CAddAnimDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddAnimDlg::OnFolderBnClicked)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CAddAnimDlg::OnLvnItemchanged)
	ON_BN_CLICKED(IDC_CHECK1, &CAddAnimDlg::OnBnClickedCheck)
	ON_EN_CHANGE(IDC_EDIT2, &CAddAnimDlg::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CAddAnimDlg 메시지 처리기입니다.

int CALLBACK Proc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
		CString str = CPathManager::GetResPathEX();
		str += L"Texture\\Animation";
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)str.GetBuffer());
		break;
	}
	return 0;
}


void CAddAnimDlg::OnFolderBnClicked()
{
	BROWSEINFO br = {};
	br.lpfn = Proc;
	br.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	br.hwndOwner = m_hWnd;
	br.lpszTitle = L"폴더 선택";
	br.lParam = 0;

	LPITEMIDLIST pidl = NULL;
	if ((pidl = SHBrowseForFolder(&br)) != NULL)
	{
		wchar_t buffer[MAX_PATH];
		if (SHGetPathFromIDList(pidl, buffer))
			m_strFolderPath = buffer;
	}

	// 해당 폴더에 있는 Texture 파일 찾아서 목록에 추가
	WIN32_FIND_DATA tData = {};
	HANDLE hHandle = FindFirstFile(wstring(m_strFolderPath + L"\\*.*").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == hHandle)
	{
		MessageBox(NULL, L"존재하지 않는 폴더입니다.");
		assert(NULL);
	}

	CString strEXT;
	int iIdx = 0;
	do {
		strEXT = CPathManager::GetExt(tData.cFileName);
		if (strEXT != L".png" && strEXT != L".jpg" && strEXT != L".jpeg" && strEXT != L".bmp")
			continue;

		m_List.InsertItem(iIdx++, tData.cFileName);

	} while (FindNextFile(hHandle, &tData));

	CString strCount;
	strCount.Format(L"%d", iIdx);
	m_editFrameCount1.SetWindowTextW(strCount);
	FindClose(hHandle);
}


void CAddAnimDlg::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	int selected = 0;
	if (pos != NULL)
	{
		while (pos)
		{
			int nItem = m_List.GetNextSelectedItem(pos);
			selected = nItem + 1;
		}
	}

	m_strTexName = m_List.GetItemText(selected - 1, 0);
	UpdateData(false);

	*pResult = 0;
}


BOOL CAddAnimDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editFrameCount1.SetReadOnly(true);

	return TRUE;
}


void CAddAnimDlg::OnBnClickedCheck()
{
	if (m_bContinue)
		m_bContinue = false;

	else
		m_bContinue = true;

	UpdateData(false);

	if (m_bContinue)
	{
		m_editFrameCount1.SetReadOnly(false);

		m_editLT_X.SetReadOnly(true);
		m_editLT_Y.SetReadOnly(true);
		m_editSizeX.SetReadOnly(true);
		m_editSizeY.SetReadOnly(true);
		m_editFrameCount2.SetReadOnly(true);
	}
	else
	{
		m_editFrameCount1.SetReadOnly(true);

		m_editLT_X.SetReadOnly(false);
		m_editLT_Y.SetReadOnly(false);
		m_editSizeX.SetReadOnly(false);
		m_editSizeY.SetReadOnly(false);
		m_editFrameCount2.SetReadOnly(false);
	}
}


void CAddAnimDlg::OnOK()
{
	UpdateData(true);

	tDescInfo info = {};
	if (m_strTexName == L"")
	{
		MessageBox(L"시작 이미지를 선택하세요", L"이미지 선택 오류", MB_OK);
		return;
	}

	info.strTexName = m_strTexName;
	info.strAnimName = m_strAnimKey;
	info.iContinue = m_bContinue;

	CString str;
	if (m_bContinue)
	{
		m_editFrameCount1.GetWindowTextW(str);
		info.iFrameCount = _wtoi(str.GetBuffer());
	}
	else
	{
		m_editFrameCount2.GetWindowTextW(str);
		info.iFrameCount = (int)_wtoi(str.GetBuffer());

		m_editLT_X.GetWindowTextW(str);
		info.vLeftTop.x = (float)_wtoi(str.GetBuffer());

		m_editLT_Y.GetWindowTextW(str);
		info.vLeftTop.y = (float)_wtoi(str.GetBuffer());

		m_editSizeX.GetWindowTextW(str);
		info.vSize.x = (float)_wtoi(str.GetBuffer());

		m_editSizeY.GetWindowTextW(str);
		info.vSize.y = (float)_wtoi(str.GetBuffer());
	}

	m_pTarget->GetAnimatorEX()->LoadAnimation(&info, m_strFolderPath.GetBuffer());

	CDialogEx::OnOK();
}


void CAddAnimDlg::OnEnChangeEdit2()
{
	UpdateData(TRUE);
}


