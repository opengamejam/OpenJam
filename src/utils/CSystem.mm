/* 
 * File:   CSystem.cpp
 * Author: yevgeniy.logachev
 */

#include "CSystem.h"

#if defined(OS_IPHONE) || defined(OS_MAC)

unsigned long CSystem::GetTickCount()
{
    struct timeval t;
    gettimeofday(&t, 0);
    return ((t.tv_sec * 1000) + (t.tv_usec / 1000));

    return 0;
}

const std::string& CSystem::GetBundlePath()
{
    if (g_bundlePath.empty()) {
#if defined(__OBJC__) && defined(OS_IPHONE)
        NSString* path = [[NSBundle mainBundle] resourcePath];

        g_bundlePath = std::string([path UTF8String]) + std::string("/"); // + std::string("/../Documents/");
#elif defined(__OBJC__) && defined(OS_MAC)
        NSString* path = [[NSBundle mainBundle] bundlePath];

        g_bundlePath = std::string([path UTF8String]) + std::string("/");
#endif
    }

    return g_bundlePath;
}

void CSystem::SetBundlePath(const std::string& path)
{
    g_bundlePath = path;
}

bool CSystem::IsFileExists(const std::string& name)
{
    return true;
    //return _access( name.c_str(), 0 ) != -1;
}

#endif // #if defined(OS_IPHONE) || defined(OS_MAC)
