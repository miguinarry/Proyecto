#pragma once
#include "MyMemory.h"

/*
Define constants
*/
// type => client or server
#define CLIENT 0
#define SERVER 1
// error codes for received message
#define RESULT_OK 0
#define ERROR_PROTOCOLID 1
#define ERROR_LENGTH 2
#define ERROR_FUNCTIONCODE 3
#define ERROR_NOTFOUND 4

class ModBus
{
public:
	unsigned int TransactionID, ProtocolID, UnitID;
	bool type; // SERVER or client
	CMyMemory *Memory;

	ModBus(CMyMemory *Memory);
	~ModBus();

	//  Set device type
	void SetClient();
	void SetServer();

	// Prepare data to make a request
	void SendWrite(unsigned char *Buffer, unsigned int Address, unsigned int Value);
	void SendRead(unsigned char *Buffer, unsigned int Address, unsigned int NumberOfRegisters);

	// ModBus msg received
	int ModBusReceived(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);

	// Response
	void ReceiveWrite(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);
	void ReceiveRead(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);
};

