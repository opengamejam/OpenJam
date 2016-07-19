//
//  Shaders_OGLES2.h
//  Arkanoid
//
//  Created by yev on 6/25/15.
//
//

#ifndef ISHADERSOURCE_H
#define ISHADERSOURCE_H

namespace jam
{
    
class IShaderSource
{
    JAM_OBJECT_BASE
public:    
    virtual const std::string& Vertex() const = 0;
    virtual const std::string& Fragment() const = 0;
    virtual const std::string& Geometry() const = 0;
    
    virtual bool Load(const std::string& filename) = 0;
};
    
}; // namespace jam

#endif // SHADERSSOURCES_H
