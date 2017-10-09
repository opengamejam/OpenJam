/*
 * File:   CGameScene.cpp
 * Author: yevgeniy.logachev
 */

#include "CGameScene.h"

#include "CSystem.h"

// Level
#include "CGame.h"
#include "CCamera2D.h"
#include "CCamera3D.h"
#include "IRenderView.h"

#include "CSprite2D.h"
#include "CObject3D.h"
#include "CModelObj.h"
#include "CResourceCache.hpp"
#include "CRenderComponent.h"
#include "CThreadPool.h"
#include "IShaderProgram.h"
#include "IFrameBuffer.h"
#include "IRenderTarget.h"
#include "IRenderer.h"

#include "CTransformationComponent.h"
#include "CEventComponent.h"
#include "CTouchEvent.h"

#include "CVirtualFileSystem.h"
#include "CZipFileSystem.h"
#include "CShaderSourceSprite.h"
#include "CShaderSourceCommon.h"

#include <unistd.h>

#include "COperationQueue.h"
#include "CBlockOperation.h"

using namespace jam;

namespace jam
{
CLASS_PTR(IModel3D)
}

CGameScene::CGameScene(CGamePtr game)
    : CSceneBase(game), m_IsPaused(false), m_MainCamera(nullptr)
{
}

CGameScene::~CGameScene()
{
}

void CGameScene::Start()
{
    CreateMainCamera();
    srand((unsigned int)CSystem::GetTickCount());
}

void CGameScene::Pause()
{
    m_IsPaused = true;
}

void CGameScene::Resume()
{
    m_IsPaused = false;
}

void CGameScene::Stop()
{
}

CSprite2DPtr sprite;
jam::CObject3DPtr tv;

static glm::vec3 dir = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, 13.0f);

void CGameScene::Update(unsigned long dt)
{
    CSceneBase::Update(dt);

    const float speed = 10.0f;

    lightPos.x += dir.x * speed * (dt / 1000.0f);
    lightPos.y += dir.y * speed * (dt / 1000.0f);
    if (lightPos.x > 14.0f || lightPos.x < -14.0)
    {
        dir.x *= -1;
    }
    if (lightPos.y > 10.0f || lightPos.y < -10.0)
    {
        dir.y *= -1;
    }

    int k = 0;
    std::for_each(m_Models3D.begin(), m_Models3D.end(), [&](jam::IEntityPtr entity) {
        entity->Get<CRenderComponent>([&](CRenderComponentPtr c) {
            const std::set<std::string> &groups = c->Groups();
            std::for_each(groups.begin(), groups.end(), [&](const std::string &groupName) {
                //c->Shader(groupName)->BindUniformfv("LightDir", {lightPos.x, lightPos.y, lightPos.z});
            });
        });

        if (k++ < 3)
        {
            //return;
        }

        const glm::vec3 &rotation = entity->Rotation();
        entity->Rotation(glm::vec3(rotation.x + 1 * 3.14f / 180.0f,
                                   rotation.y + 1 * 3.14f / 180.0f,
                                   rotation.z + 1 * 3.14f / 180.0f));
    });

    static float tv_rot = 0;
    static int tv_rot_dir = 1;
    tv_rot += (tv_rot_dir * 0.5);
    if (tv_rot > 50)
    {
        tv_rot_dir = -1;
    }
    else if (tv_rot < -50)
    {
        tv_rot_dir = 1;
    }
    tv->Rotation(glm::vec3(00.0f * 3.14f / 180.0f, tv_rot * 3.14 / 180.0f, 0));
    sprite->Rotation(glm::vec3(0, 0, tv_rot * 3.14 / 180.0f));
}

void CGameScene::CreateMainCamera()
{
    IRenderViewPtr renderView = Game()->RenderView();
    IRendererPtr renderer = renderView->Renderer();
    m_UICamera = CCamera2D::Create();
    m_MainCamera = CCamera3D::Create(90.0f);

    m_UICamera->FrameBuffer(renderView->DefaultFrameBuffer());
    m_MainCamera->FrameBuffer(renderView->DefaultFrameBuffer());

    AddCamera(m_MainCamera);
    AddCamera(m_UICamera);

    CSprite2DPtr ball = CSprite2D::Create("/ball_glitch/sprite.mpf", renderer);
    ball->Position(glm::vec3(0.0f, 0.0f, 1.0f));

    sprite = CSprite2D::Create("/bat_glitch/sprite.mpf", renderer);
    sprite->Position(glm::vec3(100.0f, 100.0f, 0.0f));
    sprite->Rotation(glm::vec3(0, 0, 45.0f * 3.14f / 180.0f));
    //sprite->Scale(glm::vec3(3.0f, 3.0f, 3.0f));

    sprite->AddChild(ball);
    Root()->AddChild(sprite);

    CEventComponentPtr eventComponent(new CEventComponent());
    eventComponent->RegisterHandler(CTypeId<CTouchEvent>::Id(),
                                    std::bind(&CGameScene::OnBallMoved, this, std::placeholders::_1));
    sprite->AddComponent(eventComponent);

    // Render texture
    CRenderTargetTexturePtr renderTextureTarget = renderer->CreateTextureRenderTarget();
    renderTextureTarget->Initialize(IRenderTarget::ColorRGBA8888);

    CRenderTargetDepthPtr depthTarget = renderer->CreateDepthRenderTarget();
    depthTarget->Initialize(IRenderTarget::Depth24_Stencil8);

    // Render texture framebuffer
    IFrameBufferPtr renderTextureFBO = renderer->CreateFrameBuffer(512, 512);
    renderTextureFBO->Initialize();
    renderTextureFBO->Bind();
    renderTextureFBO->AttachColor(renderTextureTarget, 0);
    renderTextureFBO->AttachDepth(depthTarget);
    renderTextureFBO->ClearColor(CColor4f(0.0f, 0.0f, 1.0f, 0.0f));
    assert(renderTextureFBO->IsValid());

    // Cam
    m_MainCamera->FrameBuffer(renderTextureFBO, 1);

    // TV object
    tv = CObject3D::CreateObj("/tv/tv.obj", renderer);
    Root()->AddChild(tv);
    tv->Position(glm::vec3(7.0f, 3.0f, 0.0f));
    tv->Rotation(glm::vec3(10.0f * 3.14f / 180.0f, 0, 0));
    tv->Scale(glm::vec3(3.0f, 3.0f, 3.0f));

    tv->RenderComponent()->Batchable(false);
    tv->RenderComponent()->Dirty();

    // Display obj
    jam::CObject3DPtr plane = CObject3D::CreateObj("/plane/plane.obj", renderer);
    tv->AddChild(plane);
    plane->Position(glm::vec3(-0.12f, 0.43f, 0.364f));
    plane->Rotation(glm::vec3(0, 0, 0));
    plane->Scale(glm::vec3(0.36f, 0.27f, 1.0f));

    plane->Get<CRenderComponent, true>([&](CRenderComponentPtr component) {
        CShaderSourceSprite shaderSource;
        IShaderPtr vertexShader = renderer->CreateShader();
        vertexShader->Compile(shaderSource.Vertex(), IShader::Vertex);
        assert(vertexShader);

        IShaderPtr fragmentShader = renderer->CreateShader();
        fragmentShader->Compile(shaderSource.Fragment(), IShader::Fragment);
        assert(fragmentShader);

        IShaderProgramPtr shaderProgram = renderer->CreateShaderProgram();
        shaderProgram->AttachShader(vertexShader);
        shaderProgram->AttachShader(fragmentShader);
        shaderProgram->Link();

        component->Batchable(false);
        component->Texture(renderTextureTarget->Texture());
        component->Shader(shaderProgram);
    });

    std::string filename = "/cube/cube.obj";
    std::shared_ptr<CGameScene> scene = std::static_pointer_cast<CGameScene>(shared_from_this());
    
    CBlockOperationPtr makeSceneOp(new CBlockOperation());
    makeSceneOp->AddExecutionBlock([scene, renderer, filename]() {
        IModel3DPtr model3D(new CModelObj(filename));
        model3D->Load();
        
        CResourceCache<IModel3D> resourceCache;
        resourceCache.CacheResource(filename, model3D);
        
        sleep(3);
        
        CThreadPool::Get()->RunSync(CThreadPool::Main, [scene, renderer, model3D]() {
            int y = 0;
            int x = 0;
            for (int i = 0; i < 16; ++i)
            {
                if (i != 0 && i % 4 == 0)
                {
                    y++;
                    x = 0;
                }
                
                jam::CObject3DPtr cube = CObject3D::CreateObj(model3D->Filename(), renderer);
                cube->Position(glm::vec3(x * 2.4f - 4.0f, y * 2.4f - 4.0f, 0.0f));
                cube->Rotation(glm::vec3(i * 15 * 3.1415 / 180.0,
                                         i * 15 * 3.1415 / 180.0,
                                         i * 15 * 3.1415 / 180.0));
                //cube->Scale(glm::vec3(10.0f, 10.0f, 10.0f));
                
                scene->Root()->AddChild(cube);
                scene->m_Models3D.push_back(cube);
                
                x++;
            }
        });
    });
    
    m_BackgroundQueue = COperationQueue::CreateOperationQueue();
    m_BackgroundQueue->MaxConcurrentOperationCount(1);
    m_BackgroundQueue->AddOperation(makeSceneOp);
}

bool CGameScene::OnBallMoved(IEventPtr event)
{
    CTouchEventPtr touchEvent = std::static_pointer_cast<CTouchEvent>(event);

    sprite->Position(glm::vec3(touchEvent->GetPosition().x,
                               touchEvent->GetPosition().y,
                               0.0));

    return true;
}
