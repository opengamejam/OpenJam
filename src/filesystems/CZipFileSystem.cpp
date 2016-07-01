//
//  CZipFileSystem.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CZipFileSystem.h"
#include <dirent.h>
#include <fstream>
#include "CZipFile.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CZipFileSystem::CZipFileSystem(const std::string& zipPath, bool createIfNotExist, const std::string& password)
: m_ZipPath(zipPath)
, m_IsInitialized(false)
, m_IsNeedCreate(createIfNotExist)
, m_Password(password)
{
}

CZipFileSystem::~CZipFileSystem()
{

}

void CZipFileSystem::Initialize()
{
    if (m_IsInitialized)
    {
        return;
    }
    
    m_Zip.reset(new CZip(m_ZipPath, m_IsNeedCreate, m_Password));
    m_IsInitialized = true;
}

void CZipFileSystem::Shutdown()
{
    m_FileList.clear();
    m_IsInitialized = false;
}


bool CZipFileSystem::IsInitialized() const
{
    return m_IsInitialized;
}


const std::string& CZipFileSystem::BasePath() const
{
    static std::string basePath = "/";
    return basePath;
}


const IFileSystem::TFileList& CZipFileSystem::FileList() const
{
    return m_FileList;
}


bool CZipFileSystem::IsReadOnly() const
{
    if (!IsInitialized())
    {
        return true;
    }
    
    return m_Zip->IsReadOnly();
}


IFilePtr CZipFileSystem::OpenFile(const CFileInfo& filePath, int mode)
{
    CFileInfo fileInfo(BasePath(), filePath.AbsolutePath(), false);
    IFilePtr file = FindFile(fileInfo);
    bool isExists = (file != nullptr);
    if (!isExists)
    {        
        file.reset(new CZipFile(fileInfo, m_Zip));
    }
    file->Open(mode);
    
    if (!isExists && file->IsOpened())
    {
        m_FileList.insert(file);
    }
    
    return file;
}


void CZipFileSystem::CloseFile(IFilePtr file)
{
    if (file)
    {
        file->Close();
    }
}


bool CZipFileSystem::CreateFile(const CFileInfo& filePath)
{
    bool result = false;
    if (!IsFileExists(filePath))
    {
        IFilePtr file = OpenFile(filePath, IFile::ReadWrite);
        if (file)
        {
            result = true;
            file->Close();
        }
    }
    else
    {
        result = true;
    }
    
    return result;
}


bool CZipFileSystem::RemoveFile(const CFileInfo& filePath)
{
    return false; // TODO: Filesystem, temporally not suppoted
}


bool CZipFileSystem::CopyFile(const CFileInfo& src, const CFileInfo& dest)
{
    bool result = false;
    if (!IsReadOnly())
    {
        CZipFilePtr srcFile = std::static_pointer_cast<CZipFile>(FindFile(src));
        CZipFilePtr dstFile = std::static_pointer_cast<CZipFile>(OpenFile(dest, IFile::Out));
        
        if (srcFile && dstFile)
        {
            dstFile->m_Data.assign(srcFile->m_Data.begin(), srcFile->m_Data.end());
            dstFile->Close();
            
            result = true;
        }
    }
    
    return result;
}


bool CZipFileSystem::RenameFile(const CFileInfo& src, const CFileInfo& dest)
{
    return false; // TODO: Filesystem, temporally not suppoted
}


bool CZipFileSystem::IsFileExists(const CFileInfo& filePath) const
{
    return (FindFile(BasePath() + filePath.AbsolutePath()) != nullptr);
}


bool CZipFileSystem::IsFile(const CFileInfo& filePath) const
{
    IFilePtr file = FindFile(filePath);
    if (file)
    {
        return !file->FileInfo().IsDir();
    }
    
    return false;
}


bool CZipFileSystem::IsDir(const CFileInfo& dirPath) const
{
    IFilePtr file = FindFile(dirPath);
    if (file)
    {
        return file->FileInfo().IsDir();
    }
    
    return false;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

IFilePtr CZipFileSystem::FindFile(const CFileInfo& fileInfo) const
{
    TFileList::const_iterator it = std::find_if(m_FileList.begin(), m_FileList.end(), [&](IFilePtr file)
    {
        return file->FileInfo() == fileInfo;
    });
    
    if (it != m_FileList.end())
    {
        return *it;
    }
    
    return nullptr;
}
