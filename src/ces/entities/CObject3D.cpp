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
#include "CTransform.h"
#include "CVertex.h"
#include "CColor.h"
#include "CShaderSourceSprite.h"
#include "CImagePVR.h"
#include "CImageSegaPVR.h"
#include "CModelObj.h"
#include "CTransformationComponent.h"
#include "CResourceCache.hpp"
#include "CTransformAffector.h"
#include "CUpdateComponent.h"

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
        material->PrimitiveType(IMaterial::Triangles);
        material->CullFace(false); // TODO: temp
        material->DepthEnable(true);
        
        // Vertex buffer
        uint32_t elementSize = 0;
        if (model3D->Vertices(group).size() > 0)
        {
            elementSize += sizeof(CVector3Df);
        }
        if (model3D->UVs(group).size() > 0)
        {
            elementSize += sizeof(CVector2Df);
        }
        if (model3D->Normals(group).size() > 0)
        {
            elementSize += sizeof(CVector3Df);
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
                position.Set<CVector3Df>(0, model3D->Vertices(group));
                
                offset += sizeof(CVector3Df);
            }
            
            if (model3D->UVs(group).size() > 0)
            {
                IVertexBuffer::SVertexStream& textureCoord = vertexBuffer->Lock(IVertexBuffer::TextureCoords);
                textureCoord.attributeIndex = shaderProgram->TextureCoord();
                textureCoord.dataType = IVertexBuffer::Float;
                textureCoord.stride = 2;
                textureCoord.offset = offset;
                textureCoord.Set<CVector2Df>(0, model3D->UVs(group));
                
                offset += sizeof(CVector2Df);
            }
            
            if (model3D->Normals(group).size() > 0)
            {
                IVertexBuffer::SVertexStream& normals = vertexBuffer->Lock(IVertexBuffer::Normal);
                normals.attributeIndex = shaderProgram->NormalTexture();
                normals.dataType = IVertexBuffer::Float;
                normals.stride = 3;
                normals.offset = offset;
                normals.Set<CVector3Df>(0, model3D->Normals());
                
                offset += sizeof(CVertex3Df);
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
     
    CObject3DPtr entity(new CObject3D());
    entity->Initialize(filename, {
                                  renderComponent,
                                  transformComponent,
								  updateComponent
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

void CObject3D::Position(const CVector3Df& position)
{
    CTransformAffector::Position(TransformComponent(), position);
}

const CVector3Df& CObject3D::Position()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Position();
}

void CObject3D::Rotation(const CVector3Df& rotation)
{
    CTransformAffector::Rotation(TransformComponent(), rotation);
}

const CVector3Df& CObject3D::Rotation()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Rotation();
}

void CObject3D::Scale(const CVector3Df& scale)
{
    CTransformAffector::Scale(TransformComponent(), scale);
}

const CVector3Df& CObject3D::Scale()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Scale();
}

void CObject3D::AnchorPoint(const CVector3Df& anchorPoint)
{
    CTransformationComponentPtr component = TransformComponent();
    CTransform3Df transform = component->Transform(CTransformationComponent::Local);
    transform.Offset(anchorPoint);
    component->AddTransform(CTransformationComponent::Local, transform);
    component->Dirty();
}

const CVector3Df& CObject3D::AnchorPoint()
{
    CTransformationComponentPtr component = TransformComponent();
    const CTransform3Df& transform = component->Transform(CTransformationComponent::Local);
    return transform.Offset();
}

void CObject3D::Update(unsigned long dt)
{
	CTransformAffector::Rotating(shared_from_this(), CVector3Df(1 * 3.14f / 180.0f,
																1 * 3.14f / 180.0f,
																1 * 3.14f / 180.0f));
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************
