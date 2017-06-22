//
//  CThreadExecutor.h
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#ifndef CTHREADEXECUTOR_H
#define CTHREADEXECUTOR_H

#include "Global.h"
#include "CTask.h"

namespace jam {
CLASS_PTR(CThreadExecutor)

/*
 * Execute tasks on background thread
 */
class CThreadExecutor final {
public:
    /*
     * Constructor
     */
    CThreadExecutor();
    
    /*
     * Destructor
     */
    ~CThreadExecutor();

    /*
     * Add task to execution
     */
    void AddTask(const CTask& task);
    
    /*
     * Number of pending execution tasks
     */
    uint32_t TaskCount() const;
    
    /*
     * Returns true if no tasks to execute
     */
    bool IsEmpty() const;
    
    /*
     * Returns true if task executes now
     */
    bool IsExecuting() const;

private:
    void ThreadCall();

private:
    std::condition_variable m_ConditionVariable;
    std::queue<CTask> m_Tasks;
    mutable std::mutex m_Mutex;
    std::thread m_Thread;
    bool m_IsEnabled;
    bool m_IsExecuting;
};

}; // namespace jam

#endif /* defined(CTHREADEXECUTOR_H) */
