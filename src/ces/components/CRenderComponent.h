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
    
    IMeshPtr Mesh() const;
    void Mesh(IMeshPtr mesh);
    
    IMaterialPtr Material() const;
    void Material(IMaterialPtr material);
    
    ITexturePtr Texture() const;
    void Texture(ITexturePtr texture);
    
    IShaderProgramPtr Shader() const;
    void Shader(IShaderProgramPtr shader);
    
    bool Visible() const;
    void Visible(bool isVisible);
    
    bool Batchable() const;
    void Batchable(bool isBatchable);
    
    unsigned int BatchIndex() const;
    void BatchIndex(unsigned int batchIndex);
    bool IsBatchIndexValid() const;
    
    bool IsValid() const;
    
private:
    IMeshPtr m_Mesh;
    IMaterialPtr m_Material;
    ITexturePtr m_Texture;
    IShaderProgramPtr m_Shader;
    bool m_IsVisible;
    bool m_IsBatchable;
    unsigned int m_BatchIndex;
};
    
}; // namespace jam

#endif /* defined(CRENDERCOMPONENT_H) */
