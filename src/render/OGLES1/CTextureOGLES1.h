//
//  CTextureOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#ifndef CTEXTUREOGLES1_H
#define	CTEXTUREOGLES1_H

#include "Global.h"
#include "ITexture.h"
#include "ImageDefs.h"

namespace jam
{

class CTextureOGLES1 : public ITexture
{
public:
    CTextureOGLES1();
    virtual ~CTextureOGLES1();
    
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

#endif /* CTEXTUREOGLES1_H */

#endif /* RENDER_OGLES1 */
