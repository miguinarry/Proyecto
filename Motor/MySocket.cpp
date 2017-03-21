#include "stdafx.h"
#include "Motor.h"
#include "MySocket.h"
#include "MotorDlg.h"


CMySocket::CMySocket(CMotorDlg *p)
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
	int len = this->Receive(ReceivedBuffer, 12);

	// 
	unsigned int Length;
	Length = (0xFF & (unsigned int)ReceivedBuffer[10]) << 8;
	Length += 0xFF & (unsigned int)ReceivedBuffer[11];
	Length *= 2;
	Length += 9;
	unsigned char *SendBuffer = new unsigned char[Length];

	// Prepare answer with ModBus
	pDlg->MyModBus->SetServer();
	pDlg->MyModBus->ModBusReceived(ReceivedBuffer, SendBuffer);

	// Answer
	pDlg->AcceptedSocket->Send(&SendBuffer[0], Length);

	delete[] SendBuffer;
}