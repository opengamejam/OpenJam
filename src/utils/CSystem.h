/*
 * File:   CSystem.h
 * Author: yevgeniy.logachev
 */

#ifndef CSYSTEM_H
#define	CSYSTEM_H

#include "Global.h"

namespace CSystem
{
    static std::string g_bundlePath = "";
    
    unsigned long       GetTickCount();
    
    const std::string&  GetBundlePath();
    void                SetBundlePath(const std::string& path);
	bool                IsFileExists( const std::string& name );
};

#endif	/* CSYSTEM_H */

