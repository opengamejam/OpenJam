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
    uint32_t TaskCount();
    
    /*
     * Returns true if no tasks to execute
     */
    bool IsEmpty();

private:
    void ThreadCall();

private:
    std::condition_variable m_ConditionVariable;
    std::queue<CTask> m_Tasks;
    std::mutex m_Mutex;
    std::thread m_Thread;
    bool m_IsEnabled;
};

}; // namespace jam

#endif /* defined(CTHREADEXECUTOR_H) */
