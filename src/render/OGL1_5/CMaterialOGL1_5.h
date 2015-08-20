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
    
    virtual bool DepthEnable() const;
    virtual void DepthEnable(bool value);
    virtual bool DepthWriteEnable() const;
    virtual void DepthWriteEnable(bool value);
    virtual void DepthRange(double near, double far);
    virtual void DepthFunc(TestFuncs func);
    virtual TestFuncs DepthFunc();
    
    virtual bool StencilEnable() const;
    virtual void StencilEnable(bool value);
    virtual void StencilFunc(TestFuncs func, unsigned int ref, unsigned int mask);
    virtual void StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp);
    
    virtual const std::string& Hash();
    
private:
    void ApplyState(MaterialState state, MaterialState prevState);
    void HashMe();
    
private:
    IMaterial::MaterialState m_State;
    static std::stack<IMaterial::MaterialState> s_States;
    
    TUniInt m_UniInt;
    TUniFloat m_UniFloat;
    TUniInt m_UniIntVec;
    TUniFloat m_UniFloatVec;
    TUniMatrix4Float m_UniMatrixFloat;
    
    bool m_IsDirty;
    std::string m_Hash;
    bool m_IsBound;
};

}; // namespace jam

#endif /* defined(CMaterialOGL1_5_H) */

#endif /* CMATERIALOGL1_5_H || RENDER_OGLES1_1 */