//
//  CShaderSourceSprite.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef CSHADERSOURCESPRITE_H
#define CSHADERSOURCESPRITE_H

#include "Global.h"
#include "IShaderSource.h"

namespace jam
{

class CShaderSourceSprite : public IShaderSource
{
public:
    virtual const std::string& Vertex() const;
    virtual const std::string& Fragment() const;
    virtual const std::string& Geometry() const;
    
    virtual bool Load(const std::string& filename);
    
private:
    static const std::string s_VertexShader;
    static const std::string s_FragmentShader;
    static const std::string s_GeomentryShader;
};
    
}; // namespace jam

#endif /* defined(CSHADERSOURCESPRITE_H) */