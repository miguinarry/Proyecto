#pragma once
#include "afxsock.h"
#include "ModBus.h"

class CAccionamientosDlg;

class CMySocket :
	public CSocket
{
public:
	CAccionamientosDlg *pDlg;

	CMySocket(CAccionamientosDlg *p);
	CMySocket();
	~CMySocket();

	virtual void OnAccept(int nError);
	void OnReceive(int nError);
};