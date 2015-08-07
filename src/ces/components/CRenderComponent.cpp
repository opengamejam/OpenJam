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

void CRenderComponent::AddCameraId(unsigned int cameraId)
{
    m_CameraIds.insert(cameraId);
}

void CRenderComponent::RemoveCameraId(unsigned int cameraId)
{
    m_CameraIds.erase(cameraId);
}

bool CRenderComponent::HasCameraId(unsigned int cameraId)
{
    if (m_CameraIds.empty())
    {
        return true;
    }
    
    return (m_CameraIds.find(cameraId) != m_CameraIds.end());
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
    return (m_Groups.size() > 0);
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
