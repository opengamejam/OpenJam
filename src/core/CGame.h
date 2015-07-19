/* 
 * File:   CGame.h
 * Author: yevgeniy.logachev
 */

#ifndef CGAME_H
#define	CGAME_H

#include "Global.h"
#include "IEventable.h"

namespace jam
{
CLASS_PTR(CGame);    
CLASS_PTR(IGameObject);
CLASS_PTR(IScene);
CLASS_PTR(IRenderView);
CLASS_PTR(ICamera);
CLASS_PTR(ISystem);

class CGame : public IEventable
{
public:
    typedef std::map<std::type_index, ISystemPtr> TSystemMap;
    typedef std::priority_queue<ISystemPtr> TSystemPQueue;

    
public:
    
	CGame(IRenderViewPtr render);
    virtual ~CGame();

    
    void Start();
    INL void Stop();
    INL void Pause(bool value);
    INL bool IsRunning() const;
    
    IRenderViewPtr RenderView() const;
    
    void Update(unsigned long dt);
    void Draw();
    
    void PushScene(IScenePtr state);
    INL void PopScene();
    IScenePtr GetScene() const;
    
    void AddSystem(ISystemPtr system);
    void RemoveSystem(ISystemPtr system);
    ISystemPtr GetSystem(const std::type_index& systemKey);

private:
    CGame(const CGame& orig) {}
    CGame& operator=(const CGame& other);
    
    void ThreadUpdate();
    void DrawObject(ICameraPtr camera, IGameObjectPtr gameObject);
   
private:
    bool m_IsRunning;
	
    IRenderViewPtr m_RenderView;
    std::thread m_WorkerThread;
    
    std::stack<IScenePtr> m_Scenes;
    TSystemMap m_System;
};

}; // namespace jam

#endif	/* CGAME_H */

