//
//  CMaterialOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#ifndef CMATERIALOGL1_5_H
#define CMATERIALOGL1_5_H

#include "IMaterial.h"

namespace jam
{

class CMaterialOGL1_5 : public IMaterial
{
public:
    CMaterialOGL1_5();
    virtual ~CMaterialOGL1_5();
    
    virtual void Bind();
    virtual void Unbind();
    
    virtual bool IsDefault();
    
    virtual bool BindUniform1i(int uniform, int value);
    virtual bool BindUniform1f(int uniform, float value);
    virtual bool BindUniform2i(int uniform, int value1, int value2);
    virtual bool BindUniform2f(int uniform, float value1, float value2);
    virtual bool BindUniformfv(int uniform, const std::vector<float>& value);
    virtual bool BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value);
    
    virtual const TUniInt& Uniformsi() const;
    virtual const TUniFloat& Uniformsf() const;
    virtual const TUniFloat& Uniformsfv() const;
    virtual const TUniMatrix4Float& UniformsMatrix4x4f() const;
    virtual void UpdateUniforms() const;
    
    virtual void Color(const CColor& color);
    virtual const CColor& Color() const;
    
    virtual PrimitiveTypes PrimitiveType() const;
    virtual void PrimitiveType(PrimitiveTypes primitiveType);
    
    virtual float LineWidth() const;
    virtual void LineWidth(float lineWidth);
    
    virtual bool CullFace() const;
    virtual void CullFace(bool isEnabled);
    
    virtual IStencilPtr Stencil() const;
    virtual void Stencil(IStencilPtr stencil);
    
    virtual bool DepthEnable() const;
    virtual void DepthEnable(bool value);
    
    virtual int UseFromParent() const;
    virtual void UseFromParent(int flags);
    
    virtual const std::string& Hash();
    
private:
    void HashMe();
    
private:
    bool m_IsDefault;
    TUniInt m_UniInt;
    TUniFloat m_UniFloat;
    TUniInt m_UniIntVec;
    TUniFloat m_UniFloatVec;
    TUniMatrix4Float m_UniMatrixFloat;
    CColor m_Color;
    float m_LineWidth;
    bool m_CullFace;
    int m_Flags;
    PrimitiveTypes m_PrimitiveType;
    
    IStencilPtr m_Stencil;
    bool m_DepthEnabled;
    
    bool m_IsDirty;
    std::string m_Hash;
};

}; // namespace jam

#endif /* defined(CMaterialOGL1_5_H) */

#endif /* CMATERIALOGL1_5_H || RENDER_OGLES1_1 */