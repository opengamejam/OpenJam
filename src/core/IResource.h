/* 
 * File:   IResource.h
 * Author: yevgeniy.logachev
 */

#ifndef IRESOURCE_H
#define	IRESOURCE_H

#include "Global.h"
#include "ILoader.h"

namespace jam
{
    
class IResource : public std::enable_shared_from_this<IResource>
{
public:
    typedef std::vector<uint8_t> TResourceData;
    
public:
    IResource(const std::string& filename, const std::string& loaderName);
    IResource(const std::string& name, const TResourceData& data);
    virtual ~IResource();
    
	const std::string& Filename() const;
	const std::string& LoaderName();
    virtual const TResourceData& RawData();
    virtual bool Load();
    virtual void Unload();
    
    // In most cases hash will be match with the filename,
    // override it if you want to make a unique hash for some resources (for ex. materials)
    virtual const std::string& Hash();
    
private:
    bool Load(const std::string& filename);
    
private:
    TResourceData m_Data;
    
    std::string m_Filename;
	std::string	m_LoaderName;
    
    bool m_IsExternalData;
};
    
}; // namespace jam

#endif	/* IRESOURCE_H */

