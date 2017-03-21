#pragma once

#define NUMBER_REGISTERS 5 // temperatura, rpm
#define ERROR_404 -1;


class CMyMemory
{
public:
	struct Register{
		unsigned int Address;
		unsigned int Value;
	}Registers[NUMBER_REGISTERS];

	CMyMemory();
	~CMyMemory();

	void Reset();
	unsigned int Write(unsigned int Address, unsigned int Value);
	unsigned int Read(unsigned int Address);
};

