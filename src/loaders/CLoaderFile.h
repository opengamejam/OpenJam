/* 
 * File:   CLoaderFile.h
 * Author: yevgeniy.logachev
 */

#ifndef CLOADERFILE_H
#define	CLOADERFILE_H

#include "Global.h"
#include "IResource.h"
#include "ILoader.h"

namespace jam
{

class CLoaderFile : public ILoader
{
public:
    CLoaderFile();
    virtual ~CLoaderFile();
    
    void            RegisterFileSystem(const std::string& newPath);
    
	virtual bool    Map(const std::string& filename, IResource::TResourceData& data);
	
private:
    std::vector<std::string> m_FileSystems;
};

}; // namespace jam

#endif	/* CLoaderFile_H */
