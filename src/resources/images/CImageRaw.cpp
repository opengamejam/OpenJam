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
                     unsigned int width,
                     unsigned int height,
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

unsigned int CImageRaw::Width() const
{
    return m_Width;
}

unsigned int CImageRaw::Height() const
{
    return m_Height;
}

unsigned int CImageRaw::Bpp() const
{
    return m_TexelProps.bpp;
}

unsigned int CImageRaw::MipsCount() const
{
    return 1;
}

unsigned int CImageRaw::IsCompressed() const
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