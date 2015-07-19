//
//  IMaterial.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#ifndef IMATERIAL_H
#define	IMATERIAL_H

#include "Global.h"
#include "CColor.h"
#include "CMatrix4x4.h"

namespace jam
{
    
CLASS_PTR(IStencil);
    
class IMaterial
{
public:
    typedef std::map< int, std::vector<int> > TUniInt;
    typedef std::map< int, std::vector<float> > TUniFloat;
    typedef std::map< int, CMatrix4x4f > TUniMatrix4Float;
    typedef std::list<TUniInt> TUniIntList;
    typedef std::list<TUniFloat> TUniFloatList;
    
    enum TextureSamples
    {
        DiffuseTexture = 0,
        NormalTexture,
        SpecularTexture,
        EnvironmentTexture,
        Pass1Texture,
        Pass2Texture,
        MaxSamplingTextures = 6
    };
    
    enum PrimitiveTypes
    {
        PT_Points = 0,
        PT_Lines,
        PT_LinesLoop,
        PT_LinesStrip,
        PT_Triangles,
        PT_TrianglesFan,
        PT_TrianglesStrip
    };
    
    enum Flags  // TODO: to remove
    {
        NoneFlag = 0,
        ShaderFlag = 0x01,
        PrimitiveFlag = 0x02,
        ColorFlag = 0x04,
        LineWidthFlag = 0x08,
        StencilFlag = 0x10
    };
    
public:
    IMaterial() = default;
    virtual ~IMaterial() = default;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
    virtual bool IsDefault() = 0;
    
    virtual bool BindUniform1i(int uniform, int value) = 0;
    virtual bool BindUniform1f(int uniform, float value) = 0;
    virtual bool BindUniform2i(int uniform, int value1, int value2) = 0;
    virtual bool BindUniform2f(int uniform, float value1, float value2) = 0;
    virtual bool BindUniformfv(int uniform, const std::vector<float>& value) = 0;
    virtual bool BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value) = 0;
    
    virtual const TUniInt& Uniformsi() const = 0;
    virtual const TUniFloat& Uniformsf() const = 0;
    virtual const TUniFloat& Uniformsfv() const = 0;
    virtual const TUniMatrix4Float& UniformsMatrix4x4f() const = 0;
    virtual void UpdateUniforms() const = 0;
    
    virtual PrimitiveTypes PrimitiveType() const = 0;
    virtual void PrimitiveType(PrimitiveTypes primitiveType) = 0;
    
    virtual void Color(const CColor& color) = 0;
    virtual const CColor& Color() const = 0;
    
    virtual float LineWidth() const = 0;
    virtual void LineWidth(float lineWidth) = 0;
    
    virtual IStencilPtr Stencil() const = 0;
    virtual void Stencil(IStencilPtr stencil) = 0;
    
    virtual bool DepthEnable() const = 0;
    virtual void DepthEnable(bool value) = 0;
    
    // TODO: to remove
    virtual int UseFromParent() const = 0;
    virtual void UseFromParent(int flags) = 0;
};

}; // namespace jam

#endif	/* IMATERIAL_H */

