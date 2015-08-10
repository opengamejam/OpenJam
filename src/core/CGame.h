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
CLASS_PTR(IScene);
CLASS_PTR(IRenderView);
CLASS_PTR(ICamera);
CLASS_PTR(ISystem);

class CGame : public IEventable
{
public:
    typedef std::unordered_map<std::type_index, ISystemPtr> TSystemMap;
    
public:
    
	CGame(IRenderViewPtr render);
    virtual ~CGame();

    
    void Initialize();
    void Destroy();
    bool IsInitialized() const;
    
    IRenderViewPtr RenderView() const;
    
    void Update(unsigned long dt);
    void Draw();
    
    void PushScene(IScenePtr state);
    void PopScene();
    IScenePtr GetScene() const;
    
    void AddSystem(ISystemPtr system);
    void RemoveSystem(ISystemPtr system);
    ISystemPtr GetSystem(const std::type_index& systemKey);

private:
    CGame(const CGame& orig) {}
    CGame& operator=(const CGame& other);
   
private:
    bool m_IsInitialized;
	
    IRenderViewPtr m_RenderView;
    
    std::stack<IScenePtr> m_Scenes;
    TSystemMap m_System;
};

}; // namespace jam

#endif	/* CGAME_H */

