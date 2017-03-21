#pragma once
#include "afxsock.h"
#include "ModBus.h"

class CMotorDlg;

class CMySocket :
	public CSocket
{
public:
	CMotorDlg *pDlg;

	CMySocket(CMotorDlg *p);
	CMySocket();
	~CMySocket();

	virtual void OnAccept(int nError);
	void OnReceive(int nError);
};