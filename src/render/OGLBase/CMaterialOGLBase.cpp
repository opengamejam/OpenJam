//
//  CMaterialOGLBase.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
    defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
    defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CMaterialOGLBase.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

std::stack<IMaterial::MaterialState> CMaterialOGLBase::s_States;

CMaterialOGLBase::CMaterialOGLBase()
    : m_IsDirty(true)
    , m_IsBound(false)
{
}

CMaterialOGLBase::~CMaterialOGLBase()
{
}

void CMaterialOGLBase::Bind()
{
    if (m_IsBound) {
        return;
    }

    IMaterial::MaterialState prevState;
    if (!s_States.empty()) {
        prevState = s_States.top();
    }
    s_States.push(m_State);

    ApplyState(m_State, prevState);

    m_IsBound = true;
}

void CMaterialOGLBase::Unbind()
{
    if (!m_IsBound) {
        return;
    }

    s_States.pop();

    IMaterial::MaterialState prevState;
    if (!s_States.empty()) {
        prevState = s_States.top();
    }
    ApplyState(prevState, m_State);

    m_IsBound = false;
}

const CColor4f& CMaterialOGLBase::Color() const
{
    return m_State.color;
}

void CMaterialOGLBase::Color(const CColor4f& color)
{
    m_State.color = color;
    m_IsDirty = true;
}

float CMaterialOGLBase::LineWidth() const
{
    return m_State.lineWidth;
}

void CMaterialOGLBase::LineWidth(float lineWidth)
{
    m_State.lineWidth = lineWidth;
    m_IsDirty = true;
}

IMaterial::PrimitiveTypes CMaterialOGLBase::PrimitiveType() const
{
    return m_State.primitiveType;
}

void CMaterialOGLBase::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_State.primitiveType = primitiveType;
    m_IsDirty = true;
}

bool CMaterialOGLBase::CullFace() const
{
    return m_State.cullFace;
}

void CMaterialOGLBase::CullFace(bool isEnabled)
{
    m_State.cullFace = isEnabled;
    m_IsDirty = true;
}

bool CMaterialOGLBase::DepthEnable() const
{
    return m_State.depthTest.isEnabled;
}

void CMaterialOGLBase::DepthEnable(bool value)
{
    m_State.depthTest.isEnabled = value;
    m_IsDirty = true;
}

bool CMaterialOGLBase::DepthWriteEnable() const
{
    return m_State.depthTest.isWriteEnabled;
}

void CMaterialOGLBase::DepthWriteEnable(bool value)
{
    m_State.depthTest.isWriteEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGLBase::DepthRange(double _near, double _far)
{
    m_State.depthTest.rangeNear = _near;
    m_State.depthTest.rangeFar = _far;
    m_IsDirty = true;
}

IMaterial::TestFuncs CMaterialOGLBase::DepthFunc()
{
    return m_State.depthTest.func;
}

void CMaterialOGLBase::DepthFunc(TestFuncs func)
{
    m_State.depthTest.func = func;
    m_IsDirty = true;
}

bool CMaterialOGLBase::StencilEnable() const
{
    return m_State.stencilTest.isEnabled;
}

void CMaterialOGLBase::StencilEnable(bool value)
{
    m_State.stencilTest.isEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGLBase::StencilFunc(TestFuncs func, uint32_t ref, uint32_t mask)
{
    m_State.stencilTest.func = func;
    m_State.stencilTest.ref = ref;
    m_State.stencilTest.mask = mask;
    m_IsDirty = true;
}

void CMaterialOGLBase::StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp)
{
    m_State.stencilTest.failOp = failOp;
    m_State.stencilTest.zFailOp = zFailOp;
    m_State.stencilTest.zPassOp = zPassOp;
    m_IsDirty = true;
}

bool CMaterialOGLBase::Opacity() const
{
    return m_State.opacity;
}

void CMaterialOGLBase::Opacity(bool value)
{
    m_State.opacity = value;
    m_IsDirty = true;
}

const std::string& CMaterialOGLBase::Hash()
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

void CMaterialOGLBase::ApplyState(IMaterial::MaterialState state, IMaterial::MaterialState prevState)
{
    if (state.lineWidth != prevState.lineWidth) {
        glLineWidth(state.lineWidth);
    }

    if (state.cullFace != prevState.cullFace) {
        if (state.cullFace) {
            glEnable(GL_CULL_FACE);
        } else {
            glDisable(GL_CULL_FACE);
        }
    }

    // Depth
    /*if (state.depthTest.isEnabled != prevState.depthTest.isEnabled) {
        if (state.depthTest.isEnabled) {
            glEnable(GL_DEPTH_TEST);
        } else {
            glDisable(GL_DEPTH_TEST);
        }
    }
    if (state.depthTest.isWriteEnabled != prevState.depthTest.isWriteEnabled) {
        glDepthMask(state.depthTest.isWriteEnabled ? GL_TRUE : GL_FALSE);
    }
    if (state.depthTest.func != prevState.depthTest.func) {
        glDepthFunc(ConvertTestFunc(state.depthTest.func));
    }
    if (state.depthTest.rangeNear != prevState.depthTest.rangeNear || state.depthTest.rangeFar != prevState.depthTest.rangeFar) {
        glDepthRangef(static_cast<GLfloat>(state.depthTest.rangeNear),
            static_cast<GLfloat>(state.depthTest.rangeFar));
    }*/

    // Stencil
    if (state.stencilTest.isEnabled != prevState.stencilTest.isEnabled) {
        if (state.stencilTest.isEnabled) {
            glEnable(GL_STENCIL_TEST);
        } else {
            glDisable(GL_STENCIL_TEST);
        }
    }

#if !defined(OS_KOS)
    if (state.stencilTest.func != prevState.stencilTest.func || state.stencilTest.ref != prevState.stencilTest.ref || state.stencilTest.mask != prevState.stencilTest.mask) {
        glStencilFunc(ConvertTestFunc(state.stencilTest.func), state.stencilTest.ref, state.stencilTest.mask);
    }
    if (state.stencilTest.failOp != prevState.stencilTest.failOp || state.stencilTest.zFailOp != prevState.stencilTest.zFailOp || state.stencilTest.zPassOp != prevState.stencilTest.zPassOp) {
        glStencilOp(ConvertOperation(state.stencilTest.failOp),
            ConvertOperation(state.stencilTest.zFailOp),
            ConvertOperation(state.stencilTest.zPassOp));
    }
#endif

    if (state.opacity != prevState.opacity) {
        if (state.opacity) {
            glDisable(GL_BLEND);
        } else {
            glEnable(GL_BLEND);
        }
    }

    {
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        //glShadeModel(GL_SMOOTH);

        //glFrontFace(GL_CCW);
    }

    //assert(glGetError() == GL_NO_ERROR);
}

/* KOS support
 
 
 void CMaterialOGL1_3::ApplyState(IMaterial::MaterialState state, IMaterial::MaterialState prevState)
{
#if !defined(OS_KOS)
    if (state.lineWidth != prevState.lineWidth)
    {
        glLineWidth(state.lineWidth);
    }
#endif
    
    if (state.cullFace != prevState.cullFace)
    {
        if (state.cullFace)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
    }
    
    // Depth
    if (state.depthTest.isEnabled != prevState.depthTest.isEnabled)
    {
        if (state.depthTest.isEnabled)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
    }
    if (state.depthTest.isWriteEnabled != prevState.depthTest.isWriteEnabled)
    {
        glDepthMask(state.depthTest.isWriteEnabled ? GL_TRUE : GL_FALSE);
    }
    if (state.depthTest.func != prevState.depthTest.func)
    {
        glDepthFunc(ConvertTestFunc(state.depthTest.func));
    }
#if !defined(OS_KOS)
    if (state.depthTest.rangeNear != prevState.depthTest.rangeNear ||
        state.depthTest.rangeFar != prevState.depthTest.rangeFar)
    {
        glDepthRange(state.depthTest.rangeNear, state.depthTest.rangeFar);
    }
#endif
    
    // Stencil
#ifdef GL_STENCIL_TEST
    if (state.stencilTest.isEnabled != prevState.stencilTest.isEnabled)
    {
        if (state.stencilTest.isEnabled)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
    }
    
#if !defined(OS_KOS)
    if (state.stencilTest.func != prevState.stencilTest.func ||
        state.stencilTest.ref != prevState.stencilTest.ref ||
        state.stencilTest.mask != prevState.stencilTest.mask)
    {
        glStencilFunc(ConvertTestFunc(state.stencilTest.func), state.stencilTest.ref, state.stencilTest.mask);
    }
    if (state.stencilTest.failOp != prevState.stencilTest.failOp ||
        state.stencilTest.zFailOp != prevState.stencilTest.zFailOp ||
        state.stencilTest.zPassOp != prevState.stencilTest.zPassOp)
    {
        glStencilOp(ConvertOperation(state.stencilTest.failOp),
                    ConvertOperation(state.stencilTest.zFailOp),
                    ConvertOperation(state.stencilTest.zPassOp));
    }
#endif // !defined(OS_KOS)
    
#endif
    
    // Blending
    if (state.opacity != prevState.opacity)
    {
        if (state.opacity)
        {
            glDisable(GL_BLEND);
        }
        else
        {
            glEnable(GL_BLEND);
        }
    }
    
    {
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glShadeModel(GL_SMOOTH);
        
        //glFrontFace(GL_CCW);
    }
    
    //assert(glGetError() == GL_NO_ERROR);
}*/

void CMaterialOGLBase::HashMe()
{
    std::stringstream ss;
    ss << PrimitiveType();
    ss << LineWidth();
    //ss << (Stencil() != nullptr);
    ss << DepthEnable();
    m_Hash = ss.str();
}

#endif /* defined(RENDER_OGL1_3) || defined(RENDER_OGLES1_0) || \
          defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1) || \
          defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0) */
