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
#include "CResourceCache.hpp"
#include "CTextureCache.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

// *****************************************************************************
// Public Methods
// *****************************************************************************

CSprite2DPtr CSprite2D::Create(const std::string& filename, IRendererPtr renderer)
{
    std::string assetName = "";
    std::size_t found = filename.rfind("/");
    if (found != std::string::npos) {
        assetName = filename.substr(0, found);
    }
    
    JAM_LOG("CSprite2D::CreateObj - name: %s\n", filename.c_str());

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
    vertexBuffer = renderer->CreateVertexBuffer();
    vertexBuffer->Initialize(sizeof(glm::vec3) + sizeof(CColor4f) + sizeof(glm::vec2));
    assert(vertexBuffer && vertexBuffer->IsValid());

    vertexBuffer->Resize(4);
    IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
    position.attributeIndex = shaderProgram->Attribute(shaderSource.VertexAttributeName());
    position.dataType = IVertexBuffer::Float;
    position.stride = 3;
    position.offset = 0;
    position.Set<glm::vec3>(0,
        { glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 0.0f) });

    IVertexBuffer::SVertexStream& color = vertexBuffer->Lock(IVertexBuffer::Color);
    color.attributeIndex = shaderProgram->Attribute(shaderSource.ColorAttributeName());
    color.dataType = IVertexBuffer::Float;
    color.stride = 4;
    color.offset = sizeof(glm::vec3);
    color.Set<CColor4f>(0,
        { CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
            CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
            CColor4f(1.0f, 1.0f, 1.0f, 1.0f),
            CColor4f(1.0f, 1.0f, 1.0f, 1.0f) });

    IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
    textureCoord.attributeIndex = shaderProgram->Attribute(shaderSource.UVAttributeName());
    textureCoord.dataType = IVertexBuffer::Float;
    textureCoord.stride = 2;
    textureCoord.offset = sizeof(glm::vec3) + sizeof(CColor4f);
    textureCoord.Set<glm::vec2>(0,
        { glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(0.0f, 1.0f),
            glm::vec2(1.0f, 1.0f) });

    vertexBuffer->Unlock(true);

    // Index buffer
    indexBuffer = renderer->CreateIndexBuffer();
    indexBuffer->Initialize(IIndexBuffer::UShort);
    assert(indexBuffer && indexBuffer->IsValid());

    indexBuffer->Resize(6);
    IIndexBuffer::SIndexStream& indices = indexBuffer->Lock();
    indices.Set<unsigned short>(0, { 0, 1, 3, 0, 2, 3 });
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

    // Sprite component
    CAnimation2DComponentPtr animationComponent(new CAnimation2DComponent());

    CResourceCache<ISprite> resourceCache;
    ISpritePtr sprite = resourceCache.AcquireResource(filename,
        [](const std::string& filename) -> ISpritePtr {
            ISpritePtr resultSprite(new CSpriteXML(filename));
            if (!resultSprite->Load()) {
                resultSprite = nullptr;
            }

            return resultSprite;
        });

    if (sprite) {
        animationComponent->Sprite(sprite);

        CTextureCache textureCache(renderer);
        const std::vector<std::string>& textureNames = sprite->Textures();
        std::for_each(textureNames.begin(), textureNames.end(), [&](const std::string& textureName) {
            ITexturePtr texture = textureCache.Load(assetName + textureName);
            if (texture) {
                renderComponent->Texture(texture, textureName);
                renderComponent->Visible(false, textureName);
            }
        });
    }

    // Transform component
    CTransformationComponentPtr transformComponent(new CTransformationComponent());

    CSprite2DPtr entity(new CSprite2D());
    entity->Initialize(filename, { renderComponent,
                                   animationComponent,
                                   transformComponent });
    // Store links to components
    entity->m_AnimationComponent = animationComponent;

    return entity;
}

CSprite2D::CSprite2D()
{
}

CSprite2D::~CSprite2D()
{
}

CAnimation2DComponentPtr CSprite2D::AnimationComponent() const
{
    return m_AnimationComponent;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

