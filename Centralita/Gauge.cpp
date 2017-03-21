// Gauge.cpp : implementation file
//

#include "stdafx.h"
#include "Centralita.h"
#include "Gauge.h"


// CGauge

IMPLEMENT_DYNAMIC(CGauge, CWnd)

CGauge::CGauge()
{
	nivel = 0;
}

CGauge::~CGauge()
{
}


BEGIN_MESSAGE_MAP(CGauge, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CGauge message handlers




void CGauge::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
	CRect r;
	GetClientRect(r);

	//
	dc.FillSolidRect(r,RGB(240,240,240));

	//// draw semi circle
	CRgn Region;
	Region.CreateRectRgnIndirect(r);
	dc.SelectObject(&Region);
	dc.Ellipse(0, 0, r.right, 2*r.bottom);

	//// draw arrow
	int x, y, angle = 180 + 180 * nivel / maximo;
	CPen p1; // Set red color
	p1.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

	dc.SelectObject(&p1);
	dc.MoveTo(r.right / 2, r.bottom - 1);
	// Calculate final point with the angle
	y = (int)(r.bottom*sin(angle*3.141592 / 180));
	x = (int)(r.bottom*cos(angle*3.141592 / 180));
	dc.LineTo(x + r.right / 2, y + r.bottom - 1);
}

void CGauge::SetMaxValue(int maximo){
	this->maximo = maximo;
}
