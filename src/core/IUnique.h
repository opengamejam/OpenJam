//
//  IUnique.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 8/26/15.
//
//

#ifndef TestApp_IUnique_h
#define TestApp_IUnique_h

#include "Global.h"

namespace jam
{
    
class IUnique
{
public:
    Unique();
    virtual Unique();
    
    uint64_t UniqueId() const;
    
private:
    static uint64_t NextId();
    
private:
    static uint64_t s_NextId;
    uint64_t m_UniqueId;
};
    
}; // namespace jam

#endif
