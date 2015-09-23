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
    
class IMaterial
{
public:    
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
        Points = 0,
        Lines,
        LinesLoop,
        LinesStrip,
        Triangles,
        TrianglesFan,
        TrianglesStrip
    };
    
    enum TestFuncs
    {
        Never = 0,
        Less,
        Equal,
        LEqual,
        Greater,
        NotEqual,
        GEqual,
        Always
    };
    
    enum Operations
    {
        Keep = 0,
        Replace,
        Incr,
        Decr,
        Invert,
        IncrWrap,
        DecrWrap
    };
    
    struct MaterialState
    {
        CColor color;
        float lineWidth;
        bool cullFace;
        PrimitiveTypes primitiveType;
        bool opacity;
        
        struct DepthTest
        {
            bool isEnabled;
            bool isWriteEnabled;
            double rangeNear;
            double rangeFar;
            TestFuncs func;
            
            DepthTest()
            : isEnabled(false)
            , isWriteEnabled(true)
            , rangeNear(0.0)
            , rangeFar(1.0)
            , func(Less)
            {
            }
        } depthTest;
        
        struct StencilTest
        {
            bool isEnabled;
            
            TestFuncs func;
            uint32_t ref;
            uint32_t mask;
            
            Operations failOp;
            Operations zFailOp;
            Operations zPassOp;
            
            StencilTest()
            : isEnabled(false)
            , func(Less)
            , ref(0) // TODO: default value
            , mask(0) // TODO
            , failOp(Keep)
            , zFailOp(Keep)
            , zPassOp(Keep)
            {
            }
        } stencilTest;
        
        MaterialState()
        : color(CColor(1.0f, 1.0f, 1.0f, 1.0f))
        , lineWidth(1.0f)
        , cullFace(true)
        , primitiveType(TrianglesStrip)
        , opacity(true)
        , depthTest()
        , stencilTest()
        {
        }
    };

public:
    IMaterial() = default;
    virtual ~IMaterial() = default;
    
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    
    virtual PrimitiveTypes PrimitiveType() const = 0;
    virtual void PrimitiveType(PrimitiveTypes primitiveType) = 0;
    
    virtual void Color(const CColor& color) = 0;
    virtual const CColor& Color() const = 0;
    
    virtual float LineWidth() const = 0;
    virtual void LineWidth(float lineWidth) = 0;
    
    virtual bool CullFace() const = 0;
    virtual void CullFace(bool isEnabled) = 0;
    
    virtual bool DepthEnable() const = 0;
    virtual void DepthEnable(bool value) = 0;
    virtual bool DepthWriteEnable() const = 0;
    virtual void DepthWriteEnable(bool value) = 0;
    virtual void DepthRange(double near, double far) = 0;
    virtual void DepthFunc(TestFuncs func) = 0;
    virtual TestFuncs DepthFunc() = 0;
    
    virtual bool StencilEnable() const = 0;
    virtual void StencilEnable(bool value) = 0;
    virtual void StencilFunc(TestFuncs func, uint32_t ref, uint32_t mask) = 0;
    virtual void StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp) = 0;
    
    virtual bool Opacity() const = 0;
    virtual void Opacity(bool value) = 0;
    
    virtual const std::string& Hash() = 0;
};

}; // namespace jam

#endif	/* IMATERIAL_H */

