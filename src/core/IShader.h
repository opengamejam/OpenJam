//
//  IScene.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef ISHADER_H
#define	ISHADER_H

#include "Global.h"

namespace jam
{

CLASS_PTR(IShaderSource);
    
class IShader
{
public:
    enum ShaderType
    {
        Vertex = 0,
        Fragment,
        Geometry
    };
    
public:
    IShader() = default;
    virtual ~IShader() = default;

    virtual unsigned int Id() = 0;
    virtual bool Compile(const std::string& source, ShaderType shaderType) = 0;
    virtual bool IsCompiled() const = 0;
    virtual ShaderType Type() const = 0;
};

}; // namespace jam

#endif	/* ISHADER_H */

