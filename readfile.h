#ifndef __READFILE_H__
#define __READFILE_H__

#include <string>

class readfile 
{
    private:
        std::string filename[50]; //The most number of files is 50
        int     filenum; //Actual number of files
    public:
        readfile();
        ~readfile();
        std::string* getFileList()
        {
            return filename;
        }
        int getFileNum()
        {
            return filenum;
        }
};

#endif
