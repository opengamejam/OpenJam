//
//  IResource.cpp
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "IResource.h"
#include "CVirtualFileSystem.h"

using namespace jam;
using namespace vfspp;

IResource::IResource(const std::string& filename)
    : m_Filename(filename)
    , m_IsExternalData(false)
{
}

IResource::IResource(const std::string& name, const IResource::TResourceData& data)
    : m_Data(data)
    , m_Filename(name)
    , m_IsExternalData(true)
{
}

IResource::~IResource()
{
}

const std::string& IResource::Filename() const
{
    return m_Filename;
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
    if (filename.empty()) {
        return false;
    }

    if (!m_IsExternalData) {
        bool result = false;

        CVirtualFileSystemPtr vfs = vfs_get_global();
        IFilePtr file = vfs->OpenFile(filename, IFile::In);
        if (file && file->IsOpened()) {
            uint64_t fileSize = file->Size();
            m_Data.resize((size_t)(fileSize + 1));
            memset(&m_Data[0], 0, (size_t)(fileSize + 1));

            result = (file->Read(&m_Data[0], fileSize) == fileSize);

            vfs->CloseFile(file);
        }

        return result;
    } else {
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
