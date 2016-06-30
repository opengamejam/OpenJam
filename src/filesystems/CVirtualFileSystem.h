//
//  CVirtualFileSystem.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef CVirtualFileSystem_H
#define CVirtualFileSystem_H

#include "IFileSystem.h"
#include "IFile.h"

namespace jam
{
CLASS_PTR(IFile)
CLASS_PTR(IFileSystem)
CLASS_PTR(CVirtualFileSystem)
    
extern void vfs_initialize();
extern void vfs_shutdown();
extern CVirtualFileSystemPtr vfs_get_global();
    
class CVirtualFileSystem final
{
    JAM_OBJECT_BASE
public:
    typedef std::list<IFileSystemPtr> TFileSystemList;
    typedef std::unordered_map<std::string, IFileSystemPtr> TFileSystemMap;
    
    struct SSortedAlias
    {
        std::string alias;
        IFileSystemPtr filesystem;
        
        SSortedAlias(const std::string& a,
                     IFileSystemPtr fs)
        : alias(a)
        , filesystem(fs)
        {}
    };
    typedef std::list<SSortedAlias> TSortedAliasList;
    
public:
    CVirtualFileSystem();
    ~CVirtualFileSystem();
    
    /*
     * Register new filesystem. Alias is base prefix to file access.
     * For ex. registered filesystem has base path '/home/media', but registered
     * with alias '/', so it possible to access files with path '/filename'
     * instead of '/home/media/filename
     */
    void AddFileSystem(const std::string& alias, IFileSystemPtr filesystem);
    
    /*
     * Remove registered filesystem
     */
    void RemoveFileSystem(const std::string& alias);
    
    /*
     * Check if filesystem with 'alias' registered
     */
    bool IsFileSystemExists(const std::string& alias) const;
    
    /*
     * Get filesystem with 'alias'
     */
    IFileSystemPtr GetFilesystem(const std::string& alias);
    
    /*
     * Iterate over all registered filesystems and find first ocurrences of file
     */
    IFilePtr OpenFile(const CFileInfo& filePath, IFile::FileMode mode);
    
    /*
     * Close opened file if it was opened via OpenFirstFile(..)
     */
    void CloseFile(IFilePtr file);
    
    /*
     * Get bundle directory
     */
    static const std::string& GetBundlePath();
    
    /*
     * Set bundle directory. TODO: uses for android only
     */
    static void SetBundlePath(const std::string& path);
    
private:
    TFileSystemMap m_FileSystem;
    TSortedAliasList m_SortedAlias;
    std::unordered_map<uintptr_t, IFileSystemPtr> m_OpenedFiles;
    
    static std::string s_BundlePath;
};
    
}; // namespace jam

#endif /* CVirtualFileSystem_H */
