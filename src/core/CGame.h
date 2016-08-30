/* 
 * File:   CGame.h
 * Author: yevgeniy.logachev
 */

#ifndef CGAME_H
#define	CGAME_H

#include "Global.h"

namespace jam
{
CLASS_PTR(CGame)
CLASS_PTR(IScene)
CLASS_PTR(IRenderView)
CLASS_PTR(ICamera)
CLASS_PTR(ISystem)
CLASS_PTR(CRenderSystem)
CLASS_PTR(CEventSystem)
CLASS_PTR(CTouchEvent)
    
class CGame
{
public:
    typedef std::unordered_map<typeid_t, ISystemPtr> TSystemMap;
    
public:
    
	CGame(IRenderViewPtr render);
    virtual ~CGame();

    
    void Initialize();
    void Shutdown();
    bool IsInitialized() const;
    
    IRenderViewPtr RenderView() const;
    
    void Update(unsigned long dt);
    void Draw();
    
    void PushScene(IScenePtr state);
    void PopScene();
    IScenePtr GetScene() const;
    
    void AddSystem(ISystemPtr system);
    void RemoveSystem(ISystemPtr system);
    ISystemPtr GetSystem(typeid_t systemKey);
    
    CRenderSystemPtr RenderSystem() const;
    CEventSystemPtr EventSystem() const;

private:
    CGame(const CGame& orig) {}
    CGame& operator=(const CGame& other);
    
    void InitializeFileSystems();
    void InitializeSystems();
    void ShutdownFileSystems();
    void ShutdownSystems();
    
    void OnTouchEvent(CTouchEventPtr event);
   
private:
    bool m_IsInitialized;
	
    IRenderViewPtr m_RenderView;
    
    std::stack<IScenePtr> m_Scenes;
    TSystemMap m_System;
    CRenderSystemPtr m_RenderSystem;
    CEventSystemPtr m_EventSystem;
};

}; // namespace jam

#endif	/* CGAME_H */

