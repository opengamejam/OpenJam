//
//  IScene.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "IScene.h"
#include "IEventDispatcher.hpp"
#include "IEntity.h"


using namespace jam;

IScene::IScene(CGameWeak game)
    : m_Game(game)
    , m_Root(new IEntity())
{
}

IScene::~IScene()
{

}

CGameWeak IScene::Game() const
{
    return m_Game;
}

const IScene::TCamerasList& IScene::Cameras() const
{
    return m_Cameras;
}

void IScene::AddCamera(ICameraPtr camera)
{
    assert(camera);
    if (camera)
    {
        m_Cameras.push_back(camera);
    }
}

void IScene::RemoveCamera(ICameraPtr camera)
{
    // TODO:
}

void IScene::RemoveCameraById(unsigned int cameraId)
{
    // TODO:
}

void IScene::RemoveAllCameras()
{
    m_Cameras.clear();
}

IEntityPtr IScene::Root()
{
    return m_Root;
}

void IScene::Update(unsigned long dt)
{    
    
}