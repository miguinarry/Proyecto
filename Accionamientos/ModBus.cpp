#include "stdafx.h"
#include "ModBus.h"

ModBus::ModBus(CMyMemory *Memory)
{
	this->Memory = Memory;
	TransactionID = 0x00;
	ProtocolID = 0x00;
	UnitID = 0xFF;
}


ModBus::~ModBus()
{
}


void ModBus::SetClient(){ type = CLIENT; }
void ModBus::SetServer(){ type = SERVER; }


void ModBus::SendWrite(unsigned char *Buffer, unsigned int Address, unsigned int Value){
	/*
	Esta funcion prepara un char mediante ModBus para enviarlo a través de TCP/IP y escribir un dato
	Los parametros de entrada son el valor a escribir y su dirección
	*/
	/*const int BuffLength = 12;
	Buffer = new unsigned char[BuffLength];*/
	// Transaction ID
	Buffer[0] = (TransactionID >> 8) & 0xFF;
	Buffer[1] = TransactionID & 0xFF;
	// Protocol ID => ModBus=0
	Buffer[2] = (ProtocolID >> 8) & 0xFF;
	Buffer[3] = ProtocolID & 0xFF;
	// Length 
	Buffer[4] = 0x00;
	Buffer[5] = 0x06;
	// Unit ID
	Buffer[6] = UnitID & 0xFF;
	// Function Code => write=6
	Buffer[7] = 0x06;
	// Address
	Buffer[8] = (Address >> 8) & 0xFF;
	Buffer[9] = Address & 0xFF;
	// Value to write
	Buffer[10] = (Value >> 8) & 0xFF;
	Buffer[11] = Value & 0xFF;
}

void ModBus::SendRead(unsigned char *Buffer, unsigned int Address, unsigned int NumberOfRegisters){
	/*
	Esta funcion prepara un char mediante ModBus para enviarlo a través de TCP/IP y pedir datos
	Los parametros de entrada son la dirección donde están los datos y el número de registros que se quieren
	*/
	// Transaction ID
	Buffer[0] = (TransactionID >> 8) & 0xFF;
	Buffer[1] = TransactionID & 0xFF;
	// Protocol ID => ModBus=0
	Buffer[2] = (ProtocolID >> 8) & 0xFF;
	Buffer[3] = ProtocolID & 0xFF;
	// Length 
	Buffer[4] = 0x00;
	Buffer[5] = 0x06;
	// Unit ID
	Buffer[6] = UnitID & 0xFF;
	// Function Code => write=4
	Buffer[7] = 0x04;
	// Address
	Buffer[8] = (Address >> 8) & 0xFF;
	Buffer[9] = Address & 0xFF;
	// Number of Registers to read
	Buffer[10] = (NumberOfRegisters >> 8) & 0xFF;
	Buffer[11] = NumberOfRegisters & 0xFF;
}

int ModBus::ModBusReceived(unsigned char *ReceivedBuffer, unsigned char *SendBuffer){
	/*
	Esta función se llama cuando se recibe un paquete Modbus, y comprueba las cabeceras
	para ver que todo está correcto y llama a la función correspondiente de respuesta
	*/

	// Check ProtocolID
	unsigned int pID;
	pID = (0xFF & (unsigned int)ReceivedBuffer[2]) << 8;
	pID += 0xFF & (unsigned int)ReceivedBuffer[3];
	if (pID != ProtocolID)
		return ERROR_PROTOCOLID; //Show error message

	// Check TransactionID
	unsigned int ReceivedTransactionID;
	ReceivedTransactionID = (0xFF & (unsigned int)ReceivedBuffer[0]) << 8;
	ReceivedTransactionID += 0xFF & (unsigned int)ReceivedBuffer[1];
	TransactionID = ReceivedTransactionID + 1;

	// Check Length
	unsigned int Length;
	Length = (0xFF & (unsigned int)ReceivedBuffer[4]) << 8;
	Length += 0xFF & (unsigned int)ReceivedBuffer[5];
	if (Length != 0x06 && type == SERVER)
		return ERROR_LENGTH; 

	// Manage the answer made in the server
	if (type == SERVER){
		// Check Function Code
		if (ReceivedBuffer[7] == 0x04){ // Read request
			ReceiveRead(ReceivedBuffer, SendBuffer);
			return RESULT_OK;

		}
		else if (ReceivedBuffer[7] == 0x06){ // Write request
			ReceiveWrite(ReceivedBuffer, SendBuffer);
			return RESULT_OK;
		}
		else{
			return ERROR_FUNCTIONCODE; // Show error message
		}
	}
	return ERROR_NOTFOUND;
}

void ModBus::ReceiveWrite(unsigned char *ReceivedBuffer, unsigned char *SendBuffer){
	/*
	Si se ha recibido una petición adecuada de escritura, se actualizan los valores
	y se responde con el resultado
	*/

	// Get address and value
	unsigned int Address, Value;
	Address = (0xFF & (unsigned int)ReceivedBuffer[8]) << 8;
	Address += 0xFF & (unsigned int)ReceivedBuffer[9];
	Value = (0xFF & (unsigned int)ReceivedBuffer[10]) << 8;
	Value += 0xFF & (unsigned int)ReceivedBuffer[11];

	// Update data in memory
	for (int i = 0; i < NUMBER_REGISTERS; i++){
		if (Address == Memory->Registers[i].Address){
			Memory->Registers[i].Value = Value;
			break;
		}
	}

	// Get value from address to check (in "Centralita") wether the data is correctly writen or not 
	for (int i = 0; i < NUMBER_REGISTERS; i++){
		if (Address == Memory->Registers[i].Address){
			Value = Memory->Registers[i].Value; break;
		}
	}
	// Answer
	SendWrite(&SendBuffer[0], Address, Value);
}

void ModBus::ReceiveRead(unsigned char *ReceivedBuffer, unsigned char *SendBuffer){
	/*
	Si se ha recibido una petición adecuada de lectura, se devuelven los valores
	*/

	// Get address and amount of registers
	unsigned int Address, Amount;
	Address = (0xFF & (unsigned int)ReceivedBuffer[8]) << 8;
	Address += 0xFF & (unsigned int)ReceivedBuffer[9];
	Amount = (0xFF & (unsigned int)ReceivedBuffer[10]) << 8;
	Amount += 0xFF & (unsigned int)ReceivedBuffer[11];
	Amount *= 2;
	unsigned int Length = 3 + Amount;

	// Prepare msg to send
	// Transaction ID
	SendBuffer[0] = (TransactionID >> 8) & 0xFF;
	SendBuffer[1] = TransactionID & 0xFF;
	// Protocol ID => ModBus=0
	SendBuffer[2] = (ProtocolID >> 8) & 0xFF;
	SendBuffer[3] = ProtocolID & 0xFF;
	// Length 
	SendBuffer[4] = (Length >> 8) & 0xFF;
	SendBuffer[5] = Length & 0xFF;
	// Unit ID
	SendBuffer[6] = UnitID & 0xFF;
	// Function Code => write=4
	SendBuffer[7] = 0x04;
	// Byte Count
	SendBuffer[8] = Amount & 0xFF;
	// Registers to read = amount
	unsigned int data;
	for (unsigned int i = 0; i < Amount; i += 2){
		// get data from memory;
		for (int j = 0; j < NUMBER_REGISTERS; j++){
			if (Memory->Registers[j].Address == Address + i/2){
				data = Memory->Registers[j].Value;
				break;
			}
			else{
				data = 0;
			}
		}
		SendBuffer[9 + i] = (data >> 8) & 0xFF;
		SendBuffer[10 + i] = data & 0xFF;
	}
}