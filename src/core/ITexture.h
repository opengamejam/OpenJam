//
//  ISprite.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef ITEXTURE_H
#define ITEXTURE_H

#include "Global.h"

namespace jam {

CLASS_PTR(IImage)

class ITexture {
    JAM_OBJECT_BASE
public:
    enum TextureType {
        TwoD = 0,
        Cubemap
    };

    enum TextureFilters {
        Linear = 0,
        Nearest,
        UseMipMaps
    };

public:
    ITexture() = default;
    virtual ~ITexture() = default;

    virtual bool AssignImage(IImagePtr image) = 0;

    virtual void Filter(TextureFilters filter) = 0;
    virtual TextureFilters Filter() const = 0;

    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual const std::string& Hash() = 0;
};

}; // namespace jam

#endif /* ITEXTURE_H */
