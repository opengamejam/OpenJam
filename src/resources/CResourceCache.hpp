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

namespace jam {
CLASS_PTR(IResource)

template <class T>
class CResourceCache {
public:
    CResourceCache() = default;
    virtual ~CResourceCache() = default;

    std::shared_ptr<T> AcquireResource(const std::string& filename,
        std::function<std::shared_ptr<T>(const std::string&)> instantiationFunc)
    {
        std::lock_guard<decltype(s_Mutex)> lock(s_Mutex);
        
        std::shared_ptr<T> resource = FindResourceInternal(filename);
        if (!resource) {
            resource = instantiationFunc(filename);
            CacheResourceInternal(filename, resource);
        }

        return resource;
    }

    std::shared_ptr<T> FindResource(const std::string& filename)
    {
        std::shared_ptr<T> resource = nullptr;
        {
            std::lock_guard<decltype(s_Mutex)> lock(s_Mutex);
            resource = FindResourceInternal(filename);
        }

        return resource;
    }

    void CacheResource(const std::string& filename, std::shared_ptr<T> resource)
    {
        if (!resource) {
            return;
        }

        std::lock_guard<decltype(s_Mutex)> lock(s_Mutex);
        CacheResourceInternal(filename, resource);
    }
    
private:
    std::shared_ptr<T> FindResourceInternal(const std::string& filename)
    {
        std::shared_ptr<T> resource = nullptr;
        std::string key = filename + CTypeId<T>::Name();
        if (s_CachedResources.find(key) != s_CachedResources.end()) {
            resource = std::dynamic_pointer_cast<T>(s_CachedResources[key]);
        }
        
        return resource;
    }
    
    void CacheResourceInternal(const std::string& filename, std::shared_ptr<T> resource)
    {
        std::stringstream key(filename + CTypeId<T>::Name());
        if (s_CachedResources.find(key.str()) == s_CachedResources.end()) {
            s_CachedResources[key.str()] = resource;
        }
    }

private:
    typedef std::unordered_map<std::string, std::shared_ptr<T> > TResourcesMap;
    static std::mutex s_Mutex;
    static TResourcesMap s_CachedResources;
};

template <class T>
typename CResourceCache<T>::TResourcesMap CResourceCache<T>::s_CachedResources;
    
template <class T>
typename std::mutex CResourceCache<T>::s_Mutex;

}; // namespace jam

#endif /* defined(CRESOURCECACHE_H) */


