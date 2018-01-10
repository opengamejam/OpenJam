/* 
 * File:   CGameScene.h
 * Author: yevgeniy.logachev
 */

#ifndef CGAMESCENE_H
#define CGAMESCENE_H

#include "IScene.h"

namespace jam {
CLASS_PTR(CObject3D)
CLASS_PTR(IEvent)
CLASS_PTR(COperationQueue)
}

class CGameScene : public jam::CSceneBase {
    JAM_OBJECT
public:
    CGameScene(jam::CGamePtr game);
    virtual ~CGameScene();

    virtual void Start() override;
    virtual void Pause() override;
    virtual void Resume() override;
    virtual void Stop() override;
    virtual void Update(unsigned long dt) override;

private:
    void CreateMainCamera();
    bool OnBallMoved(jam::IEventPtr event);

private:
    bool m_IsPaused;
    jam::ICameraPtr m_MainCamera;
    jam::ICameraPtr m_UICamera;
    std::list<jam::CObject3DPtr> m_Models3D;
    
    jam::COperationQueuePtr m_BackgroundQueue;
};

#endif /* CGAMESCENE_H */

