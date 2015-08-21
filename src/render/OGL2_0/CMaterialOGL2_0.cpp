//
//  CMaterialOGL2_0.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL2_0) || defined(RENDER_OGLES2_0)

#include "CMaterialOGL2_0.h"
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

std::stack<IMaterial::MaterialState> CMaterialOGL2_0::s_States;

CMaterialOGL2_0::CMaterialOGL2_0()
: m_IsDirty(true)
, m_IsBound(false)
{
    
}

CMaterialOGL2_0::~CMaterialOGL2_0()
{
}

void CMaterialOGL2_0::Bind()
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

void CMaterialOGL2_0::Unbind()
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

const CColor& CMaterialOGL2_0::Color() const
{
    return m_State.color;
}

void CMaterialOGL2_0::Color(const CColor& color)
{
    m_State.color = color;
    m_IsDirty = true;
}

float CMaterialOGL2_0::LineWidth() const
{
    return m_State.lineWidth;
}

void CMaterialOGL2_0::LineWidth(float lineWidth)
{
    m_State.lineWidth = lineWidth;
    m_IsDirty = true;
}

IMaterial::PrimitiveTypes CMaterialOGL2_0::PrimitiveType() const
{
    return m_State.primitiveType;
}

void CMaterialOGL2_0::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_State.primitiveType = primitiveType;
    m_IsDirty = true;
}

bool CMaterialOGL2_0::CullFace() const
{
    return m_State.cullFace;
}

void CMaterialOGL2_0::CullFace(bool isEnabled)
{
    m_State.cullFace = isEnabled;
    m_IsDirty = true;
}

bool CMaterialOGL2_0::DepthEnable() const
{
    return m_State.depthTest.isEnabled;
}

void CMaterialOGL2_0::DepthEnable(bool value)
{
    m_State.depthTest.isEnabled = value;
    m_IsDirty = true;
}

bool CMaterialOGL2_0::DepthWriteEnable() const
{
    return m_State.depthTest.isWriteEnabled;
}

void CMaterialOGL2_0::DepthWriteEnable(bool value)
{
    m_State.depthTest.isWriteEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL2_0::DepthRange(double near, double far)
{
    m_State.depthTest.rangeNear = near;
    m_State.depthTest.rangeFar = far;
    m_IsDirty = true;
}

IMaterial::TestFuncs CMaterialOGL2_0::DepthFunc()
{
    return m_State.depthTest.func;
}

void CMaterialOGL2_0::DepthFunc(TestFuncs func)
{
    m_State.depthTest.func = func;
    m_IsDirty = true;
}

bool CMaterialOGL2_0::StencilEnable() const
{
    return m_State.stencilTest.isEnabled;
}

void CMaterialOGL2_0::StencilEnable(bool value)
{
    m_State.stencilTest.isEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL2_0::StencilFunc(TestFuncs func, unsigned int ref, unsigned int mask)
{
    m_State.stencilTest.func = func;
    m_State.stencilTest.ref = ref;
    m_State.stencilTest.mask = mask;
    m_IsDirty = true;
}

void CMaterialOGL2_0::StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp)
{
    m_State.stencilTest.failOp = failOp;
    m_State.stencilTest.zFailOp = zFailOp;
    m_State.stencilTest.zPassOp = zPassOp;
    m_IsDirty = true;
}

const std::string& CMaterialOGL2_0::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

bool CMaterialOGL2_0::BindUniform1i(int uniform, int value)
{
    if (uniform >= 0)
    {
        m_UniInt[uniform] = std::vector<int>(value);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGL2_0::BindUniform1f(int uniform, float value)
{
    if (uniform >= 0)
    {
        m_UniFloat[uniform] = std::vector<float>(value);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGL2_0::BindUniform2i(int uniform, int value1, int value2)
{
    if (uniform >= 0)
    {
        m_UniInt[uniform] = std::vector<int>(value1, value2);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGL2_0::BindUniform2f(int uniform, float value1, float value2)
{
    if (uniform >= 0)
    {
        m_UniFloat[uniform] = std::vector<float>(value1, value2);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGL2_0::BindUniformfv(int uniform, const std::vector<float>& value)
{
    if (uniform >= 0)
    {
        m_UniFloatVec[uniform] = value;
        
        return true;
    }
    
    return false;
}

bool CMaterialOGL2_0::BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value)
{
    if (uniform >= 0)
    {
        m_UniMatrixFloat[uniform] = value;
        
        return true;
    }
    
    return false;
}

const IMaterial::TUniInt& CMaterialOGL2_0::Uniformsi() const
{
    return m_UniInt;
}

const IMaterial::TUniFloat& CMaterialOGL2_0::Uniformsf() const
{
    return m_UniFloat;
}

const IMaterial::TUniFloat& CMaterialOGL2_0::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IMaterial::TUniMatrix4Float& CMaterialOGL2_0::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

INL void AddUniformMatrix4f(unsigned int location, const CMatrix4x4f& value)
{
    glUniformMatrix4fv(location, 1, GL_FALSE, value().data());
}

INL void AddUniformf(unsigned int location, const std::vector<float>& value, bool isVector)
{
    if (isVector)
    {
        switch (value.size())
        {
            case 1:
                glUniform1fv(location, 1, value.data());
                break;
                
            case 2:
                glUniform2fv(location, 1, value.data());
                break;
                
            case 3:
                glUniform3fv(location, 1, value.data());
                break;
                
            case 4:
                glUniform4fv(location, 1, value.data());
                break;
        };
    }
    else
    {
        switch (value.size())
        {
            case 1:
                glUniform1f(location, value[0]);
                break;
                
            case 2:
                glUniform2f(location, value[0], value[1]);
                break;
                
            case 3:
                glUniform3f(location, value[0], value[1], value[2]);
                break;
                
            case 4:
                glUniform4f(location, value[0], value[1], value[2], value[3]);
                break;
        };
    }
}

INL void AddUniformi(unsigned int location, const std::vector<int>& value, bool isVector)
{
    if (isVector)
    {
        switch (value.size())
        {
            case 1:
                glUniform1iv(location, 1, value.data());
                break;
                
            case 2:
                glUniform2iv(location, 1, value.data());
                break;
                
            case 3:
                glUniform3iv(location, 1, value.data());
                break;
                
            case 4:
                glUniform4iv(location, 1, value.data());
                break;
        };
    }
    else
    {
        switch (value.size())
        {
            case 1:
                glUniform1i(location, value[0]);
                break;
                
            case 2:
                glUniform2i(location, value[0], value[1]);
                break;
                
            case 3:
                glUniform3i(location, value[0], value[1], value[2]);
                break;
            case 4:
                glUniform4i(location, value[0], value[1], value[2], value[3]);
                break;
        };
    }
}

INL void AddUniformMatrix4f(const IMaterial::TUniMatrix4Float& uniMatrix)
{
    std::for_each(uniMatrix.begin(), uniMatrix.end(), [&](const IMaterial::TUniMatrix4Float::value_type& value)
                  {
                      AddUniformMatrix4f(value.first, value.second);
                  });
}

INL void AddUniformf(const IMaterial::TUniFloat& uniFloat, bool isVector)
{
    std::for_each(uniFloat.begin(), uniFloat.end(), [&](const IMaterial::TUniFloat::value_type& value)
                  {
                      AddUniformf(value.first, value.second, isVector);
                  });
}

INL void AddUniformi(const IMaterial::TUniInt& uniInt, bool isVector)
{
    std::for_each(uniInt.begin(), uniInt.end(), [&](const IMaterial::TUniInt::value_type& value)
                  {
                      AddUniformi(value.first, value.second, isVector);
                  });
}

void CMaterialOGL2_0::UpdateUniforms() const
{
    const IMaterial::TUniFloat& uniFloat = Uniformsf();
    const IMaterial::TUniInt& uniInt = Uniformsi();
    const IMaterial::TUniFloat& uniFloatVec = Uniformsfv();
    const IMaterial::TUniMatrix4Float& uniMatrix4x4f = UniformsMatrix4x4f();
    
    AddUniformi(uniInt, false);
    AddUniformf(uniFloat, false);
    AddUniformf(uniFloatVec, true);
    AddUniformMatrix4f(uniMatrix4x4f);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CMaterialOGL2_0::ApplyState(IMaterial::MaterialState state, IMaterial::MaterialState prevState)
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
    
    {
        //glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glShadeModel(GL_SMOOTH);
        
        //glFrontFace(GL_CCW);
    }
}

void CMaterialOGL2_0::HashMe()
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

#endif // RENDER_OGL2_0 || RENDER_OGLES2_0