//
//  CMaterialOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#ifndef CMATERIALOGL2_0_H
#define CMATERIALOGL2_0_H

#include "IMaterial.h"

namespace jam
{

class CMaterialOGL2_0 : public IMaterial
{
public:
    CMaterialOGL2_0();
    virtual ~CMaterialOGL2_0();
    
    virtual void Bind() override;
    virtual void Unbind() override;
    
    virtual void Color(const CColor& color) override;
    virtual const CColor& Color() const override;
    
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

#endif /* defined(CMATERIALOGL2_0_H) */

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0
