//
//  CMaterialVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#ifndef CMATERIALVULKAN_H
#define CMATERIALVULKAN_H

#include "IMaterial.h"

namespace jam {

class CMaterialVulkan : public IMaterial
{
    JAM_OBJECT
public:
    CMaterialVulkan();
    virtual ~CMaterialVulkan();
    
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
    virtual void DepthRange(double _near, double _far) override;
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
    void HashMe();
    
private:
    IMaterial::MaterialState m_State;
    
    bool m_IsDirty;
    std::string m_Hash;
    bool m_IsBound;
};

}; // namespace jam

#endif /* CMATERIALVULKAN_H */

//#endif /* defined(RENDER_VULKAN) */
