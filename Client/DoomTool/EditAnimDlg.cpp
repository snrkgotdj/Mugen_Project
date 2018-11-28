// EditAnimDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DoomTool.h"
#include "EditAnimDlg.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "AnimatorEX.h"
#include "AnimationEX.h"
#include "TransForm.h"
#include "Texture.h"

#include "TimeManager.h"
#include "SceneManager.h"
#include "KeyManager.h"

// CEditAnimDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditAnimDlg, CDialogEx)

CEditAnimDlg::CEditAnimDlg(CWnd* pParent /*=NULL*/)
	: CComponentDlg(IDD_EDITANIMDLG, pParent)
	, m_pEditAnim(NULL)
	, m_arrFocus{}
	, m_bMove{}
	, m_eMouseMode(MODE_UP)
	, m_bTermFocus(false)
{
}

CEditAnimDlg::~CEditAnimDlg()
{
}

void CEditAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_AnimList);

	int iStartID = IDC_UPOFFSETX;

	for (int i = 0; i < 2; ++i)
	{
		for (int j = 0; j < 6; ++j)
		{
			DDX_Control(pDX, iStartID++, m_arrOffset[i][j]);
		}
	}
	DDX_Control(pDX, IDC_CHECK1, m_btnAllClip);
	DDX_Control(pDX, IDC_CHECK2, m_bMouseMode);
	DDX_Control(pDX, IDC_LIST1, m_MouseModeList);
	DDX_Control(pDX, IDC_TERM, m_editTerm);
	DDX_Control(pDX, IDC_UPOFFSETX2, m_editMoveX);
	DDX_Control(pDX, IDC_UPOFFSETY2, m_editMoveY);
	DDX_Control(pDX, IDC_UPOFFSETY3, m_editMoveSpeed);
	DDX_Control(pDX, IDC_UPOFFSETY4, m_editJumpSpeed);
}


BEGIN_MESSAGE_MAP(CEditAnimDlg, CDialogEx)
	ON_LBN_SELCHANGE(IDC_LIST2, &CEditAnimDlg::OnLbnSelchangeList)
	ON_BN_CLICKED(IDC_BUTTON1, &CEditAnimDlg::OnBnClickedPlay)
	ON_BN_CLICKED(IDC_BUTTON2, &CEditAnimDlg::OnBnClickedPause)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_UPOFFSETX, IDC_HITSCALEY, &CEditAnimDlg::OnEnKillfocusOffset)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_UPOFFSETX, IDC_HITSCALEY, &CEditAnimDlg::OnEnSetfocusOffset)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_UPOFFSETX, IDC_HITSCALEY, &CEditAnimDlg::OnEnChangeOffset)
	ON_BN_CLICKED(IDC_CHECK2, &CEditAnimDlg::OnBnClickedMouseMode)
	ON_LBN_SELCHANGE(IDC_LIST1, &CEditAnimDlg::OnLbnSelchangeMoseMode)
	ON_EN_CHANGE(IDC_TERM, &CEditAnimDlg::OnEnChangeTerm)
	ON_EN_KILLFOCUS(IDC_TERM, &CEditAnimDlg::OnEnKillfocusTerm)
	ON_EN_SETFOCUS(IDC_TERM, &CEditAnimDlg::OnEnSetfocusTerm)
	ON_EN_SETFOCUS(IDC_UPOFFSETX2, &CEditAnimDlg::OnEnSetfocusMoveX)
	ON_EN_KILLFOCUS(IDC_UPOFFSETX2, &CEditAnimDlg::OnEnKillfocusMoveX)
	ON_EN_KILLFOCUS(IDC_UPOFFSETY2, &CEditAnimDlg::OnEnKillfocusMoveY)
	ON_EN_SETFOCUS(IDC_UPOFFSETY2, &CEditAnimDlg::OnEnSetfocusMoveY)
	ON_EN_CHANGE(IDC_UPOFFSETX2, &CEditAnimDlg::OnEnChangeMoveX)
	ON_EN_CHANGE(IDC_UPOFFSETY2, &CEditAnimDlg::OnEnChangeMoveY)
	ON_EN_KILLFOCUS(IDC_UPOFFSETY3, &CEditAnimDlg::OnEnKillfocusMoveSpeed)
	ON_EN_SETFOCUS(IDC_UPOFFSETY3, &CEditAnimDlg::OnEnSetfocusMoveSpeed)
	ON_EN_CHANGE(IDC_UPOFFSETY3, &CEditAnimDlg::OnEnChangeMoveSpeed)
	ON_EN_KILLFOCUS(IDC_UPOFFSETY4, &CEditAnimDlg::OnEnKillfocusJumpSpeed)
	ON_EN_SETFOCUS(IDC_UPOFFSETY4, &CEditAnimDlg::OnEnSetfocusUJumpSpeed)
	ON_EN_CHANGE(IDC_UPOFFSETY4, &CEditAnimDlg::OnEnChangeJumpSpeed)
END_MESSAGE_MAP()


// CEditAnimDlg 메시지 처리기입니다.


void CEditAnimDlg::SetAnimation(CAnimationEX * _pAnim)
{
	if (NULL == _pAnim)
		return;

	m_AnimList.ResetContent();

	 m_pEditAnim = _pAnim; 

	 this->ShowWindow(true);
	 vector<tFrameInfo>& vecFrame = m_pEditAnim->GetVecFrame();
	 m_AnimList.ResetContent();

	 for (UINT i = 0; i < vecFrame.size(); ++i)
	 {
		 wstring str = vecFrame[i].pTex->GetKey();
		 m_AnimList.InsertString(i, str.c_str());
	 }
}

void CEditAnimDlg::SetOffsetByMouse(Vec2 _down, Vec2 _up)
{
	float a = m_pEditAnim->GetAnimatorEx()->GetGameObject()->GetTransform()->GetLocalPos().x;
	float Mx = (_down.x + _up.x) / 2.f;
	float size = m_pEditAnim->GetAnimatorEx()->GetGameObject()->GetTransform()->GetLocalScale().x;	
	
	// offset SizeX
	float MSizeX = (abs(_up.x - _down.x)) / size;
	// offset PosX
	float x = ((Mx - a) / size)* (1 / MSizeX);

	a = m_pEditAnim->GetAnimatorEx()->GetGameObject()->GetTransform()->GetLocalPos().y;
	Mx = (_down.y + _up.y) / 2.f;
	size = m_pEditAnim->GetAnimatorEx()->GetGameObject()->GetTransform()->GetLocalScale().y;
	
	// offset SizeY
	float MSizeY = (abs(_up.y - _down.y)) / size;
	// offset PosY
	float y = ((Mx - a) / size)* (1 / MSizeY);

	//UpCol일때
	switch (m_eMouseMode)
	{
	case MODE_UP:
		const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tUpCol = ColInfo{ Vec3(x,y,0),Vec3(MSizeX, MSizeY, 0) };
		if (m_btnAllClip.GetCheck())
			m_pEditAnim->ChangeAllClipUpColInfo(Vec3(x, y, 0), Vec3(MSizeX, MSizeY, 0));
		break;
	case MODE_DOWN:
		const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tDownCol = ColInfo{ Vec3(x,y,0),Vec3(MSizeX, MSizeY, 0) };
		if (m_btnAllClip.GetCheck())
			m_pEditAnim->ChangeAllClipDownColInfo(Vec3(x, y, 0), Vec3(MSizeX, MSizeY, 0));
		break;
	case MODE_HIT:
		const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tHitCol = ColInfo{ Vec3(x,y,0),Vec3(MSizeX, MSizeY, 0) };
		if (m_btnAllClip.GetCheck())
			m_pEditAnim->ChangeAllClipHitColInfo(Vec3(x, y, 0), Vec3(MSizeX, MSizeY, 0));
		break;
	}
}

void CEditAnimDlg::UpdateComponent(CGameObject * _pObj)
{
	if (NULL == _pObj || NULL == m_pEditAnim)
		return;

	m_pTarget = _pObj;
	
	CString str;

	//위쪽
	if (m_arrFocus[0][0] == false)
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tUpCol.vPos.x);
		m_arrOffset[0][0].SetWindowTextW(str);
	}
	
	if (m_arrFocus[0][1] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tUpCol.vPos.y);
	m_arrOffset[0][1].SetWindowTextW(str);
	}

	if (m_arrFocus[0][2] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tDownCol.vPos.x);
	m_arrOffset[0][2].SetWindowTextW(str);
	}

	if (m_arrFocus[0][3] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tDownCol.vPos.y);
	m_arrOffset[0][3].SetWindowTextW(str);
	}

	if (m_arrFocus[0][4] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tHitCol.vPos.x);
	m_arrOffset[0][4].SetWindowTextW(str);
	}

	if (m_arrFocus[0][5] == false)
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tHitCol.vPos.y);
		m_arrOffset[0][5].SetWindowTextW(str);
	}
	//아래쪽

	if (m_arrFocus[1][0] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tUpCol.vSize.x);
	m_arrOffset[1][0].SetWindowTextW(str);
	}

	if (m_arrFocus[1][1] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tUpCol.vSize.y);
	m_arrOffset[1][1].SetWindowTextW(str);
	}

	if (m_arrFocus[1][2] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tDownCol.vSize.x);
	m_arrOffset[1][2].SetWindowTextW(str);
	}

	if (m_arrFocus[1][3] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tDownCol.vSize.y);
	m_arrOffset[1][3].SetWindowTextW(str);
	}

	if (m_arrFocus[1][4] == false)
	{
	str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tHitCol.vSize.x);
	m_arrOffset[1][4].SetWindowTextW(str);
	}

	if (m_arrFocus[1][5] == false)
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tHitCol.vSize.y);
		m_arrOffset[1][5].SetWindowTextW(str);
	}

	if (m_bTermFocus == false)
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->fTerm);
		m_editTerm.SetWindowTextW(str);
	}

	//무브
	if (m_bMove[0] == false)
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tMove.x);
		m_editMoveX.SetWindowTextW(str);
	}

	if (false == m_bMove[1])
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->tMove.y);
		m_editMoveY.SetWindowTextW(str);
	}

	if (false == m_bMove[2])
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->fSpeed);
		m_editMoveSpeed.SetWindowTextW(str);
	}

	if (false == m_bMove[3])
	{
		str.Format(L"%f", m_pEditAnim->GetCurFrameInfo()->fJumpSpeed);
		m_editJumpSpeed.SetWindowTextW(str);
	}

	//마우스 모드 선택
	if (CKeyManager::Manager().GetKeyState(KEY_1, STATE_DOWN))	
		m_eMouseMode = MODE_UP;
	
	else if (CKeyManager::Manager().GetKeyState(KEY_2, STATE_DOWN))
		m_eMouseMode = MODE_DOWN;
	
	else if (CKeyManager::Manager().GetKeyState(KEY_3, STATE_DOWN))
		m_eMouseMode = MODE_HIT;
}

void CEditAnimDlg::PostNcDestroy()
{
	delete this;

	CComponentDlg::PostNcDestroy();
}


void CEditAnimDlg::OnLbnSelchangeList()
{
	int idx = m_AnimList.GetCurSel();

	m_pTarget->GetAnimatorEX()->GetCurAnim()->SetFrameIndex(idx);
}


void CEditAnimDlg::OnBnClickedPlay()
{
	CSceneManager::Manager().SetPlayMode(true);
}


void CEditAnimDlg::OnBnClickedPause()
{
	CSceneManager::Manager().SetPlayMode(false);
}


void CEditAnimDlg::OnEnKillfocusOffset(UINT _id)
{
	int iIdx = _id - IDC_UPOFFSETX;
	m_arrFocus[iIdx / 6][iIdx % 6] = false;
}


void CEditAnimDlg::OnEnSetfocusOffset(UINT _id)
{
	int iIdx = _id - IDC_UPOFFSETX;
	m_arrFocus[iIdx / 6][iIdx % 6] = true;
}


void CEditAnimDlg::OnEnChangeOffset(UINT _id)
{
	int id = _id - IDC_UPOFFSETX;

	if (false == m_arrFocus[id / 6][id % 6])
		return;

	CString str;
	m_arrOffset[id / 6][id % 6].GetWindowTextW(str);

	float fOffset = (float)_wtof(str.GetBuffer());

	if (id / 6 == 0)
	{
		id = id % 6;
		switch (id)
		{
		case 0:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tUpCol.vPos.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipUpColPos(fOffset, true);
			
			break;
		case 1:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tUpCol.vPos.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipUpColPos(fOffset, false);

			break;
		case 2:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tDownCol.vPos.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipDownColPos(fOffset, true);

			break;
		case 3:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tDownCol.vPos.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipDownColPos(fOffset, false);

			break;
		case 4:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tHitCol.vPos.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipHitColPos(fOffset, true);

			break; 
		case 5:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tHitCol.vPos.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipHitColPos(fOffset, false);

			break;
		}
	}

	else if (id / 6 == 1)
	{
		id = id % 6;
		switch (id)
		{
		case 0:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tUpCol.vSize.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipUpColSize(fOffset, true);

			break;
		case 1:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tUpCol.vSize.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipUpColSize(fOffset, false);

			break;
		case 2:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tDownCol.vSize.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipDownColSize(fOffset, true);

			break;
		case 3:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tDownCol.vSize.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipDownColSize(fOffset, false);

			break;
		case 4:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tHitCol.vSize.x = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipHitColSize(fOffset, true);

			break;
		case 5:
			const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tHitCol.vSize.y = fOffset;
			if (m_btnAllClip.GetCheck())
				m_pEditAnim->ChangeAllClipHitColSize(fOffset, false);

			break;
		}
	}
}

#include "GameView.h"
void CEditAnimDlg::OnBnClickedMouseMode()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();

	if (m_bMouseMode.GetCheck() == true)
	{
		((CGameView*)pMainWnd->GetGameView())->SetEditMode(true);
		m_MouseModeList.InsertString(0, L"UpCol");
		m_MouseModeList.InsertString(1, L"DownCol");
		m_MouseModeList.InsertString(2, L"HitCol");
	}

	else if (m_bMouseMode.GetCheck() == false)
	{
		((CGameView*)pMainWnd->GetGameView())->SetEditMode(false);
		m_MouseModeList.ResetContent();
	}
}


void CEditAnimDlg::OnLbnSelchangeMoseMode()
{
	int Index = m_MouseModeList.GetCurSel();

	if (Index == 0)
		m_eMouseMode = MODE_UP;

	else if (Index == 1)
		m_eMouseMode = MODE_DOWN;

	else if (Index == 2)
		m_eMouseMode = MODE_HIT;
}


void CEditAnimDlg::OnEnChangeTerm()
{
	if (m_bTermFocus == false)
		return;

	CString str;
	m_editTerm.GetWindowTextW(str);
	float fTerm = (float)_wtof(str.GetBuffer());

	const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->fTerm = fTerm;
	if (m_btnAllClip.GetCheck())
		m_pEditAnim->ChangeAllClipTerm(fTerm);
	
}

void CEditAnimDlg::OnEnSetfocusTerm()
{
	m_bTermFocus = true;
}

void CEditAnimDlg::OnEnKillfocusTerm()
{
	m_bTermFocus = false;
}

void CEditAnimDlg::OnEnSetfocusMoveX()
{
	m_bMove[0] = true;
}

void CEditAnimDlg::OnEnKillfocusMoveX()
{
	m_bMove[0] = false;
}

void CEditAnimDlg::OnEnSetfocusMoveY()
{
	m_bMove[1] = true;
}

void CEditAnimDlg::OnEnKillfocusMoveY()
{
	m_bMove[1] = false;
}

void CEditAnimDlg::OnEnSetfocusMoveSpeed()
{
	m_bMove[2] = true;
}

void CEditAnimDlg::OnEnKillfocusMoveSpeed()
{
	m_bMove[2] = false;
}

void CEditAnimDlg::OnEnSetfocusUJumpSpeed()
{
	m_bMove[3] = true;
}

void CEditAnimDlg::OnEnKillfocusJumpSpeed()
{
	m_bMove[3] = false;
}


void CEditAnimDlg::OnEnChangeMoveX()
{
	if (m_bMove[0] == false)
		return;

	CString str;
	m_editMoveX.GetWindowTextW(str);
	float MoveX = (float)_wtof(str.GetBuffer());

	const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tMove.x = MoveX;
}


void CEditAnimDlg::OnEnChangeMoveY()
{
	if (m_bMove[1] == false)
		return;

	CString str;
	m_editMoveY.GetWindowTextW(str);
	float MoveY = (float)_wtof(str.GetBuffer());

	const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->tMove.y = MoveY;
}

void CEditAnimDlg::OnEnChangeMoveSpeed()
{
	if (m_bMove[2] == false)
		return;

	CString str;
	m_editMoveSpeed.GetWindowTextW(str);
	float fSpeed = (float)_wtof(str.GetBuffer());

	const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->fSpeed = fSpeed;
}

void CEditAnimDlg::OnEnChangeJumpSpeed()
{
	if (m_bMove[3] == false)
		return;

	CString str;
	m_editJumpSpeed.GetWindowTextW(str);
	float fSpeed = (float)_wtof(str.GetBuffer());

	const_cast<tFrameInfo*>(m_pEditAnim->GetCurFrameInfo())->fJumpSpeed = fSpeed;
}
