#include "ini.h"

/*
INI
by Evremonde
*/

/* constructor and deconstructor */

CIni::CIni()
{
	//
}

CIni::~CIni()
{
	//
}

/* read ini */

std::vector<std::string> CIni::getSections(std::string fileName)
{
	// sections vector
	std::vector<std::string> sections;

	// open ini file
	std::ifstream file(fileName.c_str());
	
	// read line by line
    std::string line;
	while (std::getline(file, line))
	{
		// find and parse section
		int findPositionBegin	= line.find("[");
		int findPositionEnd		= line.find("]");
		if (findPositionBegin != std::string::npos)
		{
			// get the string between parenthesis
			line = line.substr(findPositionBegin + 1, findPositionEnd - 1);
			
			// add the string to sections vector
			sections.push_back(line);
		}
	}
	
	// close ini file
	file.close();
	
	return sections;
}

int CIni::readInteger(std::string fileName, std::string section, std::string key)
{
	// required for full path fix
	std::string fileLocation = "./" + fileName;
	
	return GetPrivateProfileInt(section.c_str(), key.c_str(), 0, fileLocation.c_str());
}
