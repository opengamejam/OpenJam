//
//  CMaterialOGL1_3.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//
#if defined(RENDER_OGL1_3)

#include "CMaterialOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

INL unsigned int ConvertTestFunc(IMaterial::TestFuncs func);
#ifdef GL_STENCIL_TEST
INL unsigned int ConvertOperation(IMaterial::Operations op);
#endif

std::stack<IMaterial::MaterialState> CMaterialOGL1_3::s_States;

CMaterialOGL1_3::CMaterialOGL1_3()
: m_IsDirty(true)
, m_IsBound(false)
{
    
}

CMaterialOGL1_3::~CMaterialOGL1_3()
{
}

void CMaterialOGL1_3::Bind()
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

void CMaterialOGL1_3::Unbind()
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

const CColor& CMaterialOGL1_3::Color() const
{
    return m_State.color;
}

void CMaterialOGL1_3::Color(const CColor& color)
{
    m_State.color = color;
    m_IsDirty = true;
}

float CMaterialOGL1_3::LineWidth() const
{
    return m_State.lineWidth;
}

void CMaterialOGL1_3::LineWidth(float lineWidth)
{
    m_State.lineWidth = lineWidth;
    m_IsDirty = true;
}

IMaterial::PrimitiveTypes CMaterialOGL1_3::PrimitiveType() const
{
    return m_State.primitiveType;
}

void CMaterialOGL1_3::PrimitiveType(IMaterial::PrimitiveTypes primitiveType)
{
    m_State.primitiveType = primitiveType;
    m_IsDirty = true;
}

bool CMaterialOGL1_3::CullFace() const
{
    return m_State.cullFace;
}

void CMaterialOGL1_3::CullFace(bool isEnabled)
{
    m_State.cullFace = isEnabled;
    m_IsDirty = true;
}

bool CMaterialOGL1_3::DepthEnable() const
{
    return m_State.depthTest.isEnabled;
}

void CMaterialOGL1_3::DepthEnable(bool value)
{
    m_State.depthTest.isEnabled = value;
    m_IsDirty = true;
}

bool CMaterialOGL1_3::DepthWriteEnable() const
{
    return m_State.depthTest.isWriteEnabled;
}

void CMaterialOGL1_3::DepthWriteEnable(bool value)
{
    m_State.depthTest.isWriteEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL1_3::DepthRange(double near, double far)
{
    m_State.depthTest.rangeNear = near;
    m_State.depthTest.rangeFar = far;
    m_IsDirty = true;
}

IMaterial::TestFuncs CMaterialOGL1_3::DepthFunc()
{
    return m_State.depthTest.func;
}

void CMaterialOGL1_3::DepthFunc(TestFuncs func)
{
    m_State.depthTest.func = func;
    m_IsDirty = true;
}

bool CMaterialOGL1_3::StencilEnable() const
{
    return m_State.stencilTest.isEnabled;
}

void CMaterialOGL1_3::StencilEnable(bool value)
{
    m_State.stencilTest.isEnabled = value;
    m_IsDirty = true;
}

void CMaterialOGL1_3::StencilFunc(TestFuncs func, unsigned int ref, unsigned int mask)
{
    m_State.stencilTest.func = func;
    m_State.stencilTest.ref = ref;
    m_State.stencilTest.mask = mask;
    m_IsDirty = true;
}

void CMaterialOGL1_3::StencilOperations(Operations failOp, Operations zFailOp, Operations zPassOp)
{
    m_State.stencilTest.failOp = failOp;
    m_State.stencilTest.zFailOp = zFailOp;
    m_State.stencilTest.zPassOp = zPassOp;
    m_IsDirty = true;
}

bool CMaterialOGL1_3::Opacity() const
{
    return m_State.opacity;
}

void CMaterialOGL1_3::Opacity(bool value)
{
    m_State.opacity = value;
    m_IsDirty = true;
}

const std::string& CMaterialOGL1_3::Hash()
{
    if (m_IsDirty)
    {
        HashMe();
        m_IsDirty = false;
    }
    
    return m_Hash;
}

bool CMaterialOGL1_3::BindUniform1i(int uniform, int value)
{
    m_UniInt[uniform] = std::vector<int>(value);
    
    return true;
}

bool CMaterialOGL1_3::BindUniform1f(int uniform, float value)
{
    m_UniFloat[uniform] = std::vector<float>(value);
    
    return true;
}

bool CMaterialOGL1_3::BindUniform2i(int uniform, int value1, int value2)
{
    m_UniInt[uniform] = std::vector<int>(value1, value2);
    
    return true;
}

bool CMaterialOGL1_3::BindUniform2f(int uniform, float value1, float value2)
{
    m_UniFloat[uniform] = std::vector<float>(value1, value2);
    
    return true;
}

bool CMaterialOGL1_3::BindUniformfv(int uniform, const std::vector<float>& value)
{
    m_UniFloatVec[uniform] = value;
    
    return true;
}

bool CMaterialOGL1_3::BindUniformMatrix4x4f(int uniform, const CMatrix4x4f& value)
{
    m_UniMatrixFloat[uniform] = value;
    
    return true;
}

const IMaterial::TUniInt& CMaterialOGL1_3::Uniformsi() const
{
    return m_UniInt;
}

const IMaterial::TUniFloat& CMaterialOGL1_3::Uniformsf() const
{
    return m_UniFloat;
}

const IMaterial::TUniFloat& CMaterialOGL1_3::Uniformsfv() const
{
    return m_UniFloatVec;
}

const IMaterial::TUniMatrix4Float& CMaterialOGL1_3::UniformsMatrix4x4f() const
{
    return m_UniMatrixFloat;
}

void CMaterialOGL1_3::UpdateUniforms() const
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

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
}

void CMaterialOGL1_3::HashMe()
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

#ifdef GL_STENCIL_TEST
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
#endif

#endif // RENDER_OGL1_3
