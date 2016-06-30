//
//  CZipFile.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef CZIPFILE_H
#define CZIPFILE_H

#include "IFile.h"

namespace jam
{
CLASS_PTR(CZip);
    
class CZip
{
public:
    CZip(const std::string& zipPath, const std::string& password = "");
    ~CZip();
    
    void* ZipFile();
    void* UnzFile();
    
    const std::string& FileName() const;
    const std::string& Password() const;
    
    void Lock();
    void Unlock();
    
private:
    std::string m_FileName;
    std::string m_Password;
    void* m_ZipFile;
    void* m_UnzFile;    
    std::mutex m_Mutex;
};
    

class CZipFile final : public IFile
{
    JAM_OBJECT
    friend class CZipFileSystem;
public:
    CZipFile(const CFileInfo& fileInfo, CZipPtr zipFile);
    ~CZipFile();
    
    /*
     * Get file information
     */
    virtual const CFileInfo& FileInfo() const override;
    
    /*
     * Returns file size
     */
    virtual uint64_t Size() override;
    
    /*
     * Check is readonly filesystem
     */
    virtual bool IsReadOnly() const override;
    
    /*
     * Open existing file for reading, if not exists return null
     */
    virtual void Open(int mode) override;
    
    /*
     * Close file
     */
    virtual void Close() override;
    
    /*
     * Check is file ready for reading/writing
     */
    virtual bool IsOpened() const override;
    
    /*
     * Seek on a file
     */
    virtual uint64_t Seek(uint64_t offset, Origin origin) override;
    
    /*
     * Returns offset in file
     */
    virtual uint64_t Tell() override;
    
    /*
     * Read data from file to buffer
     */
    virtual uint64_t Read(uint8_t* buffer, uint64_t size) override;
    
    /*
     * Write buffer data to file
     */
    virtual uint64_t Write(const uint8_t* buffer, uint64_t size) override;
    
private:
    CZipPtr m_ZipFile;
    int m_Mode;
    const CFileInfo& m_FileInfo;
    bool m_IsReadOnly;
    std::vector<uint8_t> m_Data;
    uint64_t m_SeekPos;
    bool m_IsOpened;
};
    
} // namespace jam

#endif /* CZIPFILE_H */
