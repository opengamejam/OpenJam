//
//  CIndexBufferOGL1_3.cpp
//  Arkanoid
//
//  Created by Yevgeniy Logachev on 4/14/15.
//
//
#if defined(RENDER_OGL1_3)

#include "CIndexBufferOGL1_3.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CIndexBufferOGL1_3::CIndexBufferOGL1_3()
: m_Id(1)
, m_ElementSize(1)
{
    
}

CIndexBufferOGL1_3::~CIndexBufferOGL1_3()
{
    
}

void CIndexBufferOGL1_3::Initialize(size_t elementSize)
{
    ElementSize(elementSize);
}

void CIndexBufferOGL1_3::Destroy()
{
    
}

bool CIndexBufferOGL1_3::IsValid() const
{
    return (m_Id != 0);
}

size_t CIndexBufferOGL1_3::SizeRaw() const
{
    return m_Buffer.size();
}

void CIndexBufferOGL1_3::ResizeRaw(size_t newSize)
{
    m_Buffer.resize(newSize);
}

size_t CIndexBufferOGL1_3::ElementSize() const
{
    return m_ElementSize;
}

void* CIndexBufferOGL1_3::LockRaw()
{
    return m_Buffer.data();
}

void CIndexBufferOGL1_3::Unlock()
{    
    
}

void CIndexBufferOGL1_3::Bind()
{
    
}

void CIndexBufferOGL1_3::Unbind()
{
    
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

void CIndexBufferOGL1_3::ElementSize(size_t elementSize)
{
    m_ElementSize = std::max<size_t>(elementSize, 1);
}

// *****************************************************************************
// Private Methods
// *****************************************************************************

#endif // RENDER_OGL1_3