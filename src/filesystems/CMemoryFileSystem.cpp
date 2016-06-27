//
//  CMemoryFileSystem.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CMemoryFileSystem.h"
#include "CMemoryFile.h"
#include "CMemoryFileInfo.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMemoryFileSystem::CMemoryFileSystem()
: m_IsInitialized(false)
{

}

CMemoryFileSystem::~CMemoryFileSystem()
{

}

void CMemoryFileSystem::Initialize()
{
    if (m_IsInitialized)
    {
        return;
    }
    
    m_IsInitialized = true;
}

void CMemoryFileSystem::Shutdown()
{
    
}


bool CMemoryFileSystem::IsInitialized() const
{
    return m_IsInitialized;
}


const std::string& CMemoryFileSystem::BasePath() const
{
    static std::string basePath = "/";
    return basePath;
}


const IFileSystem::TFileNamesList& CMemoryFileSystem::FileList(IFileSystem::Entries filter) const
{
    return m_FileNameList;
}


bool CMemoryFileSystem::IsReadOnly() const
{
    return false;
}


IFilePtr CMemoryFileSystem::OpenFile(const std::string& filePath, IFile::FileMode mode)
{
    IFileInfoPtr fileInfo(new CMemoryFileInfo(BasePath() + filePath, false));
    CMemoryFilePtr file = nullptr;
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(fileInfo->AbsolutePath());
    if (it == m_FileInfoMap.end())
    {
        file.reset(new CMemoryFile());
    }
    else
    {
        file = it->second;
    }
    file->Open(fileInfo, mode); // TODO: FileSystem, mode
    
    if (!IsFileExists(filePath) && file->IsOpened())
    {
        m_FileNameList.insert(fileInfo);
        m_FileInfoMap[fileInfo->AbsolutePath()] = file;
    }
    
    return file;
}


void CMemoryFileSystem::CloseFile(IFilePtr file)
{
    if (file)
    {
        file->Close();
    }
}


bool CMemoryFileSystem::CreateFile(const std::string& filePath)
{
    bool result = false;
    if (!IsFileExists(filePath))
    {
        IFilePtr file = OpenFile(filePath, IFile::ReadWrite);
        result = (file != nullptr);
        file->Close();
    }
    else
    {
        result = true;
    }
    
    return result;
}


bool CMemoryFileSystem::RemoveFile(const std::string& filePath)
{
    bool result = true;
    if (IsFileExists(filePath))
    {
        IFileInfoPtr fileInfo(new CMemoryFileInfo(BasePath() + filePath, false));
        
        m_FileNameList.erase(fileInfo);
        m_FileInfoMap.erase(fileInfo->AbsolutePath());
    }
    
    return result;
}


bool CMemoryFileSystem::CopyFile(const std::string& from, const std::string& to)
{
    bool result = false;
    if (IsFileExists(from) && !IsFileExists(to))
    {
        std::shared_ptr<CMemoryFileInfo> fromInfo(new CMemoryFileInfo(BasePath() + from, false));
        std::shared_ptr<CMemoryFileInfo> toInfo(new CMemoryFileInfo(BasePath() + to, false));
        
        CMemoryFilePtr srcFile = std::static_pointer_cast<CMemoryFile>(m_FileInfoMap[fromInfo->AbsolutePath()]);
        CMemoryFilePtr dstFile = std::static_pointer_cast<CMemoryFile>(m_FileInfoMap[toInfo->AbsolutePath()]);
        
        dstFile->m_Data.assign(srcFile->m_Data.begin(), srcFile->m_Data.end());
        
        m_FileNameList.insert(toInfo);
    }
    
    return result;
}


bool CMemoryFileSystem::RenameFile(const std::string& from, const std::string& to)
{
    bool result = true;
    if (IsFileExists(from) && !IsFileExists(to))
    {
        std::shared_ptr<CMemoryFileInfo> fromInfo(new CMemoryFileInfo(BasePath() + from, false));
        std::shared_ptr<CMemoryFileInfo> toInfo(new CMemoryFileInfo(BasePath() + to, false));
        
        m_FileNameList.erase(fromInfo);
        m_FileNameList.insert(toInfo); // TODO: FileSystem, rename fileInfo in CMemoryFile
    }
    
    return result;
}


bool CMemoryFileSystem::IsFileExists(const std::string& filePath) const
{
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + filePath);
    return (it != m_FileInfoMap.end());
}


bool CMemoryFileSystem::IsFile(const std::string& filePath) const
{
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + filePath);
    if (it != m_FileInfoMap.end())
    {
        IFileInfoPtr fileInfo = it->second->FileInfo();
        return (fileInfo ? !fileInfo->IsDir() : false);
    }
    
    return false;
}


bool CMemoryFileSystem::IsDir(const std::string& dirPath) const
{
    std::string dpath = dirPath;
    if (!StringEndsWith(dirPath, "/"))
    {
        dpath += "/";
    }
    
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + dpath);
    if (it != m_FileInfoMap.end())
    {
        IFileInfoPtr fileInfo = it->second->FileInfo();
        return (fileInfo ? fileInfo->IsDir() : false);
    }
    
    return false;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
