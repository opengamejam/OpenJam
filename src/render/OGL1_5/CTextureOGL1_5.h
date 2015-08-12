//
//  CTextureOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CTEXTUREOGL1_5_H
#define	CTEXTUREOGL1_5_H

#include "Global.h"
#include "ITexture.h"
#include "ImageDefs.h"

namespace jam
{

class CTextureOGL1_5 : public ITexture
{
public:
    CTextureOGL1_5();
    virtual ~CTextureOGL1_5();
    
    virtual void Bind();
    virtual void Unbind();
    virtual bool IsValid() const;
    virtual void Filter(TextureFilters filter);
    virtual TextureFilters Filter() const;
    virtual bool AssignImage(IImagePtr image);
    virtual const std::string& Hash();
    
private:
    void HashMe();
    
private:
    unsigned int m_Id;
    
    TextureFilters m_Filter;
    std::string m_Hash;
    bool m_IsDirty;
};

}; // namespace jam

#endif /* CTEXTUREOGL1_5_H */

#endif /* RENDER_OGL1_5 || RENDER_OGLES1_1 */
