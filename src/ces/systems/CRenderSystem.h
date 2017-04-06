//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CRENDERSYSTEM_H
#define CRENDERSYSTEM_H

#include "ISystem.h"
#include "CRenderComponent.h"

namespace jam {
CLASS_PTR(CRenderSystem)
CLASS_PTR(IEntity)
CLASS_PTR(ICamera)
CLASS_PTR(IFrameBuffer)
CLASS_PTR(CRenderComponent)
CLASS_PTR(CBatch)
CLASS_PTR(IMesh)
CLASS_PTR(IMaterial)
CLASS_PTR(ITexture)
CLASS_PTR(IShaderProgram)
CLASS_PTR(IRenderer)

class CRenderSystem : public CSystemBase<CRenderComponent> {
    JAM_OBJECT
public:
    CRenderSystem(IRendererPtr renderer);
    virtual ~CRenderSystem();

    virtual void Update(unsigned long dt) override;
    void Draw(ICameraPtr camera, IEntityPtr root);

private:
    void Draw(IEntityPtr root, const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix);
    void DrawGroup(CRenderComponentPtr renderComponent,
        const std::string& groupName,
        const glm::mat4& projectionMatrix,
        const glm::mat4& viewMatrix,
        const glm::mat4& modelMatrix) const;
    void Draw(IMeshPtr mesh, IMaterialPtr material, ITexturePtr texture, IShaderProgramPtr shader) const;

private:
    IRendererPtr m_Renderer;

    std::set<IFrameBufferPtr> m_ProccededFrameBuffers;
    std::set<uint64_t> m_ProccededBatches;
};

} // namespace jam

#endif /* CRENDERSYSTEM_H */
