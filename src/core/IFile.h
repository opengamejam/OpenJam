//
//  IFile.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/22/16.
//
//

#ifndef IFILE_H
#define IFILE_H

#include "Global.h"

namespace jam
{
    
class IFile
{
public:
    /*
     * Seek modes
     */
    enum Origin
    {
        Begin,
        End,
        Set
    };
    
public:
    IFile() = default;
    ~IFile() = default;
    
    /*
     * Get file name
     */
    const std::string& Name() const;
    
    /*
     * Get absolute file path
     */
    const std::string& FillPath() const;
    
    /*
     * Get file name without extension
     */
    const std::string& BaseName() const;
    
    /*
     * Get file extension
     */
    const std::string& Extension() const;
    
    /*
     * Returns file size
     */
    virtual uint64_t Size() = 0;
    
    /*
     * Check is readonly filesystem
     */
    virtual bool IsReadOnly() const = 0;
    
    /*
     * Seek on a file
     */
    virtual uint64_t Seek(uint64_t offset, Origin origin) = 0;
    /*
     * Returns offset in file
     */
    virtual uint64_t Tell() const = 0;
    
    /*
     * Read data from file to buffer
     */
    virtual uint64_t Read(void* buffer, uint64_t size) = 0;
    /*
     * Write buffer data to file
     */
    virtual uint64_t Write(const void* buffer, uint64_t size) = 0;
    
    /*
     * Templated alternative to Read
     */
    template<typename T>
    bool Read(T& value)
    {
        return (Read(&value, sizeof(value)) == sizeof(value));
    }
    
    /*
     * Templated alternative to Write
     */
    template<typename T>
    uint64_t Write(const T& value)
    {
        return (Write(&value, sizeof(value)) == sizeof(value));
    }
};
    
} // namespace jam

#endif /* IFILE_H */
