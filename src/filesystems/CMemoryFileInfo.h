//
//  CMemoryFileInfo.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef CMEMORYFILEINFO_H
#define CMEMORYFILEINFO_H

#include "IFileInfo.h"

namespace jam
{
    
class CMemoryFileInfo final : public IFileInfo
{
    JAM_OBJECT
public:
    CMemoryFileInfo();
    ~CMemoryFileInfo();
    
    CMemoryFileInfo(const std::string& filePath, bool isDir);
    /*
     * Get file name
     */
    virtual const std::string& Name() const override;
    
    /*
     * Get file name without extension
     */
    virtual const std::string& BaseName() const override;
    
    /*
     * Get file extension
     */
    virtual const std::string& Extension() const override;
    
    /*
     * Get absolute file path
     */
    virtual const std::string& AbsolutePath() const override;
    
    /*
     * Directory where file locates
     */
    virtual const std::string& BasePath() const override;
    
    /*
     * Is a directory
     */
    virtual bool IsDir() const override;
    
private:
    void Initialize(const std::string& basePath, const std::string& fileName, bool isDir);
    
private:
    std::string m_Name;
    std::string m_BaseName;
    std::string m_Extension;
    std::string m_AbsolutePath;
    std::string m_BasePath;
    bool m_IsDir;
};
    
}; // namespace jam

#endif /* CMEMORYFILEINFO_H */
