/**
 *  Street Fighter IV Color Changer
 *  by Evremonde
 */

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
    std::cout << "Street Fighter IV Color Changer" << std::endl
              << "by Evremonde"                    << std::endl << std::endl;

    if (argc <= 1)
    {
        std::cout << "File not found!"                                                         << std::endl
                  << "Please specify a filename or drag and drop a file on to the executable." << std::endl;

        std::cin.get();

        return 0;
    }

    std::string argument = argv[1];
    if (argument.find(".col.emz") == std::string::npos)
    {
        std::cout << "Incorrect file type!"            << std::endl
                  << "Only *.col.emz files will work." << std::endl;

        std::cin.get();

        return 0;
    }

    std::string fileName = argument.substr(argument.find_last_of("\\") + 1);
    std::cout << "Filename: " << fileName << std::endl;

    std::fstream file;
    file.open(fileName.c_str(), std::ios::in | std::ios::out | std::ios::binary);

    std::string colorNumber = argument.substr(argument.find_last_of("_") + 1, 2);

    file.seekp(-11, std::ios::end);
    file.write(colorNumber.c_str(), 2);

    file.seekp(-29, std::ios::end);
    file.write(colorNumber.c_str(), 2);

    file.close();

    std::cout << "Color has been changed!" << std::endl;

    std::cin.get();

    return 0;
}
