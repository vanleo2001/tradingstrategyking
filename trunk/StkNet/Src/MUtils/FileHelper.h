////////////////////////////////////////////////////////////////////////////////
// General utilities : File I/O helper class
//
// Copyright (c) 2003 by Morning
// http://morningspace.51.net
// mailto:moyingzz@etang.com
//
// Permission to use, copy, modify, distribute and sell this program for any 
// purpose is hereby granted without fee, provided that the above copyright 
// notice appear in all copies and that both that copyright notice and this 
// permission notice appear in supporting documentation.
//
// It is provided "as is" without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef _FILE_HELPER_H_
#define _FILE_HELPER_H_
//
#include <string>
#include <vector>
#include <fstream>
#include <stdio.h>
//
namespace MUtils {

class FileHelper
{
public:

    // used to open binary file
    static bool open(const std::string filename, std::string& content)
    {
        FILE *file = fopen(filename.c_str(), "rb");

        if (file == NULL)
            return false;

        fseek(file, 0, SEEK_END);
        int len = ftell(file);
		rewind(file);

        char *buffer = new char[len];
        fread(buffer, sizeof(char), len, file);
        content.assign(buffer, len);
        delete []buffer;

        fclose(file);
        return true;
    }

    // used to open text file
    static bool open(const std::string file_name, std::vector<std::string>& lines)
    {
        std::ifstream file(file_name.c_str(), std::ios::in);
        if (!file) 
        {
            return false;
        }

        lines.clear();
        char buffer[buffer_size];

        while (file.getline(buffer, buffer_size, '\n'))
        {
            lines.push_back(buffer);
        }
        
        return true;
    }

private:

    enum { buffer_size = 3000 };
};

} // namespace MUtils

#endif // _FILE_HELPER_H_