//
//  CRenderComponent.h
//  Arkanoid
//
//  Created by yev on 3/6/15.
//
//

#ifndef CRENDERCOMPONENT_H
#define CRENDERCOMPONENT_H

#include "Global.h"
#include "IComponent.h"

namespace jam
{
CLASS_PTR(CRenderComponent)
CLASS_PTR(IMesh)
CLASS_PTR(IMaterial)
CLASS_PTR(ITexture)
CLASS_PTR(IShaderProgram)
    
class CRenderComponent final : public CComponentBase
{
    JAM_OBJECT    
public:
    static const std::string kDefaultGroupName;
    static const std::string kBatchingGroupName;
    
public:
    CRenderComponent();
    virtual ~CRenderComponent();
    
    IMeshPtr Mesh(const std::string& group = kDefaultGroupName);
    void Mesh(IMeshPtr mesh, const std::string& group = kDefaultGroupName);
    
    IMaterialPtr Material(const std::string& group = kDefaultGroupName);
    void Material(IMaterialPtr material, const std::string& group = kDefaultGroupName);
    
    ITexturePtr Texture(const std::string& group = kDefaultGroupName);
    void Texture(ITexturePtr texture, const std::string& group = kDefaultGroupName);
    
    IShaderProgramPtr Shader(const std::string& group = kDefaultGroupName);
    void Shader(IShaderProgramPtr shader, const std::string& group = kDefaultGroupName);
    
    bool Visible(const std::string& group = kDefaultGroupName);
    void Visible(bool isVisible, const std::string& group = kDefaultGroupName);
    
    const std::set<std::string>& Groups() const;
    void RemoveGroup(const std::string& group);
    bool HasGroup(const std::string& group) const;
    
    void AddCameraId(uint32_t cameraId);
    void RemoveCameraId(uint32_t cameraId);
    bool HasCameraId(uint32_t cameraId);
    const std::set<uint32_t>& CameraIds() const;
    
    bool Batchable() const;
    void Batchable(bool isBatchable);
    
    bool IsValid() const;
    
    uint32_t DrawOrder() const;
    
private:
    std::unordered_map<std::string, IMeshPtr> m_Mesh;
    std::unordered_map<std::string, IMaterialPtr> m_Material;
    std::unordered_map<std::string, ITexturePtr> m_Texture;
    std::unordered_map<std::string, IShaderProgramPtr> m_Shader;
    std::set<std::string> m_Groups;
    std::unordered_map<std::string, bool> m_GroupsVisibility;
    bool m_IsBatchable;
    std::set<uint32_t> m_CameraIds;
    uint32_t m_DrawOrder;
};
    
}; // namespace jam

#endif /* defined(CRENDERCOMPONENT_H) */
