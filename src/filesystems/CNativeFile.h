//
//  CNativeFile.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef CNATIVEFILE_H
#define CNATIVEFILE_H

#include "IFile.h"

namespace jam
{

class CNativeFile final : public IFile
{
    JAM_OBJECT
public:
    CNativeFile();
    ~CNativeFile();
    
    /*
     * Get file information
     */
    virtual IFileInfoPtr FileInfo() const override;
    
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
    virtual void Open(IFileInfoPtr fileInfo, FileMode mode) override;
    
    /*
     * String alternative to open file
     */
    virtual void Open(const std::string& filePath, FileMode mode) override;
    
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
    std::fstream m_Stream;
    IFileInfoPtr m_FileInfo;
    bool m_IsReadOnly;
};
    
} // namespace jam

#endif /* CNATIVEFILE_H */
