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
    
    virtual bool AssignImage(IImagePtr image) override;
    
    virtual void Filter(TextureFilters filter) override;
    virtual TextureFilters Filter() const override;
    
    virtual void Bind() override;
    virtual void Unbind() override;
    virtual const std::string& Hash() override;
    
private:
    bool IsValid() const;
    void HashMe();
    
private:
    uint32_t m_Id;
    
    TextureFilters m_Filter;
    std::string m_Hash;
    bool m_IsDirty;
};

}; // namespace jam

#endif /* CTEXTUREOGL2_0_H */

#endif /* RENDER_OGL2_0 || RENDER_OGLES2_0 */
