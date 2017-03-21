#pragma once


// CGauge

class CGauge : public CWnd
{
	DECLARE_DYNAMIC(CGauge)

public:
	CGauge();
	virtual ~CGauge();
	void SetMaxValue(int maximo);
	int nivel, maximo; 

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


