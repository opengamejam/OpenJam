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
CLASS_PTR(CRenderComponent);
CLASS_PTR(IMesh);
CLASS_PTR(IMaterial);
CLASS_PTR(ITexture);
CLASS_PTR(IShaderProgram);
    
class CRenderComponent final : public IComponent
{
public:
    CRenderComponent();
    virtual ~CRenderComponent();
    
    IMeshPtr Mesh(const std::string& group = "default");
    void Mesh(IMeshPtr mesh, const std::string& group = "default");
    
    IMaterialPtr Material(const std::string& group = "default");
    void Material(IMaterialPtr material, const std::string& group = "default");
    
    ITexturePtr Texture(const std::string& group = "default");
    void Texture(ITexturePtr texture, const std::string& group = "default");
    
    IShaderProgramPtr Shader(const std::string& group = "default");
    void Shader(IShaderProgramPtr shader, const std::string& group = "default");
    
    const std::set<std::string>& Groups() const;
    
    bool Visible() const;
    void Visible(bool isVisible);
    
    bool Batchable() const;
    void Batchable(bool isBatchable);
    
    unsigned int BatchIndex() const;
    void BatchIndex(unsigned int batchIndex);
    bool IsBatchIndexValid() const;
    
    bool IsValid() const;
    
private:
    std::map<std::string, IMeshPtr> m_Mesh;
    std::map<std::string, IMaterialPtr> m_Material;
    std::map<std::string, ITexturePtr> m_Texture;
    std::map<std::string, IShaderProgramPtr> m_Shader;
    std::set<std::string> m_Groups;
    bool m_IsVisible;
    bool m_IsBatchable;
    unsigned int m_BatchIndex;
};
    
}; // namespace jam

#endif /* defined(CRENDERCOMPONENT_H) */
