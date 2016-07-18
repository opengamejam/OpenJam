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
#include "CTransformAffector.h"
#include "CThreadPool.h"
#include "IShaderProgram.h"
#include "CTransformationComponent.h"

#include "CEventComponent.h"
#include "CTouchEvent.h"

#include "CVirtualFileSystem.h"
#include "CZipFileSystem.h"

using namespace jam;

namespace jam
{
CLASS_PTR(IModel3D)
}

CGameScene::CGameScene(CGamePtr game)
    : CSceneBase(game)
    , m_IsPaused(false)
    , m_MainCamera(nullptr)
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

static glm::vec3 dir = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 lightPos = glm::vec3(-3.0f, -3.0f, -3.0f);

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
    std::for_each(m_Models3D.begin(), m_Models3D.end(), [&](jam::IEntityPtr entity)
    {
        entity->Get<CRenderComponent>([&](CRenderComponentPtr c)
        {
            const std::set<std::string>& groups = c->Groups();
            std::for_each(groups.begin(), groups.end(), [&](const std::string& groupName)
            {
                //c->Shader(groupName)->BindUniformfv("LightDir", {lightPos.x, lightPos.y, lightPos.z});
            });
        });
        
        if (k++ < 3)
        {
            //return;
        }
        
        CTransformAffector::Rotating(entity, glm::vec3(1 * 3.14f / 180.0f,
                                                       1 * 3.14f / 180.0f,
                                                       1 * 3.14f / 180.0f));
    });
    
    
}

CSprite2DPtr sprite;

void CGameScene::CreateMainCamera()
{
    IRenderViewPtr renderView = Game()->RenderView();
    IRendererPtr renderer = renderView->Renderer();
    m_UICamera = CCamera2D::Create(renderView->Width(), renderView->Height());
    m_MainCamera = CCamera3D::Create(60.0f, renderView->Width(), renderView->Height());
    
    m_UICamera->RenderTarget(renderView->DefaultRenderTarget());
    m_MainCamera->RenderTarget(renderView->DefaultRenderTarget());

    AddCamera(m_MainCamera);
    AddCamera(m_UICamera);
    
    CSprite2DPtr ball = CSprite2D::Create("/ball_glitch/sprite.mpf", renderer, m_UICamera->Id());
    CTransformAffector::Translating(ball, glm::vec3(0.0f, 0.0f, 1.0f));
    
    sprite = CSprite2D::Create("/bat_glitch/sprite.mpf", renderer, m_UICamera->Id());
    CTransformAffector::Translating(sprite, glm::vec3(100.0f, 100.0f, 0.0f));

    sprite->AddChild(ball);
    Root()->AddChild(sprite);
    
    CEventComponentPtr eventComponent(new CEventComponent());
    eventComponent->RegisterHandler(CTypeId<CTouchEvent>::Id(),
                                    std::bind(&CGameScene::OnBallMoved, this, std::placeholders::_1));
    sprite->AddComponent(eventComponent);

    std::shared_ptr<CGameScene> scene = std::static_pointer_cast<CGameScene>(shared_from_this());
    
    CThreadPool::Get()->RunAsync(CThreadPool::Background, [scene, renderer]()
    {
        std::string filename = "/cube/cube.obj";
        IModel3DPtr resultModel(new CModelObj(filename));
        resultModel->Load();
        
        CThreadPool::Get()->RunAsync(CThreadPool::Main, [scene, renderer, filename, resultModel]()
        {
            CResourceCache<IModel3D> resourceCache;
            resourceCache.CacheResource(filename, false, resultModel);
            
            int y = 0;
            int x = 0;
            for (int i = 0; i < 16; ++i)
            {
            	if (i != 0 && i % 4 == 0)
            	{
            		y++;
            		x = 0;
            	}

            	jam::CObject3DPtr model3D = CObject3D::CreateObj(filename, renderer, scene->m_MainCamera->Id());
            	CTransformAffector::Translating(model3D, glm::vec3(x * 2.4f - 4.0f, y * 2.4f - 4.0f, 0.0f));
            	CTransformAffector::Rotation(model3D, glm::vec3(i * 15 * 3.1415 / 180.0,
                                                                         i * 15 * 3.1415 / 180.0,
                                                                         i * 15 * 3.1415 / 180.0));
                //CTransformAffector::Scale(model3D, glm::vec3(10.0f, 10.0f, 10.0f));
            	scene->Root()->AddChild(model3D);
                scene->m_Models3D.push_back(model3D);

            	x++;
            }
        });
    });
}

bool CGameScene::OnBallMoved(IEventPtr event)
{
    CTouchEventPtr touchEvent = std::static_pointer_cast<CTouchEvent>(event);
    
    CTransformAffector::Position(sprite, glm::vec3(touchEvent->GetPosition().x,
                                                   touchEvent->GetPosition().y,
                                                   0.0));
    
    return true;
}

