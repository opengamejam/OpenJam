//
//  CResourceCache.cpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 9/21/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CResourceCache.hpp"

using namespace jam;

CResourceCache::TResourcesMap CResourceCache::s_CachedResources;
unsigned int CResourceCache::s_UniqIndex = 0;

CResourceCache::CResourceCache()
{

}

CResourceCache::~CResourceCache()
{

}