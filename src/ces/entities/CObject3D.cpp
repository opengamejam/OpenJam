//
//  CObject3D.cpp
//  Arkanoid
//
//  Created by yev on 5/15/15.
//
//

#include "CObject3D.h"

#include "RenderGlobal.h"
#include "CRenderComponent.h"
#include "CTransform.hpp"
#include "CShaderSourceCommon.h"
#include "CImagePVR.h"
#include "CImageSegaPVR.h"
#include "CModelObj.h"
#include "CTransformationComponent.h"
#include "CResourceCache.hpp"
#include "CTextureCache.h"
#include "CUpdateComponent.h"
#include "CBatchComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

CLASS_PTR(IModel3D)

// *****************************************************************************
// Public Methods
// *****************************************************************************

CObject3DPtr CObject3D::CreateObj(const std::string& filename, IRendererPtr renderer)
{
    assert(renderer);

    std::string assetName = "";
    std::size_t found = filename.rfind("/");
    if (found != std::string::npos) {
        assetName = filename.substr(0, found);
    }

    CTextureCache textureCache(renderer);
    CResourceCache<IModel3D> resourceCache;
    IModel3DPtr model3D = resourceCache.AcquireResource(filename,
        [](const std::string& filename) -> IModel3DPtr {
            IModel3DPtr resultModel(new CModelObj(filename));
            if (!resultModel->Load()) {
                resultModel = nullptr;
            }

            return resultModel;
        });

    if (!model3D) {
        assert("No model data" && false);
        return nullptr;
    }
    
    JAM_LOG("CObject3D::CreateObj - name: %s\n", filename.c_str());

    CRenderComponentPtr renderComponent(new CRenderComponent());

    const std::set<std::string>& groups = model3D->Groups();
    std::for_each(groups.begin(), groups.end(), [&](const std::string& group) {
        IMeshPtr mesh = nullptr;
        IMaterialPtr material = nullptr;
        IVertexBufferPtr vertexBuffer = nullptr;
        IIndexBufferPtr indexBuffer = nullptr;
        IShaderPtr vertexShader = nullptr;
        IShaderPtr fragmentShader = nullptr;
        IShaderProgramPtr shaderProgram = nullptr;

        // Shaders
        CShaderSourceCommon shaderSource;
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
        material->PrimitiveType(IMaterial::Triangles);
        material->CullFace(true); // TODO: temp
        material->DepthEnable(true);

        // Vertex buffer
        uint32_t elementSize = 0;
        bool hasPosition = (model3D->Vertices(group).size() > 0);
        bool hasUVs = (model3D->UVs(group).size() > 0);
        bool hasNormals = (model3D->Normals(group).size() > 0);

        if (hasPosition) {
            elementSize += sizeof(glm::vec3);
        }
        if (hasUVs) {
            elementSize += sizeof(glm::vec2);
        }
        if (hasNormals) {
            elementSize += sizeof(glm::vec3);
        }

        if (elementSize > 0) {
            vertexBuffer = renderer->CreateVertexBuffer();
            vertexBuffer->Initialize(elementSize);
            material->CullFace(false); // TODO: temp
            assert(vertexBuffer && vertexBuffer->IsValid());

            uint32_t offset = 0;
            if (hasPosition) {
                vertexBuffer->Resize(model3D->Vertices(group).size());
                IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
                position.attributeIndex = shaderProgram->VertexPosition();
                position.dataType = IVertexBuffer::Float;
                position.stride = 3;
                position.offset = offset;
                position.Set<glm::vec3>(0, model3D->Vertices(group));

                offset += sizeof(glm::vec3);
            }

            if (hasUVs) {
                IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
                textureCoord.attributeIndex = shaderProgram->VertexUV();
                textureCoord.dataType = IVertexBuffer::Float;
                textureCoord.stride = 2;
                textureCoord.offset = offset;
                textureCoord.Set<glm::vec2>(0, model3D->UVs(group));

                offset += sizeof(glm::vec2);
            }

            if (hasNormals) {
                IVertexBuffer::SVertexStream& normals = vertexBuffer->Lock(IVertexBuffer::Normal);
                normals.attributeIndex = shaderProgram->VertexNormal();
                normals.dataType = IVertexBuffer::Float;
                normals.stride = 3;
                normals.offset = offset;
                normals.Set<glm::vec3>(0, model3D->Normals());

                offset += sizeof(glm::vec3);
            }

            vertexBuffer->Unlock(true);
        }

        // Index buffer
        if (model3D->Indices(group).size() > 0) {
            indexBuffer = renderer->CreateIndexBuffer();
            indexBuffer->Initialize(IIndexBuffer::UShort);
            assert(indexBuffer && indexBuffer->IsValid());

            indexBuffer->Resize(model3D->Indices(group).size());
            IIndexBuffer::SIndexStream& streamIndex = indexBuffer->Lock();

            const std::vector<unsigned int>& indices = model3D->Indices(group);
            std::for_each(indices.begin(), indices.end(), [&](unsigned int face) {
                streamIndex.Set<unsigned short>(0, face);
            });
            indexBuffer->Unlock(true);
        }

        // Mesh
        mesh = renderer->CreateMesh();
        assert(mesh && mesh->IsValid());
        mesh->VertexBuffer(vertexBuffer);
        mesh->IndexBuffer(indexBuffer);

        // Texture
        ITexturePtr texture = textureCache.Load(assetName + model3D->TextureName(group));

        // Render component
        renderComponent->Batchable(true);
        renderComponent->Shader(shaderProgram, group);
        renderComponent->Material(material, group);
        renderComponent->Mesh(mesh, group);
        renderComponent->Texture(texture, group);
    });

    // Transform component
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    CBatchComponentPtr batchComponent(new CBatchComponent());

    CObject3DPtr entity(new CObject3D());
    assert(entity);
    entity->Initialize(filename, { renderComponent,
                                   transformComponent,
                                   batchComponent });

    return entity;
}

CObject3D::CObject3D()
{
}

CObject3D::~CObject3D()
{
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
