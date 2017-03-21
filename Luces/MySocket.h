#pragma once
#include "afxsock.h"
#include "ModBus.h"

class CLucesDlg;

class CMySocket :
	public CSocket
{
public:
	CLucesDlg *pDlg;

	CMySocket(CLucesDlg *p);
	CMySocket();
	~CMySocket();

	virtual void OnAccept(int nError);
	void OnReceive(int nError);
};