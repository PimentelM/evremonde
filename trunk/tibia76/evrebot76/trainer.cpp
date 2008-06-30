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
	WriteProcessMemory(processHandle, (void*)address, &value, bytes, NULL);
}

int CTrainer::readBytes(DWORD address, int bytes)
{
	int buffer = 0;
	ReadProcessMemory(processHandle, (void*)address, &buffer, bytes, NULL);
	return buffer;
}

void CTrainer::writeString(DWORD address, char *value)
{
	int j = 0;
	for(int i = 0; i < strlen(value); i++)
	{
		int k = address + j; // increment address
		WriteProcessMemory(processHandle, (void*)k, &value[i], 2, NULL);
		j++;
	}
}

char *CTrainer::readString(DWORD address)
{ 
	char buffer[255];
	ReadProcessMemory(processHandle, (void*)address, &buffer, 255, NULL);
	return buffer;
}

void CTrainer::writeNops(DWORD address, int nops)
{
	char nop[4] = {0x90, 0x90, 0x90, 0x90};
	WriteProcessMemory(processHandle, (void*)address, &nop, nops, NULL);
}