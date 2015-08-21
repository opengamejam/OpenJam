//
//  CSprite2D.cpp
//  Arkanoid
//
//  Created by yev on 5/15/15.
//
//

#include "CSprite2D.h"

#include "RenderGlobal.h"
#include "CRenderComponent.h"
#include "CTransform.h"
#include "CVertex.h"
#include "CColor.h"
#include "CShaderSourceSprite.h"
#include "CSpriteXML.h"
#include "CAnimation2DComponent.h"
#include "CTransformationComponent.h"
#include "CTransformAffector.h"
#include "CResourceCache.hpp"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CSprite2DPtr CSprite2D::Create(const std::string& filename, unsigned int cameraId)
{
    IMeshPtr mesh = nullptr;
    IMaterialPtr material = nullptr;
    IVertexBufferPtr vertexBuffer = nullptr;
    IIndexBufferPtr indexBuffer = nullptr;
    IShaderPtr vertexShader = nullptr;
    IShaderPtr fragmentShader = nullptr;
    IShaderProgramPtr shaderProgram = nullptr;
    
    // Shaders
    CShaderSourceSprite shaderSource;
    
    vertexShader = GRenderer->CreateShader();
    vertexShader->Compile(shaderSource.Vertex(), IShader::Vertex);
    assert(vertexShader);
    
    fragmentShader = GRenderer->CreateShader();
    fragmentShader->Compile(shaderSource.Fragment(), IShader::Fragment);
    assert(fragmentShader);
    
    shaderProgram = GRenderer->CreateShaderProgram();
    shaderProgram->AttachShader(vertexShader);
    shaderProgram->AttachShader(fragmentShader);
    shaderProgram->Link();
    
    // Material
    material = GRenderer->CreateMaterial();
    material->PrimitiveType(IMaterial::TrianglesStrip);
    material->DepthEnable(true);
    
    // Vertex buffer
    vertexBuffer = GRenderer->CreatVertexBuffer();
    vertexBuffer->Initialize(sizeof(CVector3Df) + sizeof(CColor) + sizeof(CVector2Df));
    assert(vertexBuffer && vertexBuffer->IsValid());
    
    vertexBuffer->Resize(4);
    IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
    position.attributeIndex = shaderProgram->VertexPosition();
    position.dataType = IVertexBuffer::Float;
    position.stride = 3;
    position.offset = 0;
    position.Set<CVector3Df>(0,
    {
        CVector3Df(0.0f, 0.0f, 0.0f),
        CVector3Df(1.0f, 0.0f, 0.0f),
        CVector3Df(0.0f, 1.0f, 0.0f),
        CVector3Df(1.0f, 1.0f, 0.0f)
    });
    
    IVertexBuffer::SVertexStream& color = vertexBuffer->Lock(IVertexBuffer::Color);
    color.attributeIndex = shaderProgram->VertexColor();
    color.dataType = IVertexBuffer::Float;
    color.stride = 4;
    color.offset = sizeof(CVector3Df);
    color.Set<CColor>(0,
    {
        CColor(1.0f, 1.0f, 1.0f, 1.0f),
        CColor(1.0f, 1.0f, 1.0f, 1.0f),
        CColor(1.0f, 1.0f, 1.0f, 1.0f),
        CColor(1.0f, 1.0f, 1.0f, 1.0f)
    });
    
    IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoors);
    textureCoord.attributeIndex = shaderProgram->TextureCoord();
    textureCoord.dataType = IVertexBuffer::Float;
    textureCoord.stride = 2;
    textureCoord.offset = sizeof(CVector3Df) + sizeof(CColor);
    textureCoord.Set<CVector2Df>(0,
    {
        CVector2Df(0.0f, 0.0f),
        CVector2Df(1.0f, 0.0f),
        CVector2Df(0.0f, 1.0f),
        CVector2Df(1.0f, 1.0f)
    });
    
    vertexBuffer->Unlock();
    
    // Index buffer
    indexBuffer = GRenderer->CreateIndexBuffer();
    indexBuffer->Initialize(sizeof(unsigned short));
    assert(indexBuffer && indexBuffer->IsValid());
    
    indexBuffer->Resize(6);
    unsigned short* lockedIndex = indexBuffer->Lock<unsigned short>();
    if (lockedIndex)
    {
        lockedIndex[0] = 0;
        lockedIndex[1] = 1;
        lockedIndex[2] = 3;
        lockedIndex[3] = 0;
        lockedIndex[4] = 2;
        lockedIndex[5] = 3;
    }
    indexBuffer->Unlock();

    // Mesh
    mesh = GRenderer->CreateMesh();
    assert(mesh && mesh->IsValid());
    mesh->VertexBuffer(vertexBuffer);
    mesh->IndexBuffer(indexBuffer);
    
    // Render component
    CRenderComponentPtr renderComponent(new CRenderComponent());
    renderComponent->Shader(shaderProgram);
    renderComponent->Material(material);
    renderComponent->Mesh(mesh);
    if (cameraId != -1u)
    {
        renderComponent->AddCameraId(cameraId);
    }
    
    // Sprite component
    CAnimation2DComponentPtr animationComponent(new CAnimation2DComponent());
    
    CResourceCache<ISprite> resourceCache;
    ISpritePtr sprite = resourceCache.AcquireResource(filename, false,
                                                      [](const std::string& filename) -> ISpritePtr
    {
        ISpritePtr resultSprite(new CSpriteXML(filename));
        if (!resultSprite->Load())
        {
            resultSprite = nullptr;
        }
        
        return resultSprite;
    });
    
    if (sprite)
    {
        animationComponent->Sprite(sprite);
    }
    
    // Transform component
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
     
    CSprite2DPtr entity(new CSprite2D());
    entity->Initialize(filename, {
                                  renderComponent,
                                  animationComponent,
                                  transformComponent
                                 });
    // Store links to components
    entity->m_RenderComponent = renderComponent;
    entity->m_AnimationComponent = animationComponent;
    entity->m_TransformationComponent = transformComponent;
    
    return entity;
}

CSprite2D::CSprite2D()
{
    
}

CSprite2D::~CSprite2D()
{

}

CRenderComponentPtr CSprite2D::RenderComponent() const
{
    return m_RenderComponent;
}

CAnimation2DComponentPtr CSprite2D::AnimationComponent() const
{
    return m_AnimationComponent;
}

CTransformationComponentPtr CSprite2D::TransformComponent() const
{
    return m_TransformationComponent;
}

void CSprite2D::Position(const CVector3Df& position)
{
    CTransformAffector::Position(TransformComponent(), position);
}

const CVector3Df& CSprite2D::Position()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Position();
}

void CSprite2D::Rotation(const CVector3Df& rotation)
{
    CTransformAffector::Rotation(TransformComponent(), rotation);
}

const CVector3Df& CSprite2D::Rotation()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Rotation();
}

void CSprite2D::Scale(const CVector3Df& scale)
{
    CTransformAffector::Scale(TransformComponent(), scale);
}

const CVector3Df& CSprite2D::Scale()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Scale();
}

void CSprite2D::AnchorPoint(const CVector3Df& anchorPoint)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Offset(anchorPoint);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CSprite2D::AnchorPoint()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Offset();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
