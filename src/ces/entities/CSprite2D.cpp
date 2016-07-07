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
#include "CShaderSourceSprite.h"
#include "CSpriteXML.h"
#include "CAnimation2DComponent.h"
#include "CTransformationComponent.h"
#include "CTransformAffector.h"
#include "CResourceCache.hpp"
#include "CTextureCache.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CSprite2DPtr CSprite2D::Create(const std::string& filename, IRendererPtr renderer, uint32_t cameraId)
{
    std::string assetName = "";
    std::size_t found = filename.rfind("/");
    if (found != std::string::npos)
    {
        assetName = filename.substr(0, found);
    }
    
    IMeshPtr mesh = nullptr;
    IMaterialPtr material = nullptr;
    IVertexBufferPtr vertexBuffer = nullptr;
    IIndexBufferPtr indexBuffer = nullptr;
    IShaderPtr vertexShader = nullptr;
    IShaderPtr fragmentShader = nullptr;
    IShaderProgramPtr shaderProgram = nullptr;
    
    // Shaders
    CShaderSourceSprite shaderSource;
    
    vertexShader = renderer->CreateShader();
    vertexShader->Compile(shaderSource.Vertex(), IShader::Vertex);
    assert(vertexShader);
    
    fragmentShader = renderer->CreateShader();
    fragmentShader->Compile(shaderSource.Fragment(), IShader::Fragment);
    assert(fragmentShader);
    
    shaderProgram = renderer->CreateShaderProgram();
    shaderProgram->AttachShader(vertexShader);
    shaderProgram->AttachShader(fragmentShader);
    shaderProgram->Link();
    
    // Material
    material = renderer->CreateMaterial();
    material->PrimitiveType(IMaterial::TrianglesStrip);
    material->DepthEnable(true);
    material->Opacity(false);
    
    // Vertex buffer
    vertexBuffer = renderer->CreatVertexBuffer();
    vertexBuffer->Initialize(sizeof(glm::vec3) + sizeof(CColor4f) + sizeof(glm::vec2));
    assert(vertexBuffer && vertexBuffer->IsValid());
    
    vertexBuffer->Resize(4);
    IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
    position.attributeIndex = shaderProgram->VertexPosition();
    position.dataType = IVertexBuffer::Float;
    position.stride = 3;
    position.offset = 0;
    position.Set<glm::vec3>(0,
    {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f)
    });
    
    IVertexBuffer::SVertexStream& color = vertexBuffer->Lock(IVertexBuffer::Color);
    color.attributeIndex = shaderProgram->VertexColor();
    color.dataType = IVertexBuffer::Float;
    color.stride = 4;
    color.offset = sizeof(glm::vec3);
    color.Set<CColor4f>(0,
    {
        CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
        CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
        CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
        CColor4f(1.0f, 1.0f, 1.0f, 1.0f)
    });
    
    IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
    textureCoord.attributeIndex = shaderProgram->VertexUV();
    textureCoord.dataType = IVertexBuffer::Float;
    textureCoord.stride = 2;
    textureCoord.offset = sizeof(glm::vec3) + sizeof(CColor4f);
    textureCoord.Set<glm::vec2>(0,
    {
        glm::vec2(0.0f, 0.0f),
        glm::vec2(1.0f, 0.0f),
        glm::vec2(0.0f, 1.0f),
        glm::vec2(1.0f, 1.0f)
    });
    
    vertexBuffer->Unlock(true);
    
    // Index buffer
    indexBuffer = renderer->CreateIndexBuffer();
    indexBuffer->Initialize(IIndexBuffer::UShort);
    assert(indexBuffer && indexBuffer->IsValid());
    
    indexBuffer->Resize(6);
    IIndexBuffer::SIndexStream& indices = indexBuffer->Lock();
    indices.Set<unsigned short>(0, {0, 1, 3, 0, 2, 3});
    indexBuffer->Unlock(true);

    // Mesh
    mesh = renderer->CreateMesh();
    assert(mesh && mesh->IsValid());
    mesh->VertexBuffer(vertexBuffer);
    mesh->IndexBuffer(indexBuffer);
    
    // Render component
    CRenderComponentPtr renderComponent(new CRenderComponent());
    renderComponent->Shader(shaderProgram);
    renderComponent->Material(material);
    renderComponent->Mesh(mesh);
    if (cameraId != std::numeric_limits<uint32_t>::max())
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
        
        CTextureCache textureCache(renderer);
        const std::vector<std::string>& textureNames = sprite->Textures();
        std::for_each(textureNames.begin(), textureNames.end(), [&](const std::string& textureName)
        {
            ITexturePtr texture = textureCache.Load(assetName + textureName);
            if (texture)
            {
                renderComponent->Texture(texture, textureName);
                renderComponent->Visible(false, textureName);
            }
        });
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

void CSprite2D::Position(const glm::vec3& position)
{
    CTransformAffector::Position(TransformComponent(), position);
}

const glm::vec3& CSprite2D::Position()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Position();
}

void CSprite2D::Rotation(const glm::vec3& rotation)
{
    CTransformAffector::Rotation(TransformComponent(), rotation);
}

const glm::vec3& CSprite2D::Rotation()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Rotation();
}

void CSprite2D::Scale(const glm::vec3& scale)
{
    CTransformAffector::Scale(TransformComponent(), scale);
}

const glm::vec3& CSprite2D::Scale()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Scale();
}

void CSprite2D::AnchorPoint(const glm::vec3& anchorPoint)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Animation);
    transform.Position(anchorPoint);
    component->AddTransform(CTransformationComponent::Animation, transform);
    component->Dirty();
}

const glm::vec3& CSprite2D::AnchorPoint()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Animation);
    return transform.Position();
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
