//
//  CTextureOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#ifndef CTEXTUREOGL1_3_H
#define	CTEXTUREOGL1_3_H

#include "Global.h"
#include "ITexture.h"
#include "ImageDefs.h"

namespace jam
{

class CTextureOGL1_3 : public ITexture
{
public:
    CTextureOGL1_3();
    virtual ~CTextureOGL1_3();
    
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

#endif /* CTEXTUREOGL1_3_H */

#endif /* RENDER_OGL1_3 */
