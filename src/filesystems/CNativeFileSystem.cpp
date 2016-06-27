//
//  CNativeFileSystem.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CNativeFileSystem.h"
#include <dirent.h>
#include <fstream>
#include "CNativeFile.h"
#include "CNativeFileInfo.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

struct SDir : public DIR {};

// *****************************************************************************
// Public Methods
// *****************************************************************************

CNativeFileSystem::CNativeFileSystem(const std::string& basePath)
: m_BasePath(basePath)
, m_IsInitialized(false)
{
    if (!StringEndsWith(m_BasePath, "/"))
    {
        m_BasePath += "/";
    }
}

CNativeFileSystem::~CNativeFileSystem()
{

}

void CNativeFileSystem::Initialize()
{
    if (m_IsInitialized)
    {
        return;
    }
    
    SDir *dir = static_cast<SDir*>(opendir(BasePath().c_str()));
    if (dir)
    {
        BuildFilelist(dir, BasePath(), m_FileList, m_FileInfoMap);
        m_IsInitialized = true;
        
        closedir(dir);
    }
}

void CNativeFileSystem::Shutdown()
{
    m_FileList.clear();
    m_FileInfoMap.clear();
    m_BasePath = "";
    m_IsInitialized = false;
}


bool CNativeFileSystem::IsInitialized() const
{
    return m_IsInitialized;
}


const std::string& CNativeFileSystem::BasePath() const
{
    return m_BasePath;
}


const IFileSystem::TFileNamesList& CNativeFileSystem::FileList(IFileSystem::Entries filter) const
{
    return m_FileList; // TODO: FileSystem, apply filter
}


bool CNativeFileSystem::IsReadOnly() const
{
    return false;
}


IFilePtr CNativeFileSystem::OpenFile(const std::string& filePath, IFile::FileMode mode)
{
    IFilePtr file(new CNativeFile());
    
    std::shared_ptr<CNativeFileInfo> fileInfo(new CNativeFileInfo(BasePath() + filePath, false));
    file->Open(fileInfo, mode);
    
    if (!IsFileExists(filePath) && file->IsOpened())
    {
        m_FileList.insert(fileInfo);
        m_FileInfoMap[fileInfo->AbsolutePath()] = fileInfo;
    }
    
    return file;
}


void CNativeFileSystem::CloseFile(IFilePtr file)
{
    if (file)
    {
        file->Close();
    }
}


bool CNativeFileSystem::CreateFile(const std::string& filePath)
{
    bool result = false;
    if (!IsFileExists(filePath))
    {
        std::shared_ptr<CNativeFileInfo> fileInfo(new CNativeFileInfo(BasePath() + filePath, false));
        
        std::fstream fs;
        fs.open(fileInfo->AbsolutePath().c_str(), std::fstream::out | std::fstream::trunc);
        if (fs.is_open())
        {
            m_FileList.insert(fileInfo);
            m_FileInfoMap[fileInfo->AbsolutePath()] = fileInfo;
            
            result = true;
        }
        
        fs.close();
    }
    else
    {
        result = true;
    }
    
    return result;
}


bool CNativeFileSystem::RemoveFile(const std::string& filePath)
{
    bool result = true;
    if (IsFileExists(filePath))
    {
        std::shared_ptr<CNativeFileInfo> fileInfo(new CNativeFileInfo(BasePath() + filePath, false));
        if (remove(fileInfo->AbsolutePath().c_str()))
        {
            m_FileList.erase(fileInfo);
            m_FileInfoMap.erase(fileInfo->AbsolutePath());
        }
    }
    
    return result;
}


bool CNativeFileSystem::CopyFile(const std::string& from, const std::string& to)
{
    bool result = false;
    if (IsFileExists(from) && !IsFileExists(to))
    {
        std::shared_ptr<CNativeFileInfo> fromInfo(new CNativeFileInfo(BasePath() + from, false));
        std::shared_ptr<CNativeFileInfo> toInfo(new CNativeFileInfo(BasePath() + to, false));
        
        std::ifstream  src(fromInfo->AbsolutePath().c_str(), std::ios::binary);
        std::ofstream  dst(toInfo->AbsolutePath().c_str(), std::ios::binary);
        
        dst << src.rdbuf();
        
        if (dst)
        {
            m_FileList.insert(toInfo);
            m_FileInfoMap[toInfo->AbsolutePath()] = toInfo;
            
            result = true;
        }
    }
    
    return result;
}


bool CNativeFileSystem::RenameFile(const std::string& from, const std::string& to)
{
    bool result = true;
    if (IsFileExists(from) && !IsFileExists(to))
    {
        std::shared_ptr<CNativeFileInfo> fromInfo(new CNativeFileInfo(BasePath() + from, false));
        std::shared_ptr<CNativeFileInfo> toInfo(new CNativeFileInfo(BasePath() + to, false));
        
        if (rename(fromInfo->AbsolutePath().c_str(), toInfo->AbsolutePath().c_str()))
        {
            m_FileList.erase(fromInfo);
            m_FileInfoMap.erase(fromInfo->AbsolutePath());
            
            m_FileList.insert(toInfo);
            m_FileInfoMap[toInfo->AbsolutePath()] = toInfo;
        }
    }
    
    return result;
}


bool CNativeFileSystem::IsFileExists(const std::string& filePath) const
{
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + filePath);
    return (it != m_FileInfoMap.end());
}


bool CNativeFileSystem::IsFile(const std::string& filePath) const
{
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + filePath);
    if (it != m_FileInfoMap.end())
    {
        return !it->second->IsDir();
    }
    
    return false;
}


bool CNativeFileSystem::IsDir(const std::string& dirPath) const
{
    std::string dpath = dirPath;
    if (!StringEndsWith(dirPath, "/"))
    {
        dpath += "/";
    }
    
    TFileInfoMap::const_iterator it = m_FileInfoMap.find(BasePath() + dpath);
    if (it != m_FileInfoMap.end())
    {
        return it->second->IsDir();
    }
    
    return false;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CNativeFileSystem::BuildFilelist(SDir* dir, std::string basePath,
                                      TFileNamesList& outFileList, TFileInfoMap& outFileInfoMap)
{
    if (!StringEndsWith(basePath, "/"))
    {
        basePath += "/";
    }
    
    struct dirent *ent;
    while ((ent = readdir(dir)) != NULL)
    {
        std::string filename = ent->d_name;
        std::string filepath = basePath + filename;
        SDir *childDir = static_cast<SDir*>(opendir(filepath.c_str()));
        
        bool isDotOrDotDot = StringEndsWith(filename, ".") && childDir;
        if (childDir)
        {
            filename += (isDotOrDotDot ? "" : "/");
        }
        
        std::shared_ptr<CNativeFileInfo> fileInfo(new CNativeFileInfo(basePath, filename, childDir != NULL));
        outFileList.insert(fileInfo);
        outFileInfoMap[fileInfo->AbsolutePath()] = fileInfo;
        
        if (childDir)
        {
            if (!isDotOrDotDot)
            {
                BuildFilelist(childDir, (childDir ? filepath : basePath), outFileList, outFileInfoMap);
            }
            closedir(childDir);
        }
    }
}