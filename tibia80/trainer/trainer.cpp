/*
Trainer
by Evremonde
*/

#include "trainer.h"

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

void CTrainer::getHwnd() { return _hwnd; }

void CTrainer::getProcessId() { return _processId; }

void CTrainer::getProcessHandle() { return _processHandle; }

void CTrainer::setProcess(HWND gameWindow)
{
	// get game window
	_hwnd = gameWindow;

	// get process id
	::GetWindowThreadProcessId(_hwnd, &_processId);

	// get process handle
	_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, _processId);
}

void CTrainer::closeProcess()
{
	// close process handle
	::CloseHandle(_processHandle);
}

/* read and write memory */

void CTrainer::writeBytes(DWORD address, int value, int bytes)
{
	::WriteProcessMemory(_processHandle, reinterpret_cast<void*>(address), &value, bytes, 0);
}

int CTrainer::readBytes(DWORD address, int bytes)
{
	int buffer = 0;
	::ReadProcessMemory(_processHandle, reinterpret_cast<void*>(address), &buffer, bytes, 0);
	return buffer;
}

void CTrainer::writeString(DWORD address, std::string value)
{
	int j = 0;
	for(unsigned int i = 0; i < value.size(); i++)
	{
		::WriteProcessMemory(_processHandle, reinterpret_cast<void*>(address + j), &value[i], 1, 0);
		j++; // increment address
	}
	unsigned char nullTerminator = 0x00; // end of string
	::WriteProcessMemory(_processHandle, reinterpret_cast<void*>(address + j), &nullTerminator, 1, 0);
}

std::string CTrainer::readString(DWORD address)
{
	static char buffer[256];
	::ReadProcessMemory(_processHandle, reinterpret_cast<void*>(address), &buffer, sizeof(buffer), 0);
	return buffer;
}

void CTrainer::writeNops(DWORD address, int nops)
{
	int j = 0;
	for (int i = 0; i < nops; i++)
	{
		unsigned char nop = 0x90;
		::WriteProcessMemory(_processHandle, reinterpret_cast<void*>(address + j), &nop, 1, 0);
		j++; // increment address
	}
}
