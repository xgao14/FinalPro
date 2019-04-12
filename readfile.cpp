#include "readfile.h"
#include <iostream>
#include <fstream>


readfile::readfile()
{
    char buffer[20];
    std::ifstream filelist;
    filelist.open("filelist.txt", std::ios::in); //Read only, get the document files name
    if(!filelist.is_open())
    {
    	std::cout << "File: filelist.txt opened failed!" << std::endl;
    	return;
	}
    filenum = 0;
    while(!filelist.eof() && filenum<=50)
    {
        filelist.getline(buffer, 20); //Get files name
        filename[filenum] = buffer;
        filenum++;
    }
    filenum--;  
    filelist.close();
}

readfile::~readfile()
{}

