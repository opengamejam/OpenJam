//
//  CImageRaw.h
//  OpenJam
//
//  Created by yev on 10/6/14.
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef __OpenJam__CImageRaw__
#define __OpenJam__CImageRaw__

#include "Global.h"
#include "IIMage.h"

namespace jam
{

class CImageRaw : public IImage
{
public:
    virtual ~CImageRaw();
    
    virtual bool            Load();
    virtual unsigned int    Width() const;
    virtual unsigned int    Height() const;
    virtual unsigned int    Bpp() const;
    virtual unsigned int    MipsCount() const;
    virtual unsigned int    IsCompressed() const;
    virtual TexelFormats    TexelFormat() const;
    virtual TexelTypes      TexelType() const;
    
    CImageRaw(const std::string& filename,
              const IResource::TResourceData& data,
              unsigned int width,
              unsigned int height,
              const TexelProps& texelProps);
    
    
private:
    unsigned int            m_Width;
    unsigned int            m_Height;
    TResourceData          m_Data;
    TexelProps              m_TexelProps;
};

}; // namespace jam

#endif /* defined(__OpenJam__CImageRaw__) */
