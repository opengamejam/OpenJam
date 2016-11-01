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
const std::string CRenderComponent::kDefaultGroupName = "default";
const std::string CRenderComponent::kBatchingGroupName = "jam_batch_group";

// *****************************************************************************
// Public Methods
// *****************************************************************************

CRenderComponent::CRenderComponent()
    : m_IsBatchable(false)
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
    m_GroupsVisibility[group] = true;
}

IMaterialPtr CRenderComponent::Material(const std::string& group)
{
    return m_Material[group];
}

void CRenderComponent::Material(IMaterialPtr material, const std::string& group)
{
    m_Material[group] = material;
    m_Groups.insert(group);
    m_GroupsVisibility[group] = true;

    m_DrawOrder = 0;
    std::for_each(m_Material.begin(), m_Material.end(), [&](const std::pair<std::string, IMaterialPtr>& element) {
        IMaterialPtr m = element.second;
        if (!m->Opacity()) {
            m_DrawOrder |= (1 << 2);
        }

        if (!m->DepthEnable()) {
            m_DrawOrder |= (1 << 1);
        }

        if (!m->StencilEnable()) {
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
    m_GroupsVisibility[group] = true;
}

IShaderProgramPtr CRenderComponent::Shader(const std::string& group)
{
    return m_Shader[group];
}

void CRenderComponent::Shader(IShaderProgramPtr shader, const std::string& group)
{
    m_Shader[group] = shader;
    m_Groups.insert(group);
    m_GroupsVisibility[group] = true;
}

bool CRenderComponent::Visible(const std::string& group)
{
    return m_GroupsVisibility[group];
}

void CRenderComponent::Visible(bool isVisible, const std::string& group)
{
    m_GroupsVisibility[group] = isVisible;
}

const std::set<std::string>& CRenderComponent::Groups() const
{
    return m_Groups;
}

void CRenderComponent::RemoveGroup(const std::string& group)
{
    m_Groups.erase(group);
    m_Mesh.erase(group);
    m_Material.erase(group);
    m_Texture.erase(group);
    m_Shader.erase(group);
}

bool CRenderComponent::HasGroup(const std::string& group) const
{
    return m_Groups.find(group) != m_Groups.end();
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
    if (m_CameraIds.empty()) {
        return true;
    }

    return (m_CameraIds.find(cameraId) != m_CameraIds.end());
}

const std::set<uint32_t>& CRenderComponent::CameraIds() const
{
    return m_CameraIds;
}

template <class T>
bool IsAllValuesIsEqual(const std::unordered_map<std::string, T>& map)
{
    if (map.empty()) {
        return true;
    }

    bool result = true;
    /*T value = map.first().second;
    std::all_of(map.begin(), map.end(), [&](const std::pair<std::string, T>& element)
    {
        if (value != element.second)
        {
            result = false;
        }
        
        return result;
    });*/

    return result;
}

bool CRenderComponent::Batchable() const
{
    if (m_IsBatchable) {
        return (IsAllValuesIsEqual(m_Material) && IsAllValuesIsEqual(m_Shader) && IsAllValuesIsEqual(m_Texture));
    }

    return false;
}

void CRenderComponent::Batchable(bool isBatchable)
{
    m_IsBatchable = isBatchable;
    if (!Batchable()) {
        printf("Render Component with different material in groups cannot be batched"); // TODO: Log system
    }
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
