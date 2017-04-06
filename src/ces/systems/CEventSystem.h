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
#include "CEventComponent.h"

namespace jam {
CLASS_PTR(CEventSystem)
CLASS_PTR(IEvent)

class CEventSystem : public CSystemBase<CEventComponent> {
    JAM_OBJECT
public:
    CEventSystem();
    virtual ~CEventSystem();

    void DispatchEvent(IEventPtr event);

    virtual void Update(unsigned long dt) override;
};

} // namespace jam

#endif /* CEventSystem_hpp */
