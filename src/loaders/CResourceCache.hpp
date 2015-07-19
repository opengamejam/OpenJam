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
    
    
class CResourceCache
{
public:
    CResourceCache();
    virtual ~CResourceCache();
    
    template <class T>
    std::shared_ptr<T> AcquireResource(const std::string& filename,
                                       bool isUnique,
                                       std::function<IResourcePtr(const std::string&)> instantiationFunc)
    {
        std::shared_ptr<T> resource = nullptr;
        if (!isUnique)
        {
            resource = FindResource<T>(filename);
        }
        
        if (!resource)
        {
            resource = std::dynamic_pointer_cast<T>(instantiationFunc(filename));
            if (resource && resource->Load())
            {
                CacheResource<T>(filename, isUnique, resource);
            }
            else
            {
                resource = nullptr;
            }
        }
        
        return resource;
    }
    
    /*template <class T>
    std::shared_ptr<T> RequestResource(const std::string& filename, bool isUnique = false)
    {
        std::shared_ptr<T> resource = nullptr;
        if (!isUnique)
        {
            resource = FindResource<T>(filename);
        }
        
        if (!resource)
        {
            resource = std::make_shared<T>(T(filename));
            assert(resource);
            if (resource && resource->Load())
            {
                CacheResource<T>(filename, isUnique, resource);
            }
            else
            {
                return nullptr;
            }
        }
        
        return resource;
    }*/
    
    template <class T>
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
    
    template <class T>
    void CacheResource(const std::string& filename, bool isUnique, std::shared_ptr<T> resource)
    {
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
    typedef std::map<std::string, IResourcePtr> TResourcesMap;
    static TResourcesMap s_CachedResources;
    static unsigned int s_UniqIndex;
};

}; // namespace jam
    
#endif /* defined(CRESOURCECACHE_H) */
