//
//  CMaterialVulkan.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
//#if defined(RENDER_VULKAN)

#include "CMaterialVulkan.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMaterialVulkan::CMaterialVulkan()
: m_IsDirty(true)
, m_IsBound(false)
{
}

CMaterialVulkan::~CMaterialVulkan()
{
}

void CMaterialVulkan::Bind()
{
    if (m_IsBound) {
        return;
    }
    
    m_IsBound = true;
}

void CMaterialVulkan::Unbind()
{
    if (!m_IsBound) {
        return;
    }
    
    m_IsBound = false;
}

const CColor4f& CMaterialVulkan::Color() const
{
    return m_State.color;
}

void CMaterialVulkan::Color(const CColor4f& color)
{
    m_State.color = color;
    m_IsDirty = true;
}

float CMaterialVulkan::LineWidth() const
{
    return m_State.lineWidth;
}

void CMaterialVulkan::LineWidth(float lineWidth)
{
    m_State.lineWidth = lineWidth;
    m_IsDirty = true;
}

IMaterial::PrimitiveTypes CMaterialVulkan::PrimitiveType() const
{
    return m_State.primitiveType;
}

void CMaterialVulkan::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_State.primitiveType = primitiveType;
    m_IsDirty = true;
}

bool CMaterialVulkan::CullFace() const
{
    return m_State.cullFace;
}

void CMaterialVulkan::CullFace(bool isEnabled)
{
    m_State.cullFace = isEnabled;
    m_IsDirty = true;
}

bool CMaterialVulkan::DepthEnable() const
{
    return m_State.depthTest.isEnabled;
}

void CMaterialVulkan::DepthEnable(bool value)
{
    m_State.depthTest.isEnabled = value;
    m_IsDirty = true;
}

bool CMaterialVulkan::DepthWriteEnable() const
{
    return m_State.depthTest.isWriteEnabled;
}

void CMaterialVulkan::DepthWriteEnable(bool value)
{
    m_State.depthTest.isWriteEnabled = value;
    m_IsDirty = true;
}

void CMaterialVulkan::DepthRange(double _near, double _far)
{
    m_State.depthTest.rangeNear = _near;
    m_State.depthTest.rangeFar = _far;
    m_IsDirty = true;
}

IMaterial::TestFuncs CMaterialVulkan::DepthFunc()
{
    return m_State.depthTest.func;
}

void CMaterialVulkan::DepthFunc(TestFuncs func)
{
    m_State.depthTest.func = func;
    m_IsDirty = true;
}

bool CMaterialVulkan::StencilEnable() const
{
    return m_State.stencilTest.isEnabled;
}

void CMaterialVulkan::StencilEnable(bool value)
{
    m_State.stencilTest.isEnabled = value;
    m_IsDirty = true;
}

void CMaterialVulkan::StencilFunc(TestFuncs func, uint32_t ref, uint32_t mask)
{
    m_State.stencilTest.func = func;
    m_State.stencilTest.ref = ref;
    m_State.stencilTest.mask = mask;
    m_IsDirty = true;
}

void CMaterialVulkan::StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp)
{
    m_State.stencilTest.failOp = failOp;
    m_State.stencilTest.zFailOp = zFailOp;
    m_State.stencilTest.zPassOp = zPassOp;
    m_IsDirty = true;
}

bool CMaterialVulkan::Opacity() const
{
    return m_State.opacity;
}

void CMaterialVulkan::Opacity(bool value)
{
    m_State.opacity = value;
    m_IsDirty = true;
}

const std::string& CMaterialVulkan::Hash()
{
    if (m_IsDirty) {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CMaterialVulkan::HashMe()
{
    std::stringstream ss;
    ss << PrimitiveType();
    ss << LineWidth();
    //ss << (Stencil() != nullptr);
    ss << DepthEnable();
    m_Hash = ss.str();
}

//#endif /* defined(RENDER_VULKAN) */


