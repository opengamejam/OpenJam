/* 
 * File:   CLoaderFile.cpp
 * Author: yevgeniy.logachev
 */

#include "CLoaderFile.h"
#include <stdio.h>
#include "Global.h"
#include <fstream>
#include <string.h>

using namespace jam;

CLoaderFile::CLoaderFile()
{
    
}

CLoaderFile::~CLoaderFile()
{
	
}

void CLoaderFile::RegisterFileSystem(const std::string& newPath)
{
    // TODO: check if newPath is directory before
    
    m_FileSystems.push_back(newPath);
}

bool CLoaderFile::Map(const std::string& filename, IResource::TResourceData& data)
{
    size_t size = m_FileSystems.size();
    for (size_t i = 0; i < size; ++i)
    {
        std::string absPath = m_FileSystems[i] + filename;
        
        std::ifstream fs(absPath, std::fstream::binary);
        if (fs)
        {
            std::streampos beg = fs.tellg();
            fs.seekg(0, std::ios::end);
            std::streampos end = fs.tellg();
            fs.seekg(0, std::ios::beg);

            size_t filesize = (size_t)(end - beg);
            data.resize(filesize + 1);
            memset(&data[0], 0, filesize + 1);

            fs.read((char*)&data[0], filesize);
            fs.close();

            return true;
        }
    }
    return false;
}

