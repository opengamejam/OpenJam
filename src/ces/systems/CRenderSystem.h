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

namespace jam
{
CLASS_PTR(CRenderSystem);
CLASS_PTR(IEntity);
CLASS_PTR(ICamera);
CLASS_PTR(IRenderTarget);
CLASS_PTR(CRenderComponent);
CLASS_PTR(CBatch);
CLASS_PTR(IMesh);
CLASS_PTR(IMaterial);
CLASS_PTR(ITexture);
CLASS_PTR(IShaderProgram);

class CRenderSystem : public ISystem
{
public:
    CRenderSystem();
    virtual ~CRenderSystem();
    
    virtual void Update(unsigned long dt) override;
    void Draw(ICameraPtr camera);
    
protected:
    virtual void OnAddedEntity(IEntityPtr entity) override;
    virtual void OnChangedEntity(IEntityPtr entity) override;
    virtual void OnRemovedEntity(IEntityPtr entity) override;
    
private:
    void DrawGroup(CRenderComponentPtr renderComponent,
                   const std::string& groupName,
                   const glm::mat4& projectionMatrix,
                   const glm::mat4& viewMatrix,
                   const glm::mat4& modelMatrix) const;
    void Draw(IMeshPtr mesh, IMaterialPtr material, ITexturePtr texture, IShaderProgramPtr shader) const;
    
private:
    
    std::set<IRenderTargetPtr> m_ProccededRenderTargets;
    std::set<uint64_t> m_ProccededBatches;
    
    std::list<CRenderComponentPtr> m_SortedComponents;
    std::map<CRenderComponentPtr, uint64_t> m_OrderKeys;
};
    
} // namespace jam


#endif /* CRENDERSYSTEM_H */
