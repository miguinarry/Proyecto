#pragma once
#include "MyMemory.h"
/*
Define constants
*/
// type => client or server
#define CLIENT 0
#define SERVER 1
// type => client or server
#define TARGET_MOTOR 0
#define TARGET_ACCIONAMIENTOS 1
#define TARGET_LUCES 2
// error codes for received message
#define RESULT_OK 0
#define ERROR_PROTOCOLID 1
#define ERROR_LENGTH 2
#define ERROR_FUNCTIONCODE 3
#define ERROR_WRITE 4
#define ERROR_TRANSACTIONID 5
#define ERROR_NOTFOUND 6

class ModBus
{
public:
	unsigned int TransactionID, ProtocolID, UnitID, Address, Storage;
	bool type; // Server or CLIENT
	int Target; // Defines the target of the request
	CMyMemory *Memory;

	ModBus(CMyMemory *Memory);
	~ModBus();

	// Set device type
	void SetClient();
	void SetServer();
	// Set target
	void SetTarget(int Target);

	// Prepare data to make a request
	void SendWrite(unsigned char *Buffer, unsigned int Address, unsigned int Value);
	void SendRead(unsigned char *Buffer, unsigned int Address, unsigned int NumberOfRegisters);

	// ModBus msg received
	int ModBusReceived(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);

	// Response
	void ReceiveWrite(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);
	void ReceiveRead(unsigned char *ReceivedBuffer, unsigned char *SendBuffer);
};

