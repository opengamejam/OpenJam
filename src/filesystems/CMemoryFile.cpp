//
//  CMemoryFile.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CMemoryFile.h"
#include "CMemoryFileInfo.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMemoryFile::CMemoryFile()
: m_IsReadOnly(true)
, m_IsOpened(false)
, m_SeekPos(0)
{
}

CMemoryFile::~CMemoryFile()
{
    Close();
}

IFileInfoPtr CMemoryFile::FileInfo() const
{
    return m_FileInfo;
}

uint64_t CMemoryFile::Size()
{
    if (IsOpened())
    {
        return m_Data.size();
    }
    
    return 0;
}

bool CMemoryFile::IsReadOnly() const
{
    return m_IsReadOnly;
}

void CMemoryFile::Open(IFileInfoPtr fileInfo, IFile::FileMode mode)
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
    if (mode & IFile::Out)
    {
        m_IsReadOnly = false;
    }
    if (mode & IFile::Append)
    {
        m_IsReadOnly = false;
        m_SeekPos = Size() > 0 ? Size() - 1 : 0;
    }
    
    m_FileInfo = fileInfo;
    m_IsOpened = true;
}

void CMemoryFile::Open(const std::string& filePath, IFile::FileMode mode)
{
    IFileInfoPtr fileInfo(new CMemoryFileInfo(filePath, false));
    Open(fileInfo, mode);
}

void CMemoryFile::Close()
{
    m_IsReadOnly = true;
    m_IsOpened = false;
    m_SeekPos = 0;
}

bool CMemoryFile::IsOpened() const
{
    return m_IsOpened;
}

uint64_t CMemoryFile::Seek(uint64_t offset, Origin origin)
{
    if (!IsOpened())
    {
        return 0;
    }
    
    if (origin == IFile::Begin)
    {
        m_SeekPos = offset;
    }
    else if (origin == IFile::End)
    {
        m_SeekPos = Size() - offset;
    }
    else
    {
        m_SeekPos += offset;
    }
    m_SeekPos = std::min(m_SeekPos, Size() - 1);
    
    return Tell();
}

uint64_t CMemoryFile::Tell()
{
    return m_SeekPos;
}

uint64_t CMemoryFile::Read(uint8_t* buffer, uint64_t size)
{
    if (!IsOpened())
    {
        return 0;
    }
    
    uint64_t bufferSize = Size() - Tell();
    uint64_t maxSize = std::min(size, bufferSize);
    if (maxSize > 0)
    {
        memcpy(buffer, m_Data.data(), maxSize);
    }
    else
    {
        return 0;
    }
    
    return maxSize;
}

uint64_t CMemoryFile::Write(const uint8_t* buffer, uint64_t size)
{
    if (!IsOpened() || IsReadOnly())
    {
        return 0;
    }
    
    uint64_t bufferSize = Size() - Tell();
    if (size > bufferSize)
    {
        m_Data.resize(m_Data.size() + (size - bufferSize));
    }
    memcpy(m_Data.data() + Tell(), buffer, size);
    
    return size;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************