#include "trainer.h"

/*
Trainer
by Evremonde
*/

// constructor
CTrainer::CTrainer()
{
	//
}

// deconstructor
CTrainer::~CTrainer()
{
	//
}

void CTrainer::writeBytes(DWORD address, int value, int bytes)
{
	::WriteProcessMemory(processHandle, (void*)address, &value, bytes, 0);
}

int CTrainer::readBytes(DWORD address, int bytes)
{
	int buffer = 0;
	::ReadProcessMemory(processHandle, (void*)address, &buffer, bytes, 0);
	return buffer;
}

void CTrainer::writeString(DWORD address, std::string value)
{
	int j = 0;
	for(int i = 0; i < value.size(); i++)
	{
		int k = address + j; // increment address
		::WriteProcessMemory(processHandle, (void*)k, &value[i], 255, 0);
		j++;
	}
}

std::string CTrainer::readString(DWORD address)
{ 
	static char buffer[255];
	::ReadProcessMemory(processHandle, (void*)address, &buffer, sizeof(buffer), 0);
	return buffer;
}

void CTrainer::writeNops(DWORD address, int nops)
{
	unsigned char nop[] = {0x90, 0x90, 0x90, 0x90};
	::WriteProcessMemory(processHandle, (void*)address, &nop, nops, 0);
}
