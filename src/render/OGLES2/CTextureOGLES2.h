//
//  CTextureOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#ifndef CTEXTUREOGLES2_H
#define	CTEXTUREOGLES2_H

#include "ITexture.h"

namespace jam
{

class CTextureOGLES2 : public ITexture
{
public:
    CTextureOGLES2();
    virtual ~CTextureOGLES2();
    
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

#endif /* CTEXTUREOGLES2_H */

#endif /* RENDER_OGLES2 */
