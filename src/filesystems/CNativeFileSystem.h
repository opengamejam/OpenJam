//
//  CNativeFileSystem.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef CNATIVEFILESYSTEM_H
#define CNATIVEFILESYSTEM_H

#include "IFileSystem.h"

struct SDir;

namespace jam
{
    
class CNativeFileSystem final : public IFileSystem
{
    JAM_OBJECT
public:
    CNativeFileSystem();
    ~CNativeFileSystem();
    /*
     * Initialize filesystem with a base path
     */
    virtual void Initialize(const std::string& basePath) override;
    /*
     * Shutdown filesystem
     */
    virtual void Shutdown() override;
    
    /*
     * Check if filesystem is initialized
     */
    virtual bool IsInitialized() const override;
    
    /*
     * Get base path
     */
    virtual const std::string& BasePath() const override;
    
    /*
     * Retrieve file list according filter
     */
    virtual const TFileNamesList& FileList(Entries filter = AllEntries) const override;
    
    /*
     * Check is readonly filesystem
     */
    virtual bool IsReadOnly() const override;
    
    /*
     * Open existing file for reading, if not exists return null
     */
    virtual IFilePtr OpenFile(const std::string& filePath, IFile::FileMode mode) override;
    
    /*
     * Close file
     */
    virtual void CloseFile(IFilePtr file) override;
    
    /*
     * Create file on writeable filesystem. Return true if file already exists
     */
    virtual bool CreateFile(const std::string& filePath) override;
    
    /*
     * Remove existing file on writable filesystem
     */
    virtual bool RemoveFile(const std::string& filePath) override;
    
    /*
     * Copy existing file on writable filesystem
     */
    virtual bool CopyFile(const std::string& from, const std::string& to) override;
    
    /*
     * Rename existing file on writable filesystem
     */
    virtual bool RenameFile(const std::string& from, const std::string& to) override;
    
    /*
     * Check if file exists on filesystem
     */
    virtual bool IsFileExists(const std::string& filePath) const override;
    
    /*
     * Check is file
     */
    virtual bool IsFile(const std::string& filePath) const override;
    
    /*
     * Check is dir
     */
    virtual bool IsDir(const std::string& dirPath) const override;
    
private:
    typedef std::unordered_map<std::string, IFileInfoPtr> TFileInfoMap;
    void BuildFilelist(SDir* dir, std::string basePath, TFileNamesList& outFileList, TFileInfoMap& outFileInfoMap);
    
private:
    std::string m_BasePath;
    bool m_IsInitialized;
    TFileNamesList m_FileList;
    TFileInfoMap m_FileInfoMap;
};
    
} // namespace jam

#endif /* CNATIVEFILESYSTEM_H */
