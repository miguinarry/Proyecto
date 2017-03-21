#include "stdafx.h"
#include "MyMemory.h"


CMyMemory::CMyMemory()
{
}
CMyMemory::~CMyMemory()
{
}

void CMyMemory::Reset(){
	// Freno
	Registers[0].Address = 500;
	Registers[0].Value = 0;
	// Led Delante Izquierda
	Registers[1].Address = 501;
	Registers[1].Value = 0;
	// Led Delante Derecha
	Registers[2].Address = 502;
	Registers[2].Value = 0;
	// Led Atras Izquierda
	Registers[3].Address = 503;
	Registers[3].Value = 0;
	// Led Atras Derecha
	Registers[4].Address = 504;
	Registers[4].Value = 0;
}

unsigned int CMyMemory::Write(unsigned int Address, unsigned int Value){
	for (int i = 0; i < NUMBER_REGISTERS; i++){
		if (Address == Registers[i].Address){
			Registers[i].Value = Value;
			return Registers[i].Value;
		}
	}
	return ERROR_404;
}

unsigned int CMyMemory::Read(unsigned int Address){
	for (int i = 0; i < NUMBER_REGISTERS; i++){
		if (Address == Registers[i].Address)
			return Registers[i].Value;
	}
	return ERROR_404;
}