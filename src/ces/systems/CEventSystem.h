//
//  CEventSystem.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/16/16.
//
//

#ifndef CEventSystem_hpp
#define CEventSystem_hpp

#include "ISystem.h"

namespace jam
{
CLASS_PTR(CEventSystem);
CLASS_PTR(IEvent);

class CEventSystem : public ISystem
{
    JAM_OBJECT
public:
    CEventSystem();
    virtual ~CEventSystem();
    
    virtual void Update(unsigned long dt) override;
    
protected:
    virtual void OnChangedComponent(IComponentPtr component) override;
    
private:
    void DispatchEvent(IEventPtr event);
};
    
} // namespace jam

#endif /* CEventSystem_hpp */
