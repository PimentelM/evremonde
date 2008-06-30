#ifndef TRAINER_H
#define TRAINER_H

#include <string>

#include <windows.h>

/*
Trainer
by Evremonde
*/

class CTrainer
{
  public:
	// constructor/deconstructor
	CTrainer();
	~CTrainer();

	// trainer basics
	HWND	hwnd;
	DWORD	processId;
	HANDLE	processHandle;

	// process priority
	enum ProcessPriority_t
	{
		PRIORITY_NORMAL			= 32,
		PRIORITY_LOW			= 64,
		PRIORITY_REAL_TIME		= 128,
		PRIORITY_HIGH			= 256,
		PRIORITY_BELOW_NORMAL	= 16384,
		PRIORITY_ABOVE_NORMAL	= 32768,
	};

	// read/write memory
	void writeBytes(DWORD address, int value, int bytes);
	int readBytes(DWORD address, int bytes);
	
	void writeString(DWORD address, std::string value);
	std::string readString(DWORD address);

	void writeNops(DWORD address, int nops);
};

#endif // #ifndef TRAINER_H
