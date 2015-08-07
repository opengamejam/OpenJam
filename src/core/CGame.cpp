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

#include "CImagePVR.h"
#include "CSpriteXML.h"

#include "CCamera2D.h"
#include "CTextureRenderTarget.h"

#include "CRenderSystem.h"
#include "CAnimation2DSystem.h"
#include "CTransformationSystem.h"
#include "CUpdateSystem.h"
#include "IRenderTarget.h"

using namespace jam;

// *****************************************************************************
// Constants
// *****************************************************************************


// *****************************************************************************
// Public Methods
// *****************************************************************************

CGame::CGame(IRenderViewPtr render)
    : m_IsRunning(false)
    , m_RenderView(render)
{
    CLoaderFile* loaderFile = new CLoaderFile();
    loaderFile->RegisterFileSystem(CSystem::GetBundlePath() + "media/");
    loaderFile->RegisterFileSystem(CSystem::GetBundlePath());
    
    ILoader::RegisterDefaultLoader(loaderFile);
}

CGame::~CGame() 
{
    if (m_WorkerThread.joinable())
    {
        m_WorkerThread.join();
    }
}

void CGame::Start()
{
    m_RenderView->CreateView();
    m_WorkerThread = std::thread(std::bind(&CGame::ThreadUpdate, this));
    
    CRenderSystemPtr renderSystem(new CRenderSystem());
    CAnimation2DSystemPtr animationSystem(new CAnimation2DSystem());
    CTransfromationSystemPtr transformationSystem(new CTransfromationSystem());
    CUpdateSystemPtr updateSystem(new CUpdateSystem());
    AddSystem(updateSystem);
    AddSystem(animationSystem);
    AddSystem(transformationSystem);
    AddSystem(renderSystem);
    
    m_IsRunning = true;
}

void CGame::Stop()
{
    m_IsRunning = false;
}

void CGame::Pause(bool value)
{
    m_IsRunning = value;
}

bool CGame::IsRunning() const
{
    return m_IsRunning;
}

IRenderViewPtr CGame::RenderView() const
{
    return m_RenderView;
}

void CGame::Update(unsigned long dt)
{
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
    CRenderSystemPtr renderSystem = nullptr;
    std::type_index key = SystemId<CRenderSystem>();
    TSystemMap::const_iterator it = m_System.find(key);
    if (it != m_System.end())
    {
        renderSystem = std::static_pointer_cast<CRenderSystem>(it->second);
    }

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

void CGame::PushScene(IScenePtr state)
{
    assert(state);
    if (!m_Scenes.empty())
    {
        m_Scenes.top()->Pause();
    }
    
    m_Scenes.push(state);
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
    std::type_index key = std::type_index(typeid(*system.get()));
    m_System[key] = system;
}

void CGame::RemoveSystem(ISystemPtr system)
{
    std::type_index key = std::type_index(typeid(*system.get()));
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

// *****************************************************************************
// Protected Methods
// *****************************************************************************

// *****************************************************************************
// Private Methods
// *****************************************************************************

void CGame::ThreadUpdate()
{
    while (m_IsRunning)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
}
