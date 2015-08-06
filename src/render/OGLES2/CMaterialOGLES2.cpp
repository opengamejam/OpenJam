//
//  CMaterialOGLES2.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES2)

#include "CMaterialOGLES2.h"
#include "IStencil.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

INL unsigned int CovertStencilFunc(IStencil::StencilFunc func);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMaterialOGLES2::CMaterialOGLES2()
: m_IsDefault(true)
, m_LineWidth(0)
, m_CullFace(true)
, m_Flags(IMaterial::NoneFlag)
, m_PrimitiveType(IMaterial::PT_Triangles)
, m_DepthEnabled(false)
, m_IsDirty(true)
{
    
}

CMaterialOGLES2::~CMaterialOGLES2()
{
}

void CMaterialOGLES2::Bind()
{
    if (LineWidth() > 0.0f)
    {
        glLineWidth(LineWidth());
    }
    
    IStencilPtr stencil = Stencil();
    if (stencil)
    {
        glEnable(GL_STENCIL_TEST);
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        
        unsigned int stencilFunc = CovertStencilFunc(stencil->Func());
        glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
        glStencilFunc(stencilFunc, stencil->Ref(), stencil->Mask());
        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // TODO:
    }
    else
    {
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
        glStencilFunc(GL_EQUAL, 0xFF, 0xFF);
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    
    if (CullFace())
    {
        glEnable(GL_CULL_FACE);
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }
}

void CMaterialOGLES2::Unbind()
{
    IStencilPtr stencil = Stencil();
    if (stencil)
    {
        glDisable(GL_STENCIL_TEST);
    }
}

bool CMaterialOGLES2::IsDefault()
{
    return m_IsDefault;
}

const CColor& CMaterialOGLES2::Color() const
{
    return m_Color;
}

float CMaterialOGLES2::LineWidth() const
{
    return m_LineWidth;
}

IMaterial::PrimitiveTypes CMaterialOGLES2::PrimitiveType() const
{
    return m_PrimitiveType;
}

void CMaterialOGLES2::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_PrimitiveType = primitiveType;
}

void CMaterialOGLES2::Color(const CColor& color)
{
    m_Color = color;
}

void CMaterialOGLES2::LineWidth(float lineWidth)
{
    m_LineWidth = lineWidth;
    m_IsDirty = true;
}

bool CMaterialOGLES2::CullFace() const
{
    return m_CullFace;
}

void CMaterialOGLES2::CullFace(bool isEnabled)
{
    m_CullFace = isEnabled;
}

IStencilPtr CMaterialOGLES2::Stencil() const
{
    return m_Stencil;
}

void CMaterialOGLES2::Stencil(IStencilPtr stencil)
{
    m_Stencil = stencil;
    m_IsDirty = true;
}

bool CMaterialOGLES2::DepthEnable() const
{
    return m_DepthEnabled;
}

void CMaterialOGLES2::DepthEnable(bool value)
{
    m_DepthEnabled = value;
    m_IsDirty = true;
}

int CMaterialOGLES2::UseFromParent() const
{
    return m_Flags;
}

void CMaterialOGLES2::UseFromParent(int flags)
{
    m_Flags = flags;
}

const std::string& CMaterialOGLES2::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

bool CMaterialOGLES2::BindUniform1i(int uniform, int value)
{
    if (uniform >= 0)
    {
        m_UniInt[uniform] = std::vector<int>(value);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGLES2::BindUniform1f(int uniform, float value)
{
    if (uniform >= 0)
    {
        m_UniFloat[uniform] = std::vector<float>(value);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGLES2::BindUniform2i(int uniform, int value1, int value2)
{
    if (uniform >= 0)
    {
        m_UniInt[uniform] = std::vector<int>(value1, value2);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGLES2::BindUniform2f(int uniform, float value1, float value2)
{
    if (uniform >= 0)
    {
        m_UniFloat[uniform] = std::vector<float>(value1, value2);
        
        return true;
    }
    
    return false;
}

bool CMaterialOGLES2::BindUniformfv(int uniform, const std::vector<float>& value)
{
    if (uniform >= 0)
    {
        m_UniFloatVec[uniform] = value;
        
        return true;
    }
    
    return false;
}

bool CMaterialOGLES2::BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value)
{
    if (uniform >= 0)
    {
        m_UniMatrixFloat[uniform] = value;
        
        return true;
    }
    
    return false;
}

const IMaterial::TUniInt& CMaterialOGLES2::Uniformsi() const
{
    return m_UniInt;
}

const IMaterial::TUniFloat& CMaterialOGLES2::Uniformsf() const
{
    return m_UniFloat;
}

const IMaterial::TUniFloat& CMaterialOGLES2::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IMaterial::TUniMatrix4Float& CMaterialOGLES2::UniformsMatrix4x4f() const
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

void CMaterialOGLES2::UpdateUniforms() const
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

void CMaterialOGLES2::HashMe()
{
    std::stringstream ss;    
    ss << PrimitiveType();
    ss << LineWidth();
    ss << (Stencil() != nullptr);
    ss << DepthEnable();
    m_Hash = ss.str();
}

INL unsigned int CovertStencilFunc(IStencil::StencilFunc func)
{
    unsigned int stencilFunc = GL_NEVER;
    switch (func)
    {
        case IStencil::Never:
            stencilFunc = GL_NEVER;
            break;
            
        case IStencil::Less:
            stencilFunc = GL_LESS;
            break;
            
        case IStencil::Equal:
            stencilFunc = GL_EQUAL;
            break;
            
        case IStencil::LEqual:
            stencilFunc = GL_LEQUAL;
            break;
            
        case IStencil::Greater:
            stencilFunc = GL_GREATER;
            break;
            
        case IStencil::NotEqual:
            stencilFunc = GL_NOTEQUAL;
            break;
            
        case IStencil::GEqual:
            stencilFunc = GL_GEQUAL;
            break;
            
        case IStencil::Always:
            stencilFunc = GL_ALWAYS;
            break;
    };
    
    return stencilFunc;
}

#endif // RENDER_OGLES2