#ifndef __INI_H__
#define __INI_H__

#include <vector>
#include <string>
#include <fstream>

#include <windows.h>

/*
INI
by Evremonde
*/

class CIni
{
  public:
	// constructor and deconstructor
	CIni();
	~CIni();
	
	// read ini
	std::vector<std::string> getSections(std::string fileName);
	int readInteger(std::string fileName, std::string section, std::string key);
};

#endif // #ifndef __INI_H__
