//
//  CZipFile.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CZipFile.h"
#include "zip.h"
#include "unzip.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CZip::CZip(const std::string& zipPath, const std::string& password)
: m_FileName(zipPath)
, m_Password(password)
, m_ZipFile(nullptr)
, m_UnzFile(nullptr)
{
    
}

CZip::~CZip()
{
    Unlock();
}

const std::string& CZip::FileName() const
{
    return m_FileName;
}

const std::string& CZip::Password() const
{
    return m_Password;
}

void* CZip::ZipFile()
{
    if (m_UnzFile)
    {
        unzClose(m_UnzFile);
        m_UnzFile = nullptr;
    }
    
    if (!m_ZipFile)
    {
        m_ZipFile = zipOpen64(FileName().c_str(), APPEND_STATUS_ADDINZIP);
    }
    return m_ZipFile;
};

void* CZip::UnzFile()
{
    if (m_ZipFile)
    {
        zipClose(m_ZipFile, 0);
        m_ZipFile = nullptr;
    }
    
    if (!m_UnzFile)
    {
        m_UnzFile = unzOpen64(FileName().c_str());
    }
    return m_UnzFile;
};

void CZip::Lock()
{
    m_Mutex.lock();
};
void CZip::Unlock()
{
    m_Mutex.unlock();
}

CZipFile::CZipFile(const CFileInfo& fileInfo, CZipPtr zipFile)
: m_ZipFile(zipFile)
, m_Mode(0)
, m_FileInfo(fileInfo)
, m_IsReadOnly(true)
, m_SeekPos(0)
, m_IsOpened(false)
{
    
}

CZipFile::~CZipFile()
{
    //Close();
}

const CFileInfo& CZipFile::FileInfo() const
{
    return m_FileInfo;
}

uint64_t CZipFile::Size()
{
    if (IsOpened())
    {
        return m_Data.size();
    }
    
    return 0;
}

bool CZipFile::IsReadOnly() const
{
    return m_IsReadOnly;
}

void CZipFile::Open(int mode)
{
    if (!FileInfo().IsValid() || (IsOpened() && m_Mode == mode))
    {
        return;
    }
    
    m_ZipFile->Lock();
    assert(m_ZipFile->UnzFile());
    unzFile unz = m_ZipFile->UnzFile();
    
    std::string absPath = FileInfo().AbsolutePath();
    if (StringStartsWith(absPath, "/"))
    {
        absPath = absPath.substr(1, absPath.length() - 1);
    }
    
    bool entryOpen = false;
    int err = unzLocateFile(unz, absPath.c_str(), 0);
    if (err == UNZ_OK)
    {
        err = unzOpenCurrentFile(unz);
        entryOpen = (err == UNZ_OK);
        
        unz_file_info64 oFileInfo;
        int err = unzGetCurrentFileInfo64(unz, &oFileInfo, 0, 0, 0, 0, 0, 0);
        if (err == UNZ_OK)
        {
            uint64_t size = oFileInfo.uncompressed_size;
            m_Data.resize(size);
            size = unzReadCurrentFile(unz, m_Data.data(), (unsigned)size);
        }
    }
    
    if (entryOpen)
    {
        unzCloseCurrentFile(unz);
    }
    m_ZipFile->Unlock();
    
    m_Mode = mode;
    m_IsReadOnly = true;
    m_SeekPos = 0;
    if (mode & IFile::Out)
    {
        m_IsReadOnly = false;
    }
    if (mode & IFile::Append)
    {
        m_IsReadOnly = false;
        m_SeekPos = Size() > 0 ? Size() - 1 : 0;
    }
    if (mode & IFile::Truncate)
    {
        m_Data.clear();
    }
    
    m_IsOpened = true;
}

void CZipFile::Close()
{
    m_IsOpened = false;
    return;
    
    m_ZipFile->Lock();
    assert(m_ZipFile->ZipFile());
    unzFile zip = m_ZipFile->ZipFile();
    
    std::string absPath = FileInfo().AbsolutePath();
    bool entryOpen = false;
    if (StringStartsWith(absPath, "/"))
    {
        absPath = absPath.substr(1, absPath.length() - 1);
    }
    
    zip_fileinfo zi = {{0}};
    time_t rawtime;
    time (&rawtime);
    auto timeinfo = localtime(&rawtime);
    zi.tmz_date.tm_sec = timeinfo->tm_sec;
    zi.tmz_date.tm_min = timeinfo->tm_min;
    zi.tmz_date.tm_hour = timeinfo->tm_hour;
    zi.tmz_date.tm_mday = timeinfo->tm_mday;
    zi.tmz_date.tm_mon = timeinfo->tm_mon;
    zi.tmz_date.tm_year = timeinfo->tm_year;
    
    int err = zipOpenNewFileInZip(zip, absPath.c_str(), &zi,
                                  NULL, 0, NULL, 0, NULL, Z_DEFLATED, Z_DEFAULT_COMPRESSION);
    entryOpen = (err == ZIP_OK);
    if (entryOpen)
    {
        if (Size() > 0)
        {
            err = zipWriteInFileInZip(zip, m_Data.data(), (unsigned int)Size());
        }
        
        zipCloseFileInZip(zip);
    }
    m_ZipFile->Unlock();
    
    m_IsOpened = false;
}

bool CZipFile::IsOpened() const
{
    return m_IsOpened;
}

uint64_t CZipFile::Seek(uint64_t offset, Origin origin)
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

uint64_t CZipFile::Tell()
{
    return m_SeekPos;
}

uint64_t CZipFile::Read(uint8_t* buffer, uint64_t size)
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

uint64_t CZipFile::Write(const uint8_t* buffer, uint64_t size)
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