#include "stdafx.h"
#include "Centralita.h"
#include "MySocket.h"
#include "CentralitaDlg.h"


CMySocket::CMySocket(CCentralitaDlg *p)
{
	pDlg = p;
}
CMySocket::CMySocket()
{
}
CMySocket::~CMySocket()
{
}

void CMySocket::OnReceive(int nError){
	unsigned char *ReceivedBuffer;
	int error, len;	

	switch (ID)
	{
	case TARGET_ACCIONAMIENTOS:
		ReceivedBuffer = new unsigned char[pDlg->ModBusAccionamientos->Storage];		// Set the buffer size with the requested data
		len = this->Receive(ReceivedBuffer, pDlg->ModBusAccionamientos->Storage);		// Receive data
		error = pDlg->ModBusAccionamientos->ModBusReceived(ReceivedBuffer, nullptr);	// Process the modbus data
		break;
	case TARGET_MOTOR:
		ReceivedBuffer = new unsigned char[pDlg->ModBusMotor->Storage];
		len = this->Receive(ReceivedBuffer, pDlg->ModBusMotor->Storage);
		error = pDlg->ModBusMotor->ModBusReceived(ReceivedBuffer, nullptr);
		break;
	case TARGET_LUCES:
		ReceivedBuffer = new unsigned char[pDlg->ModBusLuces->Storage];
		len = this->Receive(ReceivedBuffer, pDlg->ModBusLuces->Storage);
		error = pDlg->ModBusLuces->ModBusReceived(ReceivedBuffer, nullptr);
		break;
	default:
		break;
	}
	ShowErrorMSG(error, ID);
	
	
	// Update memory
	pDlg->UpdateValuesFromMemory();
	delete[] ReceivedBuffer;
}

void CMySocket::ShowErrorMSG(int nError, int ID){
	// Comprobar mensajes de error
	CString cs;
	char *name = new char[14];

	// Get ID
	switch (ID)
	{
	case 1:
		name = "Acionamientos";
		break;
	case 2:
		name = "Luces";
		break;
	case 0:
		name = "Motor";
		break;
	default:
		break;
	}

	// Show msg
	switch (nError)
	{
	case ERROR_PROTOCOLID:
		cs.Format("=> ERROR en el ID del Protocolo. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	case ERROR_LENGTH:
		cs.Format("=> ERROR en La longitud recibida. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	case ERROR_FUNCTIONCODE:
		cs.Format("=> ERROR en el Código de Función. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	case ERROR_TRANSACTIONID:
		cs.Format("=> ERROR en el ID de transacción. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	case ERROR_WRITE:
		cs.Format("=> ERROR en escritura. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	case ERROR_NOTFOUND:
		cs.Format("=> ERROR sin identificar. Target = %s", name);
		pDlg->ShowLog(cs);
		return;
	}
}


void CMySocket::OnClose(int nError){
	this->IsConnected = false;
	this->Close();
	pDlg->SetTimer(ID_TIMER_RECONNECT, 50, NULL);
}