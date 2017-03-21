// Led.cpp : implementation file
//

#include "stdafx.h"
#include "Luces.h"
#include "Led.h"

// CLed
IMPLEMENT_DYNAMIC(CLed, CWnd)

CLed::CLed()
{
	IsOn = false;
	rojo = RGB(255, 0, 0);
	verde = RGB(0, 255, 0);
	ambar = RGB(255, 126, 0);
	gris = RGB(140, 140, 140);
	color = gris;
}
CLed::~CLed()
{
}


BEGIN_MESSAGE_MAP(CLed, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()
// CLed message handlers

void CLed::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect r;
	GetClientRect(r);
	dc.FillSolidRect(r, color);
}
