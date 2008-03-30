#ifndef _INI_H_
#define _INI_H_

/*
INI
by Evremonde
*/

#include <vector>
#include <string>
#include <fstream>

#include <windows.h>

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

#endif // #ifndef _INI_H_
