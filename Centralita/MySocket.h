#pragma once
#include "afxsock.h"
#include "ModBus.h"

class CCentralitaDlg;

class CMySocket :
	public CSocket
{
public:
	int ID, ReConnect = 0;
	bool IsConnected = false;
	CCentralitaDlg *pDlg;

	CMySocket(CCentralitaDlg *p);
	CMySocket();
	~CMySocket();

	void OnReceive(int nError);
	void OnClose(int nError);
	void ShowErrorMSG(int nError, int ID);
};

