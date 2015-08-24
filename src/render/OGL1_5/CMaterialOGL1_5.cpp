//
//  CMaterialOGL1_5.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_5) || defined(RENDER_OGLES1_1)

#include "CMaterialOGL1_5.h"
#include "IStencil.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL unsigned int ConvertTestFunc(IMaterial::TestFuncs func);
INL unsigned int ConvertOperation(IMaterial::Operations op);

std::stack<IMaterial::MaterialState> CMaterialOGL1_5::s_States;

CMaterialOGL1_5::CMaterialOGL1_5()
: m_IsDirty(true)
, m_IsBound(false)
{
    
}

CMaterialOGL1_5::~CMaterialOGL1_5()
{
}

void CMaterialOGL1_5::Bind()
{
    if (m_IsBound)
    {
        return;
    }
    
    IMaterial::MaterialState prevState;
    if (!s_States.empty())
    {
        prevState = s_States.top();
    }
    s_States.push(m_State);
    
    ApplyState(m_State, prevState);
    
    m_IsBound = true;
}

void CMaterialOGL1_5::Unbind()
{
    if (!m_IsBound)
    {
        return;
    }
    
    s_States.pop();
    
    IMaterial::MaterialState prevState;
    if (!s_States.empty())
    {
        prevState = s_States.top();
    }
    ApplyState(prevState, m_State);
    
    m_IsBound = false;
}

const CColor& CMaterialOGL1_5::Color() const
{
    return m_State.color;
}

void CMaterialOGL1_5::Color(const CColor& color)
{
    m_State.color = color;
    m_IsDirty = true;
}

float CMaterialOGL1_5::LineWidth() const
{
    return m_State.lineWidth;
}

void CMaterialOGL1_5::LineWidth(float lineWidth)
{
    m_State.lineWidth = lineWidth;
    m_IsDirty = true;
}

IMaterial::PrimitiveTypes CMaterialOGL1_5::PrimitiveType() const
{
    return m_State.primitiveType;
}

void CMaterialOGL1_5::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_State.primitiveType = primitiveType;
    m_IsDirty = true;
}

bool CMaterialOGL1_5::CullFace() const
{
    return m_State.cullFace;
}

void CMaterialOGL1_5::CullFace(bool isEnabled)
{
    m_State.cullFace = isEnabled;
    m_IsDirty = true;
}

bool CMaterialOGL1_5::DepthEnable() const
{
    return m_State.depthTest.isEnabled;
}

void CMaterialOGL1_5::DepthEnable(bool value)
{
    m_State.depthTest.isEnabled = value;
    m_IsDirty = true;
}

bool CMaterialOGL1_5::DepthWriteEnable() const
{
    return m_State.depthTest.isWriteEnabled;
}

void CMaterialOGL1_5::DepthWriteEnable(bool value)
{
    m_State.depthTest.isWriteEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL1_5::DepthRange(double near, double far)
{
    m_State.depthTest.rangeNear = near;
    m_State.depthTest.rangeFar = far;
    m_IsDirty = true;
}

IMaterial::TestFuncs CMaterialOGL1_5::DepthFunc()
{
    return m_State.depthTest.func;
}

void CMaterialOGL1_5::DepthFunc(TestFuncs func)
{
    m_State.depthTest.func = func;
    m_IsDirty = true;
}

bool CMaterialOGL1_5::StencilEnable() const
{
    return m_State.stencilTest.isEnabled;
}

void CMaterialOGL1_5::StencilEnable(bool value)
{
    m_State.stencilTest.isEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL1_5::StencilFunc(TestFuncs func, unsigned int ref, unsigned int mask)
{
    m_State.stencilTest.func = func;
    m_State.stencilTest.ref = ref;
    m_State.stencilTest.mask = mask;
    m_IsDirty = true;
}

void CMaterialOGL1_5::StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp)
{
    m_State.stencilTest.failOp = failOp;
    m_State.stencilTest.zFailOp = zFailOp;
    m_State.stencilTest.zPassOp = zPassOp;
    m_IsDirty = true;
}

bool CMaterialOGL1_5::Opacity() const
{
    return m_State.opacity;
}

void CMaterialOGL1_5::Opacity(bool value)
{
    m_State.opacity = value;
    m_IsDirty = true;
}

const std::string& CMaterialOGL1_5::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

bool CMaterialOGL1_5::BindUniform1i(int uniform, int value)
{
    m_UniInt[uniform] = std::vector<int>(value);
    
    return true;
}

bool CMaterialOGL1_5::BindUniform1f(int uniform, float value)
{
    m_UniFloat[uniform] = std::vector<float>(value);
    
    return true;
}

bool CMaterialOGL1_5::BindUniform2i(int uniform, int value1, int value2)
{
    m_UniInt[uniform] = std::vector<int>(value1, value2);
    
    return true;
}

bool CMaterialOGL1_5::BindUniform2f(int uniform, float value1, float value2)
{
    m_UniFloat[uniform] = std::vector<float>(value1, value2);
    
    return true;
}

bool CMaterialOGL1_5::BindUniformfv(int uniform, const std::vector<float>& value)
{
    m_UniFloatVec[uniform] = value;
    
    return true;
}

bool CMaterialOGL1_5::BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value)
{
    m_UniMatrixFloat[uniform] = value;
    
    return true;
}

const IMaterial::TUniInt& CMaterialOGL1_5::Uniformsi() const
{
    return m_UniInt;
}

const IMaterial::TUniFloat& CMaterialOGL1_5::Uniformsf() const
{
    return m_UniFloat;
}

const IMaterial::TUniFloat& CMaterialOGL1_5::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IMaterial::TUniMatrix4Float& CMaterialOGL1_5::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

void CMaterialOGL1_5::UpdateUniforms() const
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CMaterialOGL1_5::ApplyState(IMaterial::MaterialState state, IMaterial::MaterialState prevState)
{
    if (state.lineWidth != prevState.lineWidth)
    {
        glLineWidth(state.lineWidth);
    }
    
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
    if (state.depthTest.rangeNear != prevState.depthTest.rangeNear ||
        state.depthTest.rangeFar != prevState.depthTest.rangeFar)
    {
        glDepthRangef(state.depthTest.rangeNear, state.depthTest.rangeFar);
    }
    
    // Stencil
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
}

void CMaterialOGL1_5::HashMe()
{
    std::stringstream ss;    
    ss << PrimitiveType();
    ss << LineWidth();
    //ss << (Stencil() != nullptr);
    ss << DepthEnable();
    m_Hash = ss.str();
}

INL unsigned int ConvertTestFunc(IMaterial::TestFuncs func)
{
    unsigned int stencilFunc = GL_NEVER;
    switch (func)
    {
        case IMaterial::Never:
            stencilFunc = GL_NEVER;
            break;
            
        case IMaterial::Less:
            stencilFunc = GL_LESS;
            break;
            
        case IMaterial::Equal:
            stencilFunc = GL_EQUAL;
            break;
            
        case IMaterial::LEqual:
            stencilFunc = GL_LEQUAL;
            break;
            
        case IMaterial::Greater:
            stencilFunc = GL_GREATER;
            break;
            
        case IMaterial::NotEqual:
            stencilFunc = GL_NOTEQUAL;
            break;
            
        case IMaterial::GEqual:
            stencilFunc = GL_GEQUAL;
            break;
            
        case IMaterial::Always:
            stencilFunc = GL_ALWAYS;
            break;
    };
    
    return stencilFunc;
}

INL unsigned int ConvertOperation(IMaterial::Operations op)
{
    unsigned int operation = GL_KEEP;
    switch (op)
    {
        case IMaterial::Keep:
            operation = GL_KEEP;
            break;

        case IMaterial::Replace:
            operation = GL_REPLACE;
            break;
            
        case IMaterial::Incr:
            operation = GL_INCR;
            break;
            
        case IMaterial::Decr:
            operation = GL_DECR;
            break;
            
        case IMaterial::Invert:
            // TODO:
            break;
            
        case IMaterial::IncrWrap:
            // TODO:
            break;
            
        case IMaterial::DecrWrap:
            // TODO:
            break;
    };
    
    return operation;
}

#endif // RENDER_OGL1_5 || RENDER_OGLES1_1