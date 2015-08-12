//
//  CTextureOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CTEXTUREOGL2_0_H
#define	CTEXTUREOGL2_0_H

#include "ITexture.h"

namespace jam
{

class CTextureOGL2_0 : public ITexture
{
public:
    CTextureOGL2_0();
    virtual ~CTextureOGL2_0();
    
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

#endif /* CTEXTUREOGL2_0_H */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */
