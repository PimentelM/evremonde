/*
 * Recursive Find
 * by Evremonde
 * Last updated on 12/20/2008
 */

#include "recursivefind.h"

typedef struct
{
    std::string root;
    std::string folder;
    std::string file;
} FileLocation;

std::string ALL_FILES = "*.*";

std::vector<std::string> vFiles;
std::vector<std::string>::iterator vFilesIt;

void SearchForFiles(std::string sRoot, std::string sFileExtension)
{
    WIN32_FIND_DATA FindFileData;

    std::stringstream sStream1;
    sStream1 << sRoot << ALL_FILES;

    HANDLE hFind = FindFirstFile(sStream1.str().c_str(), &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) 
    {
        std::cout << "INVALID_HANDLE_VALUE" << std::endl;
        return;
    }

    do
    {
        FileLocation fl;
        fl.root = sRoot;

        if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            if (strcmp(FindFileData.cFileName, ".") != 0)
            {
                if (strcmp(FindFileData.cFileName, "..") != 0)
                {
                    fl.folder = FindFileData.cFileName;

                    std::cout << std::endl << "Folder: " << fl.folder << std::endl;

                    std::stringstream sStream2;
                    sStream2 << fl.root << fl.folder << "/";

                    SearchForFiles(sStream2.str().c_str(), sFileExtension);
                }
            }
        }
        else
        {
            fl.file = FindFileData.cFileName;

            if (PathMatchSpec(fl.file.c_str(), sFileExtension.c_str()))
            {
                std::stringstream sStream3;
                sStream3 << fl.root << fl.folder << fl.file;
                vFiles.push_back(sStream3.str());

                std::cout << std::endl << "File: " << fl.file << std::endl;
            }
        }
    }
    while (FindNextFile(hFind, &FindFileData) != 0);

    FindClose(hFind);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 0;

    SearchForFiles(argv[1], argv[2]);

    std::fstream fsFile;

    fsFile.open("recursivefind.txt", std::ios::out);

    fsFile << "Recursive Find" << std::endl;
    fsFile << "Folder: " << argv[1] << std::endl;
    fsFile << "File Extension: " << argv[2] << std::endl;
    fsFile << vFiles.size() << " file(s) found." << std::endl;
    fsFile << std::endl;

    for(vFilesIt = vFiles.begin(); vFilesIt < vFiles.end(); vFilesIt++)
        fsFile << *vFilesIt << std::endl;

    fsFile.close();

    return 0;
}
