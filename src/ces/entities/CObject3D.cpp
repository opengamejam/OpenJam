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
#include "CTransformAffector.h"
#include "CUpdateComponent.h"
#include "CBatchComponent.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************

CLASS_PTR(IModel3D);

// *****************************************************************************
// Public Methods
// *****************************************************************************

CObject3DPtr CObject3D::CreateObj(const std::string& filename, uint32_t cameraId)
{
    CResourceCache<IModel3D> resourceCache;
    IModel3DPtr model3D = resourceCache.AcquireResource(filename, false,
                                                        [](const std::string& filename) -> IModel3DPtr
    {
        IModel3DPtr resultModel(new CModelObj(filename));
        if (!resultModel->Load())
        {
            resultModel = nullptr;
        }

        return resultModel;
    });
    
    if (!model3D)
    {
        return nullptr;
    }
    
    CRenderComponentPtr renderComponent(new CRenderComponent());
    
    const std::set<std::string>& groups = model3D->Groups();
    std::for_each(groups.begin(), groups.end(), [&](const std::string& group)
    {
        IMeshPtr mesh = nullptr;
        IMaterialPtr material = nullptr;
        IVertexBufferPtr vertexBuffer = nullptr;
        IIndexBufferPtr indexBuffer = nullptr;
        IShaderPtr vertexShader = nullptr;
        IShaderPtr fragmentShader = nullptr;
        IShaderProgramPtr shaderProgram = nullptr;
    
        // Shaders
        CShaderSourceCommon shaderSource;
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
        material->PrimitiveType(IMaterial::Triangles);
        material->CullFace(true); // TODO: temp
        material->DepthEnable(true);
        
        // Vertex buffer
        uint32_t elementSize = 0;
        if (model3D->Vertices(group).size() > 0)
        {
            elementSize += sizeof(glm::vec3);
        }
        if (model3D->UVs(group).size() > 0)
        {
            elementSize += sizeof(glm::vec2);
        }
        if (model3D->Normals(group).size() > 0)
        {
            elementSize += sizeof(glm::vec3);
        }
        
        if (elementSize > 0)
        {
            vertexBuffer = GRenderer->CreatVertexBuffer();
            vertexBuffer->Initialize(elementSize);
            material->CullFace(false); // TODO: temp
            assert(vertexBuffer && vertexBuffer->IsValid());
            
            uint32_t offset = 0;
            if (model3D->Vertices(group).size() > 0)
            {
                vertexBuffer->Resize(model3D->Vertices(group).size());
                IVertexBuffer::SVertexStream& position = vertexBuffer->Lock(IVertexBuffer::Position);
                position.attributeIndex = shaderProgram->VertexPosition();
                position.dataType = IVertexBuffer::Float;
                position.stride = 3;
                position.offset = offset;
                position.Set<glm::vec3>(0, model3D->Vertices(group));
                
                offset += sizeof(glm::vec3);
            }
            
            if (model3D->UVs(group).size() > 0)
            {
                IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
                textureCoord.attributeIndex = shaderProgram->VertexUV();
                textureCoord.dataType = IVertexBuffer::Float;
                textureCoord.stride = 2;
                textureCoord.offset = offset;
                textureCoord.Set<glm::vec2>(0, model3D->UVs(group));
                
                offset += sizeof(glm::vec2);
            }
            
            if (model3D->Normals(group).size() > 0)
            {
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
        if (model3D->Indices(group).size() > 0)
        {
            indexBuffer = GRenderer->CreateIndexBuffer();
            indexBuffer->Initialize(IIndexBuffer::UShort);
            assert(indexBuffer && indexBuffer->IsValid());
            
            indexBuffer->Resize(model3D->Indices(group).size());
            IIndexBuffer::SIndexStream& streamIndex = indexBuffer->Lock();
            
            const std::vector<unsigned int>& indices = model3D->Indices(group);
            std::for_each(indices.begin(), indices.end(), [&](unsigned int face)
            {
                streamIndex.Set<unsigned short>(0, face);
            });
            indexBuffer->Unlock(true);
        }

        // Mesh
        mesh = GRenderer->CreateMesh();
        assert(mesh && mesh->IsValid());
        mesh->VertexBuffer(vertexBuffer);
        mesh->IndexBuffer(indexBuffer);
        
        // Texture
        CResourceCache<ITexture> textureCache;
        ITexturePtr texture = textureCache.AcquireResource(model3D->TextureName(group), false,
                                                           [](const std::string& filename) -> ITexturePtr
        {
           CResourceCache<IImage> imageCache;
           IImagePtr image = imageCache.AcquireResource(filename, false,
                                                        [](const std::string& filename) -> IImagePtr
           {
#ifdef OS_KOS
               IImagePtr resultImage(new CImageDreamPVR(filename));
#else
               IImagePtr resultImage(new CImagePVR(filename));
#endif
               if (!resultImage->Load())
               {
                   resultImage = nullptr;
               }
                
               return resultImage;
           });
           
           ITexturePtr resultTexture = GRenderer->CreateTexture();
           if (image)
           {
               resultTexture->AssignImage(image);
           }
           
           return resultTexture;
        });
        
        // Render component
        renderComponent->Batchable(false);
        renderComponent->Shader(shaderProgram, group);
        renderComponent->Material(material, group);
        renderComponent->Mesh(mesh, group);
        renderComponent->Texture(texture, group);
        if (cameraId != std::numeric_limits<uint32_t>::max())
        {
            renderComponent->AddCameraId(cameraId);
        }
    });
    
    // Transform component
    CTransformationComponentPtr transformComponent(new CTransformationComponent());
    CUpdateComponentPtr updateComponent(new CUpdateComponent());
    CBatchComponentPtr batchComponent(new CBatchComponent());
     
    CObject3DPtr entity(new CObject3D());
    entity->Initialize(filename, {
                                  renderComponent,
                                  transformComponent,
								  updateComponent,
                                  //batchComponent
                                 });
    // Store links to components
    entity->m_RenderComponent = renderComponent;
    entity->m_TransformationComponent = transformComponent;
    
    updateComponent->SetUpdateFunc(std::bind(&CObject3D::Update, entity.get(), std::placeholders::_1));
    updateComponent->Dirty();

    return entity;
}

CObject3D::CObject3D()
{
    
}

CObject3D::~CObject3D()
{

}

CRenderComponentPtr CObject3D::RenderComponent() const
{
    return m_RenderComponent;
}

CTransformationComponentPtr CObject3D::TransformComponent() const
{
    return m_TransformationComponent;
}

void CObject3D::Position(const glm::vec3& position)
{
    CTransformAffector::Position(TransformComponent(), position);
}

const glm::vec3& CObject3D::Position()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Position();
}

void CObject3D::Rotation(const glm::vec3& rotation)
{
    CTransformAffector::Rotation(TransformComponent(), rotation);
}

const glm::vec3& CObject3D::Rotation()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Rotation();
}

void CObject3D::Scale(const glm::vec3& scale)
{
    CTransformAffector::Scale(TransformComponent(), scale);
}

const glm::vec3& CObject3D::Scale()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Scale();
}

void CObject3D::AnchorPoint(const glm::vec3& anchorPoint)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Animation);
    transform.Position(anchorPoint);
    component->AddTransform(CTransformationComponent::Animation, transform);
    component->Dirty();
}

const glm::vec3& CObject3D::AnchorPoint()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Animation);
    return transform.Position();
}

void CObject3D::Update(unsigned long dt)
{
	
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
