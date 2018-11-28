#include "stdafx.h"
#include "MySplitterWnd.h"

CMySplitterWnd::CMySplitterWnd()
	: m_iBorderX(1)
	, m_iBorderY(1)
	, m_iGapX(1)
	, m_iGapY(1)
{
}


CMySplitterWnd::~CMySplitterWnd()
{
}
BEGIN_MESSAGE_MAP(CMySplitterWnd, CSplitterWnd)
	ON_WM_CREATE()
	ON_WM_CREATE()
END_MESSAGE_MAP()


void CMySplitterWnd::SetBorderSize(int _iGapX, int _iGapY, int _iBorderX, int _iBorderY)
{
	m_iGapX = _iGapX;
	m_iGapY = _iGapY;
	m_iBorderX = _iBorderX;
	m_iBorderY = _iBorderY;
}



int CMySplitterWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  ���⿡ Ư��ȭ�� �ۼ� �ڵ带 �߰��մϴ�.

	m_cxBorder = m_iBorderX;
	m_cyBorder = m_iBorderY;
	m_cxSplitterGap = m_iGapX;
	m_cySplitterGap = m_iGapY;

	return 0;
}
