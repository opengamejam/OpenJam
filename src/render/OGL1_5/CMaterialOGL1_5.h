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
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
    virtual void Color(const CColor4f& color) override;
    virtual const CColor4f& Color() const override;
    
    virtual PrimitiveTypes PrimitiveType() const override;
    virtual void PrimitiveType(PrimitiveTypes primitiveType) override;
    
    virtual float LineWidth() const override;
    virtual void LineWidth(float lineWidth) override;
    
    virtual bool CullFace() const override;
    virtual void CullFace(bool isEnabled) override;
    
    virtual bool DepthEnable() const override;
    virtual void DepthEnable(bool value) override;
    virtual bool DepthWriteEnable() const override;
    virtual void DepthWriteEnable(bool value) override;
    virtual void DepthRange(double near, double far) override;
    virtual void DepthFunc(TestFuncs func) override;
    virtual TestFuncs DepthFunc() override;
    
    virtual bool StencilEnable() const override;
    virtual void StencilEnable(bool value) override;
    virtual void StencilFunc(TestFuncs func, uint32_t ref, uint32_t mask) override;
    virtual void StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp) override;
    
    virtual bool Opacity() const override;
    virtual void Opacity(bool value) override;
    
    virtual const std::string& Hash() override;
    
private:
    void ApplyState(MaterialState state, MaterialState prevState);
    void HashMe();
    
private:
    IMaterial::MaterialState m_State;
    static std::stack<IMaterial::MaterialState> s_States;
    
    bool m_IsDirty;
    std::string m_Hash;
    bool m_IsBound;
};

}; // namespace jam

#endif /* defined(CMaterialOGL1_5_H) */

#endif /* CMATERIALOGL1_5_H || RENDER_OGLES1_1 */