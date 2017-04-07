//
//  CTask.hpp
//  OpenJam
//
//  Created by Yevgeniy Logachev on 4/7/17.
//
//

#ifndef CTASK_H
#define CTASK_H

#include "Global.h"

namespace jam {
    
CLASS_PTR(CSemaphore)
    
class CTask final
{
public:
    typedef std::function<void()> TExecuteBlock;
    
public:
    /*
     * Default constructor
     */
    CTask();
    
    /*
     * Constructor with execution block
     */
    CTask(const TExecuteBlock& executeBlock);
    
    /*
     * Constructor with execution block and shared semaphore
     */
    CTask(CSemaphorePtr semaphore, const TExecuteBlock& executeBlock);
    
    /*
     * Destructor
     */
    ~CTask();
    
    /*
     * Execute block and notify semaphore signal
     */
    void Execute();
    
private:
    TExecuteBlock m_ExecuteBlock;
    CSemaphorePtr m_Semaphore;
};
    
}; // namespace jam

#endif /* CTASK_H */
