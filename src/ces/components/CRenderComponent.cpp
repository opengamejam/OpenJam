//
//  CRenderComponent.cpp
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#include "CRenderComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderComponent::CRenderComponent()
: IComponent(ComponentId<CRenderComponent>())
, m_IsVisible(true)
, m_IsBatchable(false)
, m_BatchIndex(std::numeric_limits<typeof(m_BatchIndex)>::max())
{

}

CRenderComponent::~CRenderComponent()
{

}

IMeshPtr CRenderComponent::Mesh() const
{
    return m_Mesh;
}

void CRenderComponent::Mesh(IMeshPtr mesh)
{
    m_Mesh = mesh;
}

IMaterialPtr CRenderComponent::Material() const
{
    return m_Material;
}

void CRenderComponent::Material(IMaterialPtr material)
{
    m_Material = material;
}

ITexturePtr CRenderComponent::Texture() const
{
    return m_Texture;
}

void CRenderComponent::Texture(ITexturePtr texture)
{
    m_Texture = texture;
}

IShaderProgramPtr CRenderComponent::Shader() const
{
    return m_Shader;
}

void CRenderComponent::Shader(IShaderProgramPtr shader)
{
    m_Shader = shader;
}

bool CRenderComponent::Visible() const
{
    return m_IsVisible;
}

void CRenderComponent::Visible(bool isVisible)
{
    if (m_IsVisible != isVisible)
    {
        m_IsVisible = isVisible;
    }
}

bool CRenderComponent::Batchable() const
{
    return (m_BatchIndex != std::numeric_limits<typeof(m_BatchIndex)>::max());
}

void CRenderComponent::Batchable(bool isBatchable)
{
    if (m_IsBatchable != isBatchable)
    {
        m_IsBatchable = isBatchable;
    }
}

unsigned int CRenderComponent::BatchIndex() const
{
    return m_BatchIndex;
}

void CRenderComponent::BatchIndex(unsigned int batchIndex)
{
    if (m_BatchIndex != batchIndex)
    {
        m_BatchIndex = batchIndex;
    }
}

bool CRenderComponent::IsBatchIndexValid() const
{
    return (m_BatchIndex != std::numeric_limits<typeof(m_BatchIndex)>::max());
}

bool CRenderComponent::IsValid() const
{
    return (m_Mesh && m_Material && m_Shader);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
