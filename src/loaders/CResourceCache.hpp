//
//  CResourceCache.h
//  OpenJam
//
//  Created by yev on 6/11/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CRESOURCECACHE_H
#define CRESOURCECACHE_H

#include "IResource.h"


namespace jam
{
CLASS_PTR(IResource);
    
template <class T>
class CResourceCache
{
public:
    CResourceCache() = default;
    virtual ~CResourceCache() = default;
    
    std::shared_ptr<T> AcquireResource(const std::string& filename,
                                       bool isUnique,
                                       std::function<std::shared_ptr<T>(const std::string&)> instantiationFunc)
    {
        std::shared_ptr<T> resource = nullptr;
        if (!isUnique)
        {
            resource = FindResource(filename);
        }
        
        if (!resource)
        {
            resource = instantiationFunc(filename);
            CacheResource(filename, isUnique, resource);
        }
        
        return resource;
    }
    
    std::shared_ptr<T> FindResource(const std::string& filename)
    {
        std::shared_ptr<T> resource = nullptr;
        std::string key = filename + typeid(T).name();
        if (s_CachedResources.find(key) != s_CachedResources.end())
        {
            resource = std::dynamic_pointer_cast<T>(s_CachedResources[key]);
        }
        
        return resource;
    }
    
    void CacheResource(const std::string& filename, bool isUnique, std::shared_ptr<T> resource)
    {
        if (!resource)
        {
            return;
        }
        
        std::stringstream key(filename + typeid(T).name());
        if (isUnique)
        {
            key << "unique" << s_UniqIndex++;
        }
        if (s_CachedResources.find(key.str()) == s_CachedResources.end())
        {
            s_CachedResources[key.str()] = resource;
        }
    }
    
private:
    typedef std::unordered_map<std::string, std::shared_ptr<T> > TResourcesMap;
    static TResourcesMap s_CachedResources;
    static unsigned int s_UniqIndex;
};
    
template <class T>
typename CResourceCache<T>::TResourcesMap CResourceCache<T>::s_CachedResources;

template <class T>
unsigned int CResourceCache<T>::s_UniqIndex = 0;

}; // namespace jam
    
#endif /* defined(CRESOURCECACHE_H) */
