//
//  CRenderComponent.cpp
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#include "CRenderComponent.h"
#include "IMaterial.h"

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
, m_DrawOrder(0)
{

}

CRenderComponent::~CRenderComponent()
{

}

IMeshPtr CRenderComponent::Mesh(const std::string& group)
{
    return m_Mesh[group];
}

void CRenderComponent::Mesh(IMeshPtr mesh, const std::string& group)
{
    m_Mesh[group] = mesh;
    m_Groups.insert(group);
}

IMaterialPtr CRenderComponent::Material(const std::string& group)
{
    return m_Material[group];
}

void CRenderComponent::Material(IMaterialPtr material, const std::string& group)
{
    m_Material[group] = material;
    m_Groups.insert(group);
    
    m_DrawOrder = 0;
    std::for_each(m_Material.begin(), m_Material.end(), [&](const std::pair<std::string, IMaterialPtr>& element)
    {
        IMaterialPtr m = element.second;
        if (!m->Opacity())
        {
            m_DrawOrder |= (1 << 2);
        }
        
        // Objects without depth test draws after that passed depth test
        if (!m->DepthEnable())
        {
            m_DrawOrder |= (1 << 1);
        }
        
        // Stencil test like and depth test
        if (!m->StencilEnable())
        {
            m_DrawOrder |= (1 << 0);
        }
    });
}

ITexturePtr CRenderComponent::Texture(const std::string& group)
{
    return m_Texture[group];
}

void CRenderComponent::Texture(ITexturePtr texture, const std::string& group)
{
    m_Texture[group] = texture;
    m_Groups.insert(group);
}

IShaderProgramPtr CRenderComponent::Shader(const std::string& group)
{
    return m_Shader[group];
}

void CRenderComponent::Shader(IShaderProgramPtr shader, const std::string& group)
{
    m_Shader[group] = shader;
    m_Groups.insert(group);
}

const std::set<std::string>& CRenderComponent::Groups() const
{
    return m_Groups;
}

void CRenderComponent::AddCameraId(uint32_t cameraId)
{
    m_CameraIds.insert(cameraId);
}

void CRenderComponent::RemoveCameraId(uint32_t cameraId)
{
    m_CameraIds.erase(cameraId);
}

bool CRenderComponent::HasCameraId(uint32_t cameraId)
{
    if (m_CameraIds.empty())
    {
        return true;
    }
    
    return (m_CameraIds.find(cameraId) != m_CameraIds.end());
}

const std::set<uint32_t>& CRenderComponent::CameraIds() const
{
    return m_CameraIds;
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
    return m_IsBatchable;
}

void CRenderComponent::Batchable(bool isBatchable)
{
    m_IsBatchable = isBatchable;
}

bool CRenderComponent::IsValid() const
{
    return (m_Groups.size() > 0);
}

uint32_t CRenderComponent::DrawOrder() const
{
    return m_DrawOrder;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
