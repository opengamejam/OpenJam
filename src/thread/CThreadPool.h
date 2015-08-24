//
//  CThreadPool.h
//  TestApp
//
//  Created by yev on 8/10/15.
//
//

#ifndef CTHREADPOOL_H
#define CTHREADPOOL_H

#include "Global.h"
#include "CThreadExecutor.h"

namespace jam
{
CLASS_PTR(CThreadPool);
CLASS_PTR(CThreadExecutor);
    
class CThreadPool final
{
public:
    enum ThreadType
    {
        Main,
        Background
    };
    
public:
    CThreadPool();
    ~CThreadPool();
     
    static CThreadPoolPtr Get();
    static bool IsMainThread();
    
    void Initialize(uint32_t threadsNum = 1);
    void Destroy();
    
    void RunAsync(ThreadType threadType, const CThreadExecutor::TExecuteBlock& block);
    void Update(unsigned long dt);
    
private:
    CThreadExecutorPtr FindLeisureExecutor();
    
private:
    static CThreadPoolPtr s_Instance;
    static std::thread::id s_MainThreadId;
    
    std::mutex m_Mutex;
    std::vector<CThreadExecutorPtr> m_ThreadExecutors;
    std::queue<CThreadExecutor::TExecuteBlock> m_MainThreadTasks;
};
    
}; // namespace jam

#endif /* defined(CTHREADPOOL_H) */
