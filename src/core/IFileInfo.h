//
//  IFileInfo.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/23/16.
//
//

#ifndef IFILEINFO_H
#define IFILEINFO_H

#include "Global.h"

namespace jam
{
    
class IFileInfo
{
    JAM_OBJECT_BASE
public:
    IFileInfo() = default;
    ~IFileInfo() = default;
    
    /*
     * Get file name
     */
    virtual const std::string& Name() const = 0;
    
    /*
     * Get file name without extension
     */
    virtual const std::string& BaseName() const = 0;
    
    /*
     * Get file extension
     */
    virtual const std::string& Extension() const = 0;
    
    /*
     * Get absolute file path
     */
    virtual const std::string& AbsolutePath() const = 0;
    
    /*
     * Directory where file locates
     */
    virtual const std::string& BasePath() const = 0;
    
    /*
     * Is a directory
     */
    virtual bool IsDir() const = 0;
    
    /*
     * Compare with another file info
     */
    virtual bool operator ==(const IFileInfo& other) const = 0;
    
    /*
     * Compare with string file path
     */
    virtual bool operator ==(const std::string& filePath) const = 0;
};
    
}; // namespace jam

#endif /* IFILEINFO_H */
