//
//  IFileSystem.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/22/16.
//
//

#ifndef IFILESYSTEM_H
#define IFILESYSTEM_H

#include "Global.h"
#include "IFile.h"

namespace jam
{
CLASS_PTR(IFile)
CLASS_PTR(IFileInfo)

class IFileSystem
{
    JAM_OBJECT_BASE
public:
    typedef std::set<IFileInfoPtr> TFileNamesList;
    
    /*
     * Filelist filtering modes
     */
    enum Entries
    {
        Files = 0x01,
        Dirs = 0x02,
        AllEntries = Files | Dirs
    };
    
public:
    IFileSystem() = default;
    ~IFileSystem() = default;
    
    /*
     * Initialize filesystem with a base path
     */
    virtual void Initialize(const std::string& basePath) = 0;
    /*
     * Shutdown filesystem
     */
    virtual void Shutdown() = 0;
    
    /*
     * Check if filesystem is initialized
     */
    virtual bool IsInitialized() const = 0;
    
    /*
     * Get base path
     */
    virtual const std::string& BasePath() const = 0;
    
    /*
     * Retrieve file list according filter
     */
    virtual const TFileNamesList& FileList(Entries filter = AllEntries) const = 0;
    
    /*
     * Check is readonly filesystem
     */
    virtual bool IsReadOnly() const = 0;
    
    /*
     * Open existing file for reading, if not exists return null
     */
    virtual IFilePtr OpenFile(const std::string& filePath, IFile::FileMode mode) = 0;
    
    /*
     * Close file
     */
    virtual void CloseFile(IFilePtr file) = 0;
    
    /*
     * Create file on writeable filesystem. Return true if file already exists
     */
    virtual bool CreateFile(const std::string& filePath) = 0;
    
    /*
     * Remove existing file on writable filesystem
     */
    virtual bool RemoveFile(const std::string& filePath) = 0;
    
    /*
     * Copy existing file on writable filesystem
     */
    virtual bool CopyFile(const std::string& from, const std::string& to) = 0;
    
    /*
     * Rename existing file on writable filesystem
     */
    virtual bool RenameFile(const std::string& from, const std::string& to) = 0;
    
    /*
     * Check if file exists on filesystem
     */
    virtual bool IsFileExists(const std::string& filePath) const = 0;
    
    /*
     * Check is file
     */
    virtual bool IsFile(const std::string& filePath) const = 0;
    
    /*
     * Check is dir
     */
    virtual bool IsDir(const std::string& dirPath) const = 0;
};

}; // namespace jam

#endif /* IFILESYSTEM_H */
