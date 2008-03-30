#ifndef _TRAINER_H_
#define _TRAINER_H_

/*
Trainer
by Evremonde
*/

#include <string>

#include <windows.h>

class CTrainer
{
  public:
	// constructor and deconstructor
	CTrainer();
	~CTrainer();

	// game window and process
	HWND _hwnd;
	DWORD _processId;
	HANDLE _processHandle;

	HWND getHwnd();
	DWORD getProcessId();
	HANDLE getProcessHandle();

	void setProcess(HWND gameWindow);
	void closeProcess();

	// read and write memory
	void writeBytes(DWORD address, int value, int bytes);
	int readBytes(DWORD address, int bytes);

	void writeString(DWORD address, std::string value);
	std::string readString(DWORD address);

	void writeNops(DWORD address, int nops);

	void writeArray(DWORD address, unsigned char* value, int bytes);
};

#endif // #ifndef _TRAINER_H_
