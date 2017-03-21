#pragma once
class CLed : public CWnd
{
	DECLARE_DYNAMIC(CLed)

public:
	CLed();
	virtual ~CLed();
	int color;
	int rojo, verde, ambar, gris;
	bool IsOn;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};