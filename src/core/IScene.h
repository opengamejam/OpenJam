//
//  IScene.h
//  OpenJam
//
//  Created by yev
//  Copyright (c) 2014 Yevgeniy Logachev. All rights reserved.
//
#ifndef ISCENE_H
#define	ISCENE_H

#include "Global.h"

namespace jam
{
    
CLASS_PTR(IEventDispatcher)
CLASS_PTR(ICamera)
CLASS_PTR(IEntity)
CLASS_PTR(CGame)
    
/*
 * Interface IScene
 */
class IScene : public std::enable_shared_from_this<IScene>
{
public:
    typedef std::vector<ICameraPtr> TCamerasList;
    
public:
    IScene() = default;
    ~IScene() = default;
    
	virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
	virtual void Stop() = 0;
	virtual void Update(unsigned long dt) = 0;
    
    virtual CGamePtr Game() const = 0;
    virtual const TCamerasList& Cameras() const = 0;
    virtual void AddCamera(ICameraPtr camera) = 0;
    virtual void RemoveCamera(ICameraPtr camera) = 0;
    virtual void RemoveCameraById(uint32_t cameraId) = 0;
    virtual void RemoveAllCameras() = 0;

    virtual IEntityPtr Root() = 0;
};

/*
 * Base class CSceneBase
 */
class CSceneBase : public IScene
{
public:
    CSceneBase(CGamePtr game);
    virtual ~CSceneBase();
    
    virtual void Update(unsigned long dt) override;
    
    virtual CGamePtr Game() const override;
    virtual const TCamerasList& Cameras() const override;
    virtual void AddCamera(ICameraPtr camera) override;
    virtual void RemoveCamera(ICameraPtr camera) override;
    virtual void RemoveCameraById(uint32_t cameraId) override;
    virtual void RemoveAllCameras() override;
    
    virtual IEntityPtr Root() override;
    
private:
    CGameWeak m_Game;
    IEntityPtr m_Root;
    
    TCamerasList m_Cameras;
};

}; // namespace jam

#endif	/* ISCENE_H */

