//
//  CImageRaw.cpp
//  OpenJam
//
//  Created by yev on 10/6/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CImageRaw.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CImageRaw::CImageRaw(const std::string& filename,
    const IResource::TResourceData& data,
    uint32_t width,
    uint32_t height,
    const TexelProps& texelProps)
    : IImage(filename, data)
    , m_Width(width)
    , m_Height(height)
    , m_TexelProps(texelProps)
{
}

CImageRaw::~CImageRaw()
{
}

bool CImageRaw::Load()
{
    return true;
}

uint32_t CImageRaw::Width() const
{
    return m_Width;
}

uint32_t CImageRaw::Height() const
{
    return m_Height;
}

uint32_t CImageRaw::Bpp() const
{
    return m_TexelProps.bpp;
}

uint32_t CImageRaw::MipsCount() const
{
    return 1;
}

bool CImageRaw::IsCompressed() const
{
    return false;
}

TexelFormats CImageRaw::TexelFormat() const
{
    return m_TexelProps.texelFormat;
}

TexelTypes CImageRaw::TexelType() const
{
    return m_TexelProps.texelType;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
