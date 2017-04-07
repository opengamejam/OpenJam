//
//  CSemaphore.h
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef CSEMAPHORE_H
#define CSEMAPHORE_H

#include "Global.h"

namespace jam {
    
class CSemaphore final
{
public:
    /*
     * Constructor
     */
    CSemaphore();
    
    /*
     * Destructor
     */
    ~CSemaphore();
    
    /*
     * Increment the counter
     */
    void Notify();
    
    /*
     * Decrement the counter. If the resulting value is eaual to zero,
     * this function waits for a signal to occur before returning.
     */
    void Wait();
    
    /*
     * Decrement the counter if counter greater than 0 and returns true. In other case returns false
     */
    bool TryWait();
    
private:
    std::mutex m_Mutex;
    std::condition_variable m_Condition;
    size_t m_Count;
};
    
}; // namespace jam

#endif /* CSEMAPHORE_H */
