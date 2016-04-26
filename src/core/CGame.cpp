/* 
 * File:   CGame.cpp
 * Author: yevgeniy.logachev
 */

#include "CGame.h"
#include "IRenderView.h"
#include "CSystem.h"
#include "IScene.h"
#include "CLoaderFile.h"
#include "IEventDispatcher.hpp"

#include "CRenderSystem.h"
#include "CAnimation2DSystem.h"
#include "CTransformationSystem.h"
#include "CUpdateSystem.h"
#include "CBatchingSystem.h"
#include "IRenderTarget.h"
#include "CThreadPool.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CGame::CGame(IRenderViewPtr render)
    : m_IsInitialized(false)
    , m_RenderView(render)
{
    CThreadPool::Get()->Initialize(5);
    
    CLoaderFile* loaderFile = new CLoaderFile();
    loaderFile->RegisterFileSystem(CSystem::GetBundlePath() + "media/");
    loaderFile->RegisterFileSystem(CSystem::GetBundlePath());
    
    ILoader::RegisterDefaultLoader(loaderFile);
}

CGame::~CGame() 
{
    CThreadPool::Get()->Shutdown();
}

void CGame::Initialize()
{
    m_RenderView->CreateView();
    
    m_RenderSystem.reset(new CRenderSystem(m_RenderView->Renderer()));
    CAnimation2DSystemPtr animationSystem(new CAnimation2DSystem());
    CTransfromationSystemPtr transformationSystem(new CTransfromationSystem());
    CUpdateSystemPtr updateSystem(new CUpdateSystem());
    CBatchingSystemPtr batchingSystem(new CBatchingSystem(m_RenderView->Renderer()));
    AddSystem(updateSystem);
    AddSystem(animationSystem);
    AddSystem(batchingSystem);
    AddSystem(transformationSystem);
    AddSystem(m_RenderSystem);
    
    m_IsInitialized = true;
}

void CGame::Shutdown()
{
    m_IsInitialized = false;
}

bool CGame::IsInitialized() const
{
    return m_IsInitialized;
}

IRenderViewPtr CGame::RenderView() const
{
    return m_RenderView;
}

void CGame::Update(unsigned long dt)
{
    if (!IsInitialized())
    {
        return;
    }
    
    CThreadPool::Get()->Update(dt);
    Dispatcher()->Update(dt);
    
    if (!m_Scenes.empty())
    {
        IScenePtr scene = m_Scenes.top();
        scene->Update(dt);
    }
    
    std::for_each(m_System.begin(), m_System.end(), [dt](const TSystemMap::value_type& system)
    {
        system.second->Update(dt);
    });
}

void CGame::Draw()
{
    CRenderSystemPtr renderSystem = RenderSystem();
    /*std::type_index key = SystemId<CRenderSystem>();
    TSystemMap::const_iterator it = m_System.find(key);
    if (it != m_System.end())
    {
        renderSystem = std::static_pointer_cast<CRenderSystem>(it->second);
    }*/

    if (renderSystem && !m_Scenes.empty())
    {
        IScenePtr scene = m_Scenes.top();
        
        m_RenderView->Begin();
        const IScene::TCamerasList& cameras = scene->Cameras();
        std::for_each(cameras.begin(), cameras.end(), [&](ICameraPtr camera)
        {
            renderSystem->Draw(camera);
        });
        m_RenderView->End();
    }
}

void CGame::PushScene(IScenePtr scene)
{
    assert(scene);
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Pause();
    }
    
    m_Scenes.push(scene);
    m_Scenes.top()->Start();
}

void CGame::PopScene()
{
    assert(!m_Scenes.empty());
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Stop();
        m_Scenes.pop();
    }
}

IScenePtr CGame::GetScene() const
{
    if (!m_Scenes.empty())
    {
        return m_Scenes.top();
    }
    
    return nullptr;
}

void CGame::AddSystem(ISystemPtr system)
{
    ISystem& s = *system.get();
    std::type_index key = std::type_index(typeid(s));
    m_System[key] = system;
}

void CGame::RemoveSystem(ISystemPtr system)
{
    ISystem& s = *system.get();
    std::type_index key = std::type_index(typeid(s));
    TSystemMap::const_iterator it = m_System.find(key);
    if (it != m_System.end())
    {
        m_System.erase(it);
    }
}

ISystemPtr CGame::GetSystem(const std::type_index& systemKey)
{
    TSystemMap::const_iterator it = m_System.find(systemKey);
    if (it != m_System.end())
    {
        return it->second;
    }
    
    return nullptr;
}

CRenderSystemPtr CGame::RenderSystem() const
{
    return m_RenderSystem;
}

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

