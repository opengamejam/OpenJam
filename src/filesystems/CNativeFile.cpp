//
//  CNativeFile.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CNativeFile.h"
#include "CNativeFileInfo.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CNativeFile::CNativeFile()
: m_IsReadOnly(true)
{
}

CNativeFile::~CNativeFile()
{
    Close();
}

IFileInfoPtr CNativeFile::FileInfo() const
{
    return m_FileInfo;
}

uint64_t CNativeFile::Size()
{
    if (IsOpened())
    {
        uint64_t curPos = Tell();
        Seek(0, End);
        uint64_t size = Tell();
        Seek(curPos, Begin);
        
        return size;
    }
    
    return 0;
}

bool CNativeFile::IsReadOnly() const
{
    return m_IsReadOnly;
}

void CNativeFile::Open(IFileInfoPtr fileInfo, IFile::FileMode mode)
{
    if (!fileInfo)
    {
        return;
    }
    
    if (FileInfo() == fileInfo && IsOpened())
    {
        return;
    }
    else if (FileInfo() != fileInfo)
    {
        Close();
    }
    
    m_IsReadOnly = true;
    
    std::ios_base::open_mode open_mode = 0x00;
    if (mode & IFile::In)
    {
        open_mode |= std::fstream::in;
    }
    if (mode & IFile::Out)
    {
        m_IsReadOnly = false;
        open_mode |= std::fstream::out;
    }
    if (mode & IFile::Append)
    {
        m_IsReadOnly = false;
        open_mode |= std::fstream::app;
    }
    if (mode & IFile::Truncate)
    {
        open_mode |= std::fstream::trunc;
    }
    
    m_FileInfo = fileInfo;
    m_Stream.open(fileInfo->AbsolutePath().c_str(), open_mode);
}

void CNativeFile::Open(const std::string& filePath, IFile::FileMode mode)
{
    IFileInfoPtr fileInfo(new CNativeFileInfo(filePath, false));
    Open(fileInfo, mode);
}

void CNativeFile::Close()
{
    m_Stream.close();
}

bool CNativeFile::IsOpened() const
{
    return m_Stream.is_open();
}

uint64_t CNativeFile::Seek(uint64_t offset, Origin origin)
{
    std::ios_base::seekdir way;
    if (origin == Begin)
    {
        way = std::ios_base::beg;
    }
    else if (origin == End)
    {
        way = std::ios_base::end;
    }
    else
    {
        way = std::ios_base::cur;
    }
    
    m_Stream.seekg(offset, way);
    m_Stream.seekp(offset, way);
    
    return Tell();
}

uint64_t CNativeFile::Tell()
{
    return static_cast<uint64_t>(m_Stream.tellg());
}

uint64_t CNativeFile::Read(uint8_t* buffer, uint64_t size)
{
    m_Stream.read (reinterpret_cast<char*>(buffer), size);
    if (m_Stream)
    {
        return size;
    }
    
    return static_cast<uint64_t>(m_Stream.gcount());
}

uint64_t CNativeFile::Write(const uint8_t* buffer, uint64_t size)
{
    m_Stream.write (reinterpret_cast<const char*>(buffer), size);
    if (m_Stream)
    {
        return size;
    }
    
    return static_cast<uint64_t>(m_Stream.gcount());
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************