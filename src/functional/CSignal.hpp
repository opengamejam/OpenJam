//
//  CSignal.h
//  TestApp
//
//  Created by Yevgeniy Logachev on 6/17/16.
//
//

#ifndef CSIGNAL_H
#define CSIGNAL_H

#include "Global.h"
#include "CSlot.hpp"

#define emit
#define signals public

namespace jam
{
    
template <typename R, typename... Args>
class CSignal final
{
public:
    CSignal()
    {
        
    }
    
    ~CSignal()
    {
        
    }
    
    void operator += (const CSlot<R, Args...>& slot)
    {
        std::unique_lock<std::mutex> locker(m_Mutex);
        m_Slots.push_back(slot);
    }
    
    void operator -= (const CSlot<R, Args...>& slot)
    {
        std::unique_lock<std::mutex> locker(m_Mutex);
        typename TSlotsList::const_iterator it = std::find(m_Slots.begin(), m_Slots.end(), slot);
        m_Slots.erase(it);
    }
    
    void operator ()(Args... args)
    {
        std::unique_lock<std::mutex> locker(m_Mutex);
        std::for_each(m_Slots.begin(), m_Slots.end(), [&](const typename TSlotsList::value_type& slot)
        {
            locker.unlock();
            slot(args...);
            locker.lock();
        });
    }
    
private:
    typedef std::list<CSlot<R, Args...>> TSlotsList;
    TSlotsList m_Slots;
    std::mutex m_Mutex;
};

}; // namespace jam

#endif /* CSIGNAL_H */
