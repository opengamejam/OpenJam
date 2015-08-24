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

namespace jam
{
CLASS_PTR(CThreadExecutor);
    
class CThreadExecutor 
{
public:
    typedef std::function<void()> TExecuteBlock;
    
public:
    static CThreadExecutorPtr Create();
    
    CThreadExecutor();
    virtual ~CThreadExecutor();
    
    void AddTask(const TExecuteBlock& block);
    uint32_t TaskCount();
    bool IsEmpty();
    
private:
    void ThreadCall();
    
private:
    std::condition_variable m_ConditionVariable;
    std::queue<TExecuteBlock> m_Tasks;
    std::mutex m_Mutex;
    std::thread m_Thread;
    bool m_IsEnabled;
};
    
}; // namespace jam

#endif /* defined(CTHREADEXECUTOR_H) */
