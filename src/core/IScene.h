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
#include "IEventable.h"

namespace jam
{
    
CLASS_PTR(IEventDispatcher);
CLASS_PTR(ICamera);
CLASS_PTR(IEntity);
CLASS_WEAK(CGame);
    
class IScene : public IEventable
{
public:
    typedef std::vector<ICameraPtr> TCamerasList;
    
public:
    IScene(CGameWeak game);
    virtual ~IScene();
    
	virtual void Start() = 0;
    virtual void Pause() = 0;
    virtual void Resume() = 0;
	virtual void Stop() = 0;
	virtual void Update(unsigned long dt);
    
    virtual CGameWeak Game() const;
    virtual const TCamerasList& Cameras() const;
    virtual void AddCamera(ICameraPtr camera);
    virtual void RemoveCamera(ICameraPtr camera);
    virtual void RemoveCameraById(unsigned int cameraId);
    virtual void RemoveAllCameras();

    virtual IEntityPtr Root();
    
private:
    CGameWeak m_Game;
    IEntityPtr m_Root;
    
    TCamerasList m_Cameras;
};

}; // namespace jam

#endif	/* ISCENE_H */

