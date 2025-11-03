#ifndef FILE_UTILITY_H
#define FILE_UTILITY_H

#include <fstream>
#include <iostream>

static inline std::string LoadTextFile(const char* name)
{
    std::ifstream file;
    file.open(name);
    if (file.fail())
    {
        throw(std::ios_base::failure(std::string("Error opening file: ") + std::string(name))); 
    }

    std::string contents;
    std::string cur_line;
    while (std::getline(file, cur_line))
    {
        contents += cur_line +"\n";
    }

    file.close();

    return contents;
}

#endif