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
    CImageRaw(const std::string& filename,
              const IResource::TResourceData& data,
              uint32_t width,
              uint32_t height,
              const TexelProps& texelProps);
    virtual ~CImageRaw();
    
    virtual bool Load() override;
    virtual uint32_t Width() const override;
    virtual uint32_t Height() const override;
    virtual uint32_t Bpp() const override;
    virtual uint32_t MipsCount() const override;
    virtual bool IsCompressed() const override;
    virtual TexelFormats TexelFormat() const override;
    virtual TexelTypes TexelType() const override;
    
private:
    uint32_t m_Width;
    uint32_t m_Height;
    TResourceData m_Data;
    TexelProps m_TexelProps;
};

}; // namespace jam

#endif /* defined(__OpenJam__CImageRaw__) */
