//
//  IScene.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#include "IScene.h"
#include "IEntity.h"

using namespace jam;

CSceneBase::CSceneBase(CGamePtr game)
    : m_Game(game)
    , m_Root(new CEntityBase())
{
}

CSceneBase::~CSceneBase()
{
}

CGamePtr CSceneBase::Game() const
{
    return m_Game.lock();
}

const IScene::TCamerasList& CSceneBase::Cameras() const
{
    return m_Cameras;
}

void CSceneBase::AddCamera(ICameraPtr camera)
{
    assert(camera);
    if (camera) {
        m_Cameras.push_back(camera);
    }
}

void CSceneBase::RemoveCamera(ICameraPtr camera)
{
    // TODO:
    assert(false);
}

void CSceneBase::RemoveCameraById(uint32_t cameraId)
{
    // TODO:
    assert(false);
}

void CSceneBase::RemoveAllCameras()
{
    m_Cameras.clear();
}

IEntityPtr CSceneBase::Root()
{
    return m_Root;
}

void CSceneBase::Update(unsigned long dt)
{
}
