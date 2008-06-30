#include "trainer.h"

/*
Trainer
by Evremonde
*/

/* constructor and deconstructor */

CTrainer::CTrainer()
{
	//
}

CTrainer::~CTrainer()
{
	//
}

/* process */

void CTrainer::setProcess(HWND gameWindow)
{
	// get game window
	hwnd = gameWindow;

	// get process id
	::GetWindowThreadProcessId(hwnd, &processId);

	// get process handle
	processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
}

void CTrainer::closeProcess()
{
	// close process handle
	::CloseHandle(processHandle);
}

/* read and write memory */

void CTrainer::writeBytes(DWORD address, int value, int bytes)
{
	::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address), &value, bytes, 0);
}

int CTrainer::readBytes(DWORD address, int bytes)
{
	int buffer = 0;
	::ReadProcessMemory(processHandle, reinterpret_cast<void*>(address), &buffer, bytes, 0);
	return buffer;
}

void CTrainer::writeString(DWORD address, std::string value)
{	
	int j = 0;
	for(unsigned int i = 0; i < value.size(); i++)
	{
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address + j), &value[i], 1, 0);
		j++; // increment address
	}
	
	unsigned char nullTerminator = 0x00;
	::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address + j), &nullTerminator, 1, 0);
}

std::string CTrainer::readString(DWORD address)
{ 
	static char buffer[255];
	::ReadProcessMemory(processHandle, reinterpret_cast<void*>(address), &buffer, sizeof(buffer), 0);
	return buffer;
}

void CTrainer::writeNops(DWORD address, int nops)
{
	int j = 0;
	for (int i = 0; i < nops; i++)
	{
		unsigned char nop = 0x90;
		::WriteProcessMemory(processHandle, reinterpret_cast<void*>(address + j), &nop, 1, 0);
		j++; // increment address
	}
}
