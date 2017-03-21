#include "stdafx.h"
#include "Luces.h"
#include "MySocket.h"
#include "LucesDlg.h"


CMySocket::CMySocket(CLucesDlg *p)
{
	pDlg = p;
}
CMySocket::CMySocket()
{
}
CMySocket::~CMySocket()
{
}

void CMySocket::OnAccept(int nError){
	// Close the previous socket accepted
	pDlg->AcceptedSocket->Close();

	Accept(*pDlg->AcceptedSocket);
}

void CMySocket::OnReceive(int nError){
	// Receive request
	unsigned char ReceivedBuffer[12];
	unsigned char *SendBuffer = new unsigned char[12];
	int len = this->Receive(ReceivedBuffer, 12);	

	// Prepare answer with ModBus
	int Error = pDlg->MyModBus->ModBusReceived(ReceivedBuffer, SendBuffer);

	// Answer
	pDlg->AcceptedSocket->Send(&SendBuffer[0], 12);

	// Update values
	pDlg->UpdateValues();

	delete[] SendBuffer;
}