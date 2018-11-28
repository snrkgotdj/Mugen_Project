#pragma once

#include "afxext.h"

class CMySplitterWnd :
	public CSplitterWnd
{
private:
	int m_iGapX;
	int m_iGapY;
	int m_iBorderX;
	int m_iBorderY;

public:
	void SetBorderSize(int _iGapX, int _iGapY, int _iBorderX, int _iBorderY);

public:
	CMySplitterWnd();
	~CMySplitterWnd();
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

