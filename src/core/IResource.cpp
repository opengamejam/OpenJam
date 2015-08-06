//
//  IResource.cpp
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "IResource.h"

using namespace jam;

IResource::IResource(const std::string& filename, const std::string& loaderName)
    : m_Filename(filename)
    , m_LoaderName(loaderName)
    , m_IsExternalData(false)
{}

IResource::IResource(const std::string& name, const IResource::TResourceData& data)
: m_Data(data)
, m_Filename(name)
, m_IsExternalData(true)
{}

IResource::~IResource()
{}

const std::string& IResource::Filename() const
{
    return m_Filename;
}

const std::string& IResource::LoaderName()
{
    return m_LoaderName;
}

const IResource::TResourceData& IResource::RawData()
{
    return m_Data;
}

bool IResource::Load()
{
    return Load(Filename());
}

bool IResource::Load(const std::string& filename)
{
    if (filename.empty())
    {
        return false;
    }
    
    if (!m_IsExternalData)
    {
        ILoader* loader = ILoader::GetLoader(LoaderName());
        return loader->Map(filename, m_Data);
    }
    else
    {
        return true;
    }
}

void IResource::Unload()
{
    TResourceData freeData;
    m_Data.swap(freeData);
}

const std::string& IResource::Hash()
{
    return Filename();
}