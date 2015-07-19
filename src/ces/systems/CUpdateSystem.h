//
//  CUpdateSystem.h
//  OpenJam
//
//  Created by Yevgeniy Logachev
//  Copyright (c) 2014 yev. All rights reserved.
//

#ifndef CUPDATESYSTEM_H
#define CUPDATESYSTEM_H

#include "ISystem.h"

namespace jam
{
CLASS_PTR(CUpdateSystem);

class CUpdateSystem : public ISystem
{
public:
    CUpdateSystem();
    virtual ~CUpdateSystem();
    
    void Update(unsigned long dt);
};
    
} // namespace jam


#endif /* CUPDATESYSTEM_H */
