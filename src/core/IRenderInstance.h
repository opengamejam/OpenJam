//
//  IRenderInstance.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 11/8/17.
//  Copyright © 2017 Yevgeniy Logachev. All rights reserved.
//

#ifndef IRENDERINSTANCE_H
#define IRENDERINSTANCE_H

#include "Global.h"
#include "IObject.h"

namespace jam {
CLASS_PTR(IRenderInstance)

class IRenderInstance : public IObject<IRenderInstance> {
public:
    IRenderInstance() = default;
    virtual ~IRenderInstance() = default;
    
    /*
     * Initialize instance object
     */
    virtual void Initialize() = 0;
    
    /*
     * Deinitialize instance object
     */
    virtual void Shutdown() = 0;
    
    /*
     * Check if instance is initialized
     */
    virtual bool IsInitialized() const = 0;
};
    
}; // namespace jam

#endif /* IRENDERINSTANCE_H */
