#include "stdafx.h"
#include "MyMemory.h"


CMyMemory::CMyMemory()
{
}
CMyMemory::~CMyMemory()
{
}

void CMyMemory::Reset(){
	// temperatura
	Registers[0].Address = 400;
	Registers[0].Value = 0;
	// rpm
	Registers[1].Address = 401;
	Registers[1].Value = 0;
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