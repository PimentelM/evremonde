#ifndef _TRAINER_H_
#define _TRAINER_H_

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

	// protection
	void setProtection(DWORD address, DWORD type, int bytes);

	// read and write memory
	void writeBytes(DWORD address, int value, int bytes);
	int readBytes(DWORD address, int bytes);

	void writeDouble(DWORD address, double value);

	void writeChar(DWORD address, unsigned char* value, int bytes);

	void writeString(DWORD address, std::string value);
	std::string readString(DWORD address);

	void writeNops(DWORD address, int nops);
};

#endif // #ifndef _TRAINER_H_
