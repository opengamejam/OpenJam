//
//  CSprite2DEntity.cpp
//  Arkanoid
//
//  Created by yev on 5/15/15.
//
//

#include "CSprite2DEntity.h"

#include "RenderGlobal.h"
#include "CRenderComponent.h"
#include "CTransform.h"
#include "CVertex.h"
#include "CColor.h"
#include "CShaderSourceSprite.h"
#include "CImagePVR.h"
#include "CSpriteXML.h"
#include "CAnimation2DComponent.h"
#include "CTransformationComponent.h"

using namespace jam;

CSprite2DEntityPtr CSprite2DEntity::Create(const std::string& filename)
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
    
    // Vertex buffer
    vertexBuffer = GRenderer->CreatVertexBuffer();
    vertexBuffer->Initialize(sizeof(CVector3Df) + sizeof(CColor) + sizeof(CVector2Df));
    assert(vertexBuffer && vertexBuffer->IsValid());
    
    vertexBuffer->Resize(4);
    IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
    position.binding = shaderProgram->VertexPosition();
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
    color.binding = shaderProgram->VertexColor();
    color.dataType = IVertexBuffer::Float;
    color.stride = 4;
    color.offset = sizeof(CVector3Df);
    color.Set<CColor>(0,
    {
        CColor(1.0f, 1.0f, 1.0f, 1.0),
        CColor(1.0f, 1.0f, 1.0f, 1.0),
        CColor(1.0f, 1.0f, 1.0f, 1.0),
        CColor(1.0f, 1.0f, 1.0f, 1.0)
    });
    
    IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoors);
    textureCoord.binding = shaderProgram->TextureCoord();
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
    indexBuffer->Initialize(sizeof(short));
    assert(indexBuffer && indexBuffer->IsValid());
    
    indexBuffer->Resize(6);
    short* lockedIndex = indexBuffer->Lock<short>();
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
    
    // Sprite component
    CAnimation2DComponentPtr animationComponent(new CAnimation2DComponent());
    ISpritePtr sprite(new CSpriteXML(filename));
    if (sprite->Load())
    {
        animationComponent->Sprite(sprite);
    }
    
    // Transform component
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
     
    CSprite2DEntityPtr entity = IEntity::Create<CSprite2DEntity>(filename, {
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

CSprite2DEntity::CSprite2DEntity()
{
    
}

CSprite2DEntity::~CSprite2DEntity()
{

}

CRenderComponentPtr CSprite2DEntity::RenderComponent() const
{
    return m_RenderComponent;
}

CAnimation2DComponentPtr CSprite2DEntity::AnimationComponent() const
{
    return m_AnimationComponent;
}

CTransformationComponentPtr CSprite2DEntity::TransformComponent() const
{
    return m_TransformationComponent;
}

void CSprite2DEntity::Position(const CVector3Df& position)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Position(position);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CSprite2DEntity::Position()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Position();
}

void CSprite2DEntity::Rotation(const CVector3Df& rotation)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Rotation(rotation);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CSprite2DEntity::Rotation()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Rotation();
}

void CSprite2DEntity::Scale(const CVector3Df& scale)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Scale(scale);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CSprite2DEntity::Scale()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Scale();
}

void CSprite2DEntity::AnchorPoint(const CVector3Df& anchorPoint)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Offset(anchorPoint);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CSprite2DEntity::AnchorPoint()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Offset();
}
