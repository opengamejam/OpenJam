//
//  CVirtualFileSystem.cpp
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#include "CVirtualFileSystem.h"
#include "CStringUtils.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

static CVirtualFileSystemPtr g_FS;

std::string CVirtualFileSystem::s_BundlePath;

// *****************************************************************************
// Public Methods
// *****************************************************************************

struct SAliasComparator
{
    bool operator()(const CVirtualFileSystem::SSortedAlias& a1, const CVirtualFileSystem::SSortedAlias& a2) const
    {
        return a1.alias.length() > a2.alias.length();
    }
};

void jam::vfs_initialize()
{
    if (!g_FS)
    {
        g_FS.reset(new CVirtualFileSystem());
    }
}

void jam::vfs_shutdown()
{
    g_FS = nullptr;
}

CVirtualFileSystemPtr jam::vfs_get_global()
{
    return g_FS;
}

CVirtualFileSystem::CVirtualFileSystem()
{
    std::string bpath = GetBundlePath();
    if (bpath.empty())
    {
#if defined(__OBJC__) && (defined(OS_IPHONE) || defined(OS_MAC))
        NSString* path = [[NSBundle mainBundle] bundlePath];
        
        bpath = std::string([path UTF8String]) + std::string("/");
#elif defined(OS_KOS)
        bpath = "/rd/";
#endif
        
        SetBundlePath(bpath);
    }
}

CVirtualFileSystem::~CVirtualFileSystem()
{

}

void CVirtualFileSystem::AddFileSystem(const std::string& alias, IFileSystemPtr filesystem)
{
    if (!filesystem)
    {
        return;
    }
    
    std::string a = alias;
    if (!StringEndsWith(a, "/"))
    {
        a += "/";
    }
    
    TFileSystemMap::const_iterator it = m_FileSystem.find(a);
    if (it == m_FileSystem.end())
    {
        m_FileSystem[a] = filesystem;
        m_SortedAlias.push_back(SSortedAlias(a, filesystem));
        m_SortedAlias.sort(SAliasComparator());
    }
}

void CVirtualFileSystem::RemoveFileSystem(const std::string& alias)
{
    std::string a = alias;
    if (!StringEndsWith(a, "/"))
    {
        a += "/";
    }
    
    TFileSystemMap::const_iterator it = m_FileSystem.find(a);
    if (it == m_FileSystem.end())
    {
        m_FileSystem.erase(it);
        // TODO: remove from alias list
    }
}

bool CVirtualFileSystem::IsFileSystemExists(const std::string& alias) const
{
    return (m_FileSystem.find(alias) != m_FileSystem.end());
}

IFilePtr CVirtualFileSystem::OpenFile(const std::string& filePath, IFile::FileMode mode)
{
    IFilePtr file = nullptr;
    std::all_of(m_SortedAlias.begin(), m_SortedAlias.end(), [&](const TSortedAliasList::value_type& fs)
    {
        const std::string& alias = fs.alias;
        IFileSystemPtr filesystem = fs.filesystem;
        if (StringStartsWith(filePath, alias) && filePath.length() != alias.length())
        {
            std::string realFilePath = filePath.substr(alias.length(), filePath.length() - alias.length());
            file = filesystem->OpenFile(realFilePath, mode);
        }
        
        if (file)
        {
            uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(file.get()));
            m_OpenedFiles[addr] = filesystem;
            
            return false;
        }
        
        return true;
    });
    
    return file;
}

void CVirtualFileSystem::CloseFile(IFilePtr file)
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(static_cast<void*>(file.get()));
    
    std::unordered_map<uintptr_t, IFileSystemPtr>::const_iterator it = m_OpenedFiles.find(addr);
    if (it != m_OpenedFiles.end())
    {
        it->second->CloseFile(file);
        m_OpenedFiles.erase(it);
    }
}

const std::string& CVirtualFileSystem::GetBundlePath()
{
    return s_BundlePath;
}

void CVirtualFileSystem::SetBundlePath(const std::string& path)
{
    s_BundlePath = path;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
