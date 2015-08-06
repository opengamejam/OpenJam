//
//  CMaterialOGLES1.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGLES1)

#include "CMaterialOGLES1.h"
#include "IStencil.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

INL unsigned int CovertStencilFunc(IStencil::StencilFunc func);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CMaterialOGLES1::CMaterialOGLES1()
: m_IsDefault(true)
, m_LineWidth(0)
, m_CullFace(true)
, m_Flags(IMaterial::NoneFlag)
, m_PrimitiveType(IMaterial::PT_Triangles)
, m_DepthEnabled(false)
, m_IsDirty(true)
{
    
}

CMaterialOGLES1::~CMaterialOGLES1()
{
}

void CMaterialOGLES1::Bind()
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
        glDepthFunc(GL_LESS); 
        
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        glEnable(GL_TEXTURE_2D);
        glShadeModel(GL_SMOOTH);
        
        //glFrontFace(GL_CCW);
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

void CMaterialOGLES1::Unbind()
{
    IStencilPtr stencil = Stencil();
    if (stencil)
    {
        glDisable(GL_STENCIL_TEST);
    }
}

bool CMaterialOGLES1::IsDefault()
{
    return m_IsDefault;
}

const CColor& CMaterialOGLES1::Color() const
{
    return m_Color;
}

float CMaterialOGLES1::LineWidth() const
{
    return m_LineWidth;
}

IMaterial::PrimitiveTypes CMaterialOGLES1::PrimitiveType() const
{
    return m_PrimitiveType;
}

void CMaterialOGLES1::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_PrimitiveType = primitiveType;
}

void CMaterialOGLES1::Color(const CColor& color)
{
    m_Color = color;
}

void CMaterialOGLES1::LineWidth(float lineWidth)
{
    m_LineWidth = lineWidth;
    m_IsDirty = true;
}

bool CMaterialOGLES1::CullFace() const
{
    return m_CullFace;
}

void CMaterialOGLES1::CullFace(bool isEnabled)
{
    m_CullFace = isEnabled;
}

IStencilPtr CMaterialOGLES1::Stencil() const
{
    return m_Stencil;
}

void CMaterialOGLES1::Stencil(IStencilPtr stencil)
{
    m_Stencil = stencil;
    m_IsDirty = true;
}

bool CMaterialOGLES1::DepthEnable() const
{
    return m_DepthEnabled;
}

void CMaterialOGLES1::DepthEnable(bool value)
{
    m_DepthEnabled = value;
    m_IsDirty = true;
}

int CMaterialOGLES1::UseFromParent() const
{
    return m_Flags;
}

void CMaterialOGLES1::UseFromParent(int flags)
{
    m_Flags = flags;
}

const std::string& CMaterialOGLES1::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

bool CMaterialOGLES1::BindUniform1i(int uniform, int value)
{
    m_UniInt[uniform] = std::vector<int>(value);
    
    return true;
}

bool CMaterialOGLES1::BindUniform1f(int uniform, float value)
{
    m_UniFloat[uniform] = std::vector<float>(value);
    
    return true;
}

bool CMaterialOGLES1::BindUniform2i(int uniform, int value1, int value2)
{
    m_UniInt[uniform] = std::vector<int>(value1, value2);
    
    return true;
}

bool CMaterialOGLES1::BindUniform2f(int uniform, float value1, float value2)
{
    m_UniFloat[uniform] = std::vector<float>(value1, value2);
    
    return true;
}

bool CMaterialOGLES1::BindUniformfv(int uniform, const std::vector<float>& value)
{
    m_UniFloatVec[uniform] = value;
    
    return true;
}

bool CMaterialOGLES1::BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value)
{
    m_UniMatrixFloat[uniform] = value;
    
    return true;
}

const IMaterial::TUniInt& CMaterialOGLES1::Uniformsi() const
{
    return m_UniInt;
}

const IMaterial::TUniFloat& CMaterialOGLES1::Uniformsf() const
{
    return m_UniFloat;
}

const IMaterial::TUniFloat& CMaterialOGLES1::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IMaterial::TUniMatrix4Float& CMaterialOGLES1::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

void CMaterialOGLES1::UpdateUniforms() const
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CMaterialOGLES1::HashMe()
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

#endif // RENDER_OGLES1