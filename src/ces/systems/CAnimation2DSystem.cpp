//
//  CRenderSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#include "CAnimation2DSystem.h"
#include "IEntity.h"
#include "CTransformationComponent.h"
#include "CRenderComponent.h"
#include "IMaterial.h"
#include "IShaderProgram.h"
#include "IVertexBuffer.h"
#include "IMesh.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CAnimation2DSystem::CAnimation2DSystem()
{
}

CAnimation2DSystem::~CAnimation2DSystem()
{
}

void CAnimation2DSystem::Update(unsigned long dt)
{
    ForEachComponents([&](CAnimation2DComponentPtr anim2DComponent) {
        bool frameChanged = anim2DComponent->Time(anim2DComponent->Time() + dt);
        if (!frameChanged && !anim2DComponent->IsStatic()) { // TODO: don't update static animation
            return;
        }
        // Update texture coords
        IEntityPtr entity = anim2DComponent->Entity();
        if (!entity) {  // TODO: check if need this condition
            return;
        }
        
        entity->Get<CRenderComponent>([&](CRenderComponentPtr renderComponent) {
            bool isDirty = false;
            // Switch texture atlas
            ITexturePtr currentTexture = renderComponent->Texture(anim2DComponent->TextureName());
            if (currentTexture != renderComponent->Texture()) {
                renderComponent->Texture(currentTexture);
                isDirty = true;
            }

            // Modify texture coords
            IVertexBufferPtr vertexBuffer = renderComponent->Mesh()->VertexBuffer();
            if (vertexBuffer->HasStream(IVertexBuffer::TextureCoords)) {
                IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
                textureCoord.Set<glm::vec2>(0, anim2DComponent->TextureFrame());
                vertexBuffer->Unlock(true);
                isDirty = true;
            }

            if (isDirty) {
                renderComponent->Dirty();
            }
        });

        // Change animation transformation (pivot point, tweens,..)
        entity->Get<CTransformationComponent, true>([&](CTransformationComponentPtr transformComponent) {
            CTransform3Df frameTransform = anim2DComponent->FrameTransform();
            transformComponent->AddTransform(CTransformationComponent::Animation,
                frameTransform,
                false);
        });
    });
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
